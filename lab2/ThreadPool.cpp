#include "ThreadPool.h"
#include <iostream>

bool ThreadPool::working() const
{
	read_lock _(rw_lock);
	return workingUnsafe();
}

bool ThreadPool::workingUnsafe() const
{
	return initialized && !terminated;
}

void ThreadPool::initialize(const size_t worker_count)
{
	write_lock _(rw_lock);

	if (initialized || terminated)
	{
		return;
	}

	workers.reserve(worker_count);

	for (size_t id = 0; id < worker_count; id++)
	{
		workers.emplace_back(&ThreadPool::routine, this);
	}

	initialized = !workers.empty();
}

void ThreadPool::routine()
{
	while (true)
	{
		bool taskAcquired = false;
		TaskWithTimer taskWithTimer;

		{
			write_lock _(rw_lock);
			auto waitCondition = [this, &taskAcquired, &taskWithTimer]
			{
				taskAcquired = tasks.pop(taskWithTimer);
				return terminated || taskAcquired;
			};

			task_waiter.wait(_, waitCondition);
		}

		if (terminated && !taskAcquired)
		{
			return;
		}

		taskWithTimer.task();
	}
}

void ThreadPool::addTask(TaskWithTimer&& task)
{
	{
		read_lock _(rw_lock);
		if (!workingUnsafe()) {
			return;
		}
	}

	if (tasks.emplace(TaskWithTimer(task)))
	{
		task_waiter.notify_one();
	}
	else
	{
		write_lock _(rw_lock);
		std::cout << "New task is not added to the queue\n\n";
	}
}

void ThreadPool::terminate()
{
	{
		write_lock _(rw_lock);

		if (workingUnsafe())
		{
			terminated = true;
		}
		else
		{
			workers.clear();
			terminated = false;
			initialized = false;
			return;
		}
	}

	task_waiter.notify_all();

	for (std::thread& worker : workers)
	{
		worker.join();
	}

	workers.clear();
	terminated = false;
	initialized = false;
}

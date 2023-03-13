#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(const size_t workerCount)
{
	write_lock _(rw_lock);

	if (initialized || terminated)
	{
		return;
	}

	workers.reserve(workerCount);

	for (size_t id = 0; id < workerCount; id++)
	{
		workers.emplace_back(&ThreadPool::routine, this);
	}

	initialized = !workers.empty();
}

bool ThreadPool::working() const
{
	read_lock _(rw_lock);
	return workingUnsafe();
}

bool ThreadPool::workingUnsafe() const
{
	return initialized && !terminated;
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

}

void ThreadPool::terminate(bool shouldFinishTasks)
{
	{
		write_lock _(rw_lock);

		if (shouldFinishTasks)
		{
			tasks.clear();
		}

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

#include "ThreadPool.h"

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
		bool task_acquired = false;
		std::function<void()> task;

		{
			write_lock _(rw_lock);
			auto wait_condition = [this, &task_acquired, &task] 
			{
				task_acquired = tasks.pop(task);
				return terminated || task_acquired;
			};

			task_waiter.wait(_, wait_condition);
		}

		if (terminated && !task_acquired)
		{
			return;
		}


		task();
	}
}

void ThreadPool::addTask(std::function<void()>&& task)
{
	{
		read_lock _(rw_lock);
		if (!workingUnsafe()) {
			return;
		}
	}

	tasks.emplace(std::forward<std::function<void()>>(task));
	task_waiter.notify_one();
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

#pragma once

#include "TaskQueue.h"
#include "TaskWithTimer.h"
#include <vector>
#include <functional>

class ThreadPool
{
public:
	ThreadPool(const size_t workerCount);
	~ThreadPool() = default;

	void addTask(TaskWithTimer&& task);
	void terminate(bool shouldFinishTasks = true);
	void routine();

	bool working() const;
	bool workingUnsafe() const;


private:
	mutable read_write_lock rw_lock;
	mutable std::condition_variable_any task_waiter;

	std::vector<std::thread> workers;
	TaskQueue<TaskWithTimer> tasks;

	bool initialized = false;
	bool terminated = false;
};
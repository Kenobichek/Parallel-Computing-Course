#pragma once

#include "TaskQueue.h"
#include "TaskWithTimer.h"
#include <vector>
#include <functional>

class ThreadPool
{
public:
	ThreadPool() = default;
	~ThreadPool() = default;

	void addTask(TaskWithTimer&& task);
	void initialize(const size_t worker_count);
	void terminate();
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
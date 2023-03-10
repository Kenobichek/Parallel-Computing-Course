#pragma once
#include <queue>
#include <thread>
#include <shared_mutex>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;

template <typename T>
class TaskQueue
{
public:
	TaskQueue() = default;
	~TaskQueue() = default;

	size_t size() const;
	bool empty() const;
	bool pop(T& task);
	void emplace(T&& task);
	void clear();
	void changeTotalTime(const int seconds);

private:
	mutable read_write_lock rw_lock;
	std::queue<T> tasks;
	int totalTasksTime;
};
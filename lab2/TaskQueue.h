#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include "StatisticCollector.h"

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
	bool full() const;
	bool pop(T& task);
	bool emplace(T&& task);
	void clear();

private:
	bool hasFreeSpace(int taskTime);

	mutable read_write_lock rw_lock;

	std::queue<T> tasks;

	int totalTasksTime = 0;
	int maxTotalTasksTime = 60;

	std::shared_ptr<StatisticCollector> statisticCollector = StatisticCollector::getInstance();
};
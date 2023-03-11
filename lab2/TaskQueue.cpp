#include "TaskQueue.h"
#include <functional>
#include <utility>
#include "TaskWithTimer.h"

template <typename T>
bool TaskQueue<T>::empty() const
{
	read_lock _(rw_lock);
	return tasks.empty();
}

template <typename T>
size_t TaskQueue<T>::size() const
{
	read_lock _(rw_lock);
	return tasks.size();
}

template <typename T>
void TaskQueue<T>::clear()
{
	write_lock _(rw_lock);
	while (!tasks.empty())
	{
		tasks.pop();
	}
}

template<typename T>
bool TaskQueue<T>::hasFreeSpace(int taskTime)
{
	return taskTime + totalTasksTime <= maxTotalTasksTime;
}

template<typename T>
bool TaskQueue<T>::emplace(T&& task)
{
	if (hasFreeSpace(task.time))
	{
		totalTasksTime += task.time;
		tasks.emplace(std::forward<T>(task));
		return true;
	}
	
	return false;
}

template <typename T>
bool TaskQueue<T>::pop(T& task)
{
	write_lock _(rw_lock);
	if (tasks.empty())
	{
		return false;
	}
	else
	{
		task = std::move(tasks.front());
		totalTasksTime -= task.time;
		tasks.pop();
		return true;
	}
}

template class TaskQueue<TaskWithTimer>;

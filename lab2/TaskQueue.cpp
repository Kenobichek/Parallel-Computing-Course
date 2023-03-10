#include "TaskQueue.h"
#include <functional>
#include <utility>

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
void TaskQueue<T>::changeTotalTime(const int seconds)
{
	totalTasksTime += seconds;
}

template<typename T>
void TaskQueue<T>::emplace(T&& task)
{
	tasks.emplace(std::forward<T>(task));
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
		tasks.pop();
		return true;
	}
}

template class TaskQueue<std::function<void()>>;

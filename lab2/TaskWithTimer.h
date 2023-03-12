#pragma once
#include <functional>

struct TaskWithTimer 
{
	TaskWithTimer() 
	{
		id = 0;
		task = []{};
		time = 0;
	}

	TaskWithTimer(int id, 
		std::function<void()> task, 
		int time) : 
		id(id), task(task), time(time) {};

	int id;
	std::function<void()> task;
	int time;
};
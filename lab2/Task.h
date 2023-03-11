#pragma once
#include <mutex>

class Task
{
public:
	static void task(const int taskId, const int seconds);
	static int random(const int first, const int second);
};
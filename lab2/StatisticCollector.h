#pragma once
#include <memory>
#include <chrono>

class StatisticCollector
{
public:
	using timePoint = std::chrono::high_resolution_clock::time_point;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	static std::shared_ptr<StatisticCollector> getInstance();
	
	void resetTime();
	double elapsedTime() const;

	void addRejectedTask();
	int getRejectedTasksCount() const;

	StatisticCollector(StatisticCollector const&) = delete;
	StatisticCollector& operator=(StatisticCollector const&) = delete;

private:
	StatisticCollector() = default;
	timePoint startTime;
	int rejectedTasksCount = 0;
};
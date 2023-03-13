#include "StatisticCollector.h"

std::shared_ptr<StatisticCollector> StatisticCollector::getInstance()
{
    static std::shared_ptr<StatisticCollector> statisticCollector{ new StatisticCollector };
    return statisticCollector;
}

void StatisticCollector::resetTime()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void StatisticCollector::addRejectedTask()
{
    rejectedTasksCount++;
}

int StatisticCollector::getRejectedTasksCount() const
{
    return rejectedTasksCount;
}

double StatisticCollector::elapsedTime() const
{
    return std::chrono::duration_cast<second_t>(std::chrono::high_resolution_clock::now() - startTime).count();
}

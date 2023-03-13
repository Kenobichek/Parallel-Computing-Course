#include "ThreadPool.h"
#include "Task.h"
#include "StatisticCollector.h"

std::mutex mutex;

void addTask(std::shared_ptr<ThreadPool> threadPool, int tasksCount)
{
    for (int i = 0; i < tasksCount; i++)
    {
        mutex.lock();
        static int taskId = 1;

        int randomTime = Task::random(5, 12);
        auto task = std::bind(Task::task, taskId, randomTime);
        threadPool->addTask(TaskWithTimer{ taskId++, task, randomTime });
        mutex.unlock();
    }
}

int main() 
{
    auto statisticCollector = StatisticCollector::getInstance();
    statisticCollector->resetTime();

    int workersCount = 4;
    std::shared_ptr<ThreadPool> myThreadPool = std::make_shared<ThreadPool>(workersCount);

    int threadsCount = 3;
    int tasksСount = 4;

    std::vector<std::thread> threads;

    for (int i = 0; i < threadsCount; i++)
    {
        threads.push_back(std::thread(&addTask, myThreadPool, tasksСount));
    }

    std::printf("Queue filling time: %f\n\n", statisticCollector->elapsedTime());

    for (int i = 0; i < threadsCount; i++)
    {
        threads[i].join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    myThreadPool->terminate(false);

    std::printf("Time to complete all tasks: %f\n\n", statisticCollector->elapsedTime());
    std::printf("The number of rejected tasks: %d\n\n", statisticCollector->getRejectedTasksCount());

    return 0;
}

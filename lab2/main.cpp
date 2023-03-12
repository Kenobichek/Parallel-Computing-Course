#include "ThreadPool.h"
#include "Task.h"

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
    int workersCount = 4;
    std::shared_ptr<ThreadPool> myThreadPool = std::make_shared<ThreadPool>(workersCount);

    int threadsCount = 5;
    int tasksСount = 3;

    std::vector<std::thread> threads;

    for (int i = 0; i < threadsCount; i++)
    {
        threads.push_back(std::thread(&addTask, myThreadPool, tasksСount));
    }

    for (int i = 0; i < threadsCount; i++)
    {
        threads[i].join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    myThreadPool->terminate(false);

    
    return 0;
}

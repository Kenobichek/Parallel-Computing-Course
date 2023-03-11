#include "ThreadPool.h"
#include "Task.h"

void addTask(std::shared_ptr<ThreadPool> threadPool)
{
    static int taskId = 1;
    int randomTime = Task::random(5, 12);
    auto task = std::bind(Task::task, taskId, randomTime);
    threadPool->addTask(TaskWithTimer{ taskId++, task, randomTime });
}

int main() 
{
    std::shared_ptr<ThreadPool> myThreadPool = std::make_unique<ThreadPool>();

    myThreadPool->initialize(4);

    for (int i = 0; i < 10; i++)
    {
        addTask(myThreadPool);
    }

    myThreadPool->terminate();

    return 0;
}

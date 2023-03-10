#include "ThreadPool.h"
#include "Task.h"

void addTask(std::shared_ptr<ThreadPool> threadPool)
{
    int randSeconds = Task::random(5, 12);
    auto task = std::bind(Task::task, Task::random(5, 12));
    threadPool->addTask(task);
}

int main() 
{
    std::shared_ptr<Task> myTask = std::make_unique<Task>();
    std::shared_ptr<ThreadPool> myThreadPool = std::make_unique<ThreadPool>();

    myThreadPool->initialize(4);

    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);
    addTask(myThreadPool);

    myThreadPool->terminate();

    return 0;
}

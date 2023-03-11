#include "Task.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

std::mutex mutex;

void Task::task(const int taskId, const int seconds)
{  
    std::unique_lock<std::mutex> lk(mutex);
    std::printf("Thread ID: %d\n", std::this_thread::get_id());
    std::printf("Task ID:: %d\n", taskId);
    std::printf("The task takes a %d seconds\n\n", seconds);

    lk.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int Task::random(const int first, const int second)
{
    static auto rd = std::random_device{};
    static auto rng = std::default_random_engine{ rd() };
    static auto uni = std::uniform_int_distribution<int>{ first, second };
    return uni(rng);
}

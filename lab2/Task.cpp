#include "Task.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

std::mutex mutex;

void Task::task(const int seconds)
{  
    std::unique_lock<std::mutex> lk(mutex);
    std::cout << "The task takes a " << seconds << " seconds\n";
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

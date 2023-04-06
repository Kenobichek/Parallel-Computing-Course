#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <thread>

std::vector<int> generateRandomVector(const int sizeArray)
{
    std::vector<int> vec;
    int minValue = 0;
    int maxValue = 500;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue);

    for (int i = 0; i < sizeArray; i++)
    {
        vec.push_back(dis(gen));
    }

    return vec;
}


std::mutex mtx;

int sumOfOddMod2(const std::vector<int>& vec, int start, int end) 
{
    int sum = 0;

    for (int i = start; i < end; i++) 
    {
        if (vec[i] % 2 != 0) 
        {
            mtx.lock();
            sum += vec[i];
            mtx.unlock();
        }
    }

    return sum;
}

void task(const std::vector<int>& vec, int numThreads)
{
	std::vector<std::thread> threads(numThreads);
	int numPerThread = vec.size() / numThreads;
	int remaining = vec.size() % numThreads;
	int start = 0;

	for(int i = 0; i < numThreads; i++)
	{
		int end = start + numPerThread;

		if (i < remaining)
		{
			end++;
		}

		threads[i] = std::thread(&sumOfOddMod2, std::ref(vec), start, end);
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}

int main()
{
	std::vector<int> vectorSizes = { 50, 100, 500, 5000, 10000 };
    std::vector<int> numThreadsSizes = { 1, 2, 3, 4 };


	for(auto size : vectorSizes)
	{
        std::vector<int> vec = generateRandomVector(size);

        for (auto threads : numThreadsSizes)
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            task(vec, threads);
            auto endTime = std::chrono::high_resolution_clock::now(); 

            std::chrono::duration<double, std::milli> elapsed_time = endTime - startTime;
            std::cout << "Elapsed time: " << elapsed_time.count() << " ms" << std::endl;
        }
	}

    return 0;
}
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <thread>
#include <memory>
#include <atomic>

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
int sum = 0;
std::atomic<int> sum_atomic(0);

int sumOfOddMod2(const std::vector<int>& vec, int start, int end) 
{

    for (int i = start; i < end; i++) 
    {
        if (vec[i] % 2 == 1)
        {
			mtx.lock();
			sum = (sum + vec[i]) % 2;
			mtx.unlock();
        }
    }

    return sum;
}

void sumOfOddModUsingAtomic(const std::vector<int>& vec, int begin, int end)
{

	for (int i = begin; i < end; i++) 
	{
		if (vec[i] % 2 == 1)
		{
			int expected_sum = sum_atomic.load();
			while (!sum_atomic.compare_exchange_weak(expected_sum, (expected_sum + vec[i]) % 2))
			{
			}
		}
	}

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
		start = end;
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}

void taskUsingAtomic(const std::vector<int>& vec, int numThreads)
{
	std::vector<std::thread> threads(numThreads);
	int numPerThread = vec.size() / numThreads;
	int remaining = vec.size() % numThreads;
	int start = 0;


	for (int i = 0; i < numThreads; i++)
	{
		int end = start + numPerThread;

		if (i < remaining)
		{
			end++;
		}

		threads[i] = std::thread(&sumOfOddModUsingAtomic, std::ref(vec), start, end);
		start = end;
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}

using second_t = std::chrono::duration<double, std::ratio<1>>;

int main()
{
	std::vector<int> vectorSizes = { 1000, 10000, 100000 };
	std::vector<int> numThreadsSizes = { 1, 2, 3, 4 };

	for(auto size : vectorSizes)
	{
		std::vector<int> vec = generateRandomVector(size);

		std::printf("-----------------------------\n");
		std::printf("Vector size: %d\n", size);

		for (auto threads : numThreadsSizes)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			task(vec, threads);
			auto endTime = std::chrono::high_resolution_clock::now(); 

			std::chrono::duration<double, std::milli> elapsed_time = endTime - startTime;
			std::printf("Number of threads: %d\n", threads);
			std::printf("Elapsed time: %lf sec\n\n", std::chrono::duration_cast<second_t>(std::chrono::high_resolution_clock::now() - startTime).count());
			sum = 0;
		}

		std::printf("****************************\n");

		std::printf("USING ATOMIC\n");

		for (auto threads : numThreadsSizes)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			taskUsingAtomic(vec, threads);
			auto endTime = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> elapsed_time = endTime - startTime;
			std::printf("Number of threads: %d\n", threads);
			std::printf("Elapsed time: %lf sec\n\n", std::chrono::duration_cast<second_t>(std::chrono::high_resolution_clock::now() - startTime).count());
			sum_atomic.store(0);
		}
	}

	return 0;
}
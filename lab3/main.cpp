#include <iostream>
#include <vector>
#include <random>

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

int sumOfOddMod2(const std::vector<int>& vec) 
{
    int sum = 0;
    for (const auto& elem : vec) 
    {
        if (elem % 2 == 1) 
        {
            sum += elem;
        }
    }
    return sum;
}

int main()
{
	int sizeArray = 100;
    std::vector<int> vec = generateRandomVector(sizeArray);
	std::cout << sumOfOddMod2(vec);
}
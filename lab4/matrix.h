#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include <random>

class Matrix
{
public:

    static std::vector<std::vector<int>> createMatrix(const int size)
    {
        std::vector<std::vector<int>> matrix(size, std::vector<int>(size));

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(1, size);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i][j] = uni(rng);
            }
        }

        return matrix;
    }

    static void placeMaxRowElementsOnMainDiagonal(std::vector<std::vector<int>>& matrix, const int numThreads)
    {

        std::vector<std::thread> threads(numThreads);
        int numRowsPerThread = matrix.size() / numThreads;
        int remainingRows = matrix.size() % numThreads;
        int startRow = 0;

        for (int i = 0; i < numThreads; i++)
        {
            int endRow = startRow + numRowsPerThread;
            if (i < remainingRows)
            {
                endRow++;
            }

            threads[i] = std::thread(&placeMaxRowElementOnPosition, std::ref(matrix), startRow, endRow);
            startRow = endRow;
        }

        for (auto& thread : threads)
        {
            thread.join();
        }
    }

    static void print(const std::vector<std::vector<int>>& matrix)
    {
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

private:

    static void findMaxElementInARow(const std::vector<int>& row, double& maxVal)
    {
        maxVal = *std::max_element(row.begin(), row.end());
    }
    
    static void placeMaxRowElementOnPosition(std::vector<std::vector<int>>& matrix, const int startRow, const int endRow)
    {
        for (int i = startRow; i < endRow; i++)
        {
            double maxVal = 0;
            findMaxElementInARow(matrix[i], maxVal);
            matrix[i][i] = maxVal;
        }
    }
};

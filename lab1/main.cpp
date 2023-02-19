#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

class Matrix
{
public:

    static std::vector<std::vector<double>> createMatrix(const int size)
    {
        std::vector<std::vector<double>> matrix(size, std::vector<double>(size));

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                matrix[i][j] = (double)rand() / RAND_MAX;
            }
        }
        return matrix;
    }

    static void placeMaxRowElementsOnMainDiagonal(std::vector<std::vector<double>>& matrix, const int numThreads)
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

    static void print(const std::vector<std::vector<double>>& matrix)
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

    static void findMaxElementInARow(const std::vector<double>& row, double& maxVal)
    {
        maxVal = *std::max_element(row.begin(), row.end());
    }
    
    static void placeMaxRowElementOnPosition(std::vector<std::vector<double>>& matrix, const int startRow, const int endRow)
    {
        for (int i = startRow; i < endRow; i++)
        {
            double maxVal = 0;
            findMaxElementInARow(matrix[i], maxVal);
            matrix[i][i] = maxVal;
        }
    }
};


int main() 
{
    int logicalCores = std::thread::hardware_concurrency();

    std::vector<int> matrixSizes = { 50, 100, 500, 5000, 10000 };

    for (auto mSize : matrixSizes)
    {
        std::vector<std::vector<double>> matrix = Matrix::createMatrix(mSize);

        std::vector<int> numThreads = { 1, logicalCores / 2, logicalCores, logicalCores * 2, logicalCores * 4, logicalCores * 8, logicalCores * 16 };

        std::cout << "Matrix size: " << mSize << std::endl;
        for (auto threads : numThreads)
        {
            std::cout << "Threads: " << threads << " ";

            std::vector<std::vector<double>> matrixCopy(matrix);

            auto startTime = std::chrono::high_resolution_clock::now();
            Matrix::placeMaxRowElementsOnMainDiagonal(matrixCopy, threads);
            auto endTime = std::chrono::high_resolution_clock::now(); 

            std::chrono::duration<double, std::milli> elapsed_time = endTime - startTime;
            std::cout << "Elapsed time: " << elapsed_time.count() << " ms" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}

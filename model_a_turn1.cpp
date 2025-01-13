#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <functional>

inline int square(int x) {
    return x * x;
}

void squareAndSum(const std::vector<int>& arr, long long& result, int start, int end) {
    long long sum = 0;
    for (int i = start; i < end; ++i) {
        sum += square(arr[i]);
    }
    result = sum;
}

int main() {
    const int SIZE = 1000000;
    std::vector<int> input(SIZE);
    
    // Initialize the array with some values (1 to 1,000,000)
    for (int i = 0; i < SIZE; ++i) {
        input[i] = i + 1; // Fill with numbers from 1 to 1,000,000
    }

    // Number of threads to use
    const int NUM_THREADS = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(NUM_THREADS);
    std::vector<long long> results(NUM_THREADS, 0);
    
    // Launch threads to process the array
    int chunkSize = SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int startIdx = i * chunkSize;
        int endIdx = (i == NUM_THREADS - 1) ? SIZE : startIdx + chunkSize; // Handle last chunk
        threads[i] = std::thread(squareAndSum, std::ref(input), std::ref(results[i]), startIdx, endIdx);
    }

    // Join threads to the main thread
    for (auto& thread : threads) {
        thread.join();
    }

    // Calculate final sum
    long long totalSum = std::accumulate(results.begin(), results.end(), 0LL);
    
    std::cout << "Total Sum of Squares: " << totalSum << std::endl;

    return 0;
}

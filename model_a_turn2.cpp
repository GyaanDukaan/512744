#include <iostream>
#include <vector>
#include <numeric>
#include <execution>
#include <chrono>

// Function to square a number
inline int square(int x) {
    return x * x;
}

int main() {
    const int SIZE = 1000000;
    std::vector<int> input(SIZE);

    // Initialize the array with values from 1 to 1,000,000
    for (int i = 0; i < SIZE; ++i) {
        input[i] = i + 1;
    }

    // Measure performance of the original multi-threaded approach
    auto start = std::chrono::high_resolution_clock::now();

    long long sum_original = 0;
    for (int i = 0; i < SIZE; ++i) {
        sum_original += square(input[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_original = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Total Sum of Squares (Original): " << sum_original << std::endl;
    std::cout << "Original Duration: " << duration_original << " ms" << std::endl;

    // Measure performance of the optimized approach using Parallel STL
    start = std::chrono::high_resolution_clock::now();

    // Use std::transform to square elements in parallel
    std::vector<int> squared(input.size());
    std::transform(std::execution::par, input.begin(), input.end(), squared.begin(), square);

    // Use std::reduce to sum the squared values in parallel
    long long sum_optimized = std::reduce(std::execution::par, squared.begin(), squared.end(), 0LL);

    end = std::chrono::high_resolution_clock::now();
    auto duration_optimized = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Total Sum of Squares (Optimized): " << sum_optimized << std::endl;
    std::cout << "Optimized Duration: " << duration_optimized << " ms" << std::endl;

    return 0;
}

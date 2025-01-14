#include <iostream>
#include <vector>
#include <execution>
#include <numeric>
#include <chrono>
#include <atomic>

// Inline function to square a number
inline int square(int num) {
    return num * num;
}

// Main function to calculate the sum of squares using parallel algorithms
long long parallel_square_sum(const std::vector<int>& nums) {
    std::atomic<long long> total_sum(0); // Atomic variable for thread-safe summation
    
    // Parallel for_each with execution policy
    std::for_each(std::execution::par, nums.begin(), nums.end(), [&total_sum](int num) {
        total_sum += square(num);
    });
    
    return total_sum.load(); // Return the total sum atomically
}

int main() {
    const int array_size = 10000; // Size of the input array
    std::vector<int> nums(array_size);
    
    // Fill the array with sample data
    for (int i = 0; i < array_size; ++i) {
        nums[i] = i + 1; // Example: numbers from 1 to 10,000
    }

    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    long long total_sum = parallel_square_sum(nums);
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate execution duration
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Sum of squares: " << total_sum << std::endl;
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}

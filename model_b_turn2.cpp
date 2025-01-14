#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <atomic>
#include <algorithm>
#include <chrono>

inline int square(int num) {
    return num * num;
}

long long parallel_square_sum_stl(const std::vector<int>& nums) {
    auto start = std::chrono::high_resolution_clock::now();
    
    long long total_sum = std::reduce(std::execution::par, nums.begin(), nums.end(), 0LL,
        [](long long sum, int num) {
            return sum + square(num);
        }
    );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Parallel STL time: " << elapsed.count() << " ms" << std::endl;

    return total_sum;
}

long long parallel_square_sum_atomic(const std::vector<int>& nums) {
    const int num_threads = std::thread::hardware_concurrency();
    int chunk_size = nums.size() / num_threads;
    std::atomic<long long> results[num_threads];
    
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        int start_idx = i * chunk_size;
        int end_idx = (i == num_threads - 1) ? nums.size() : start_idx + chunk_size;
        results[i].store(0);

        std::thread t([&nums, &results, start_idx, end_idx, i] {
            long long sum = 0;
            for (int j = start_idx; j < end_idx; ++j) {
                sum += square(nums[j]);
            }
            results[i] = sum;
        });
        t.join();
    }
    
    long long total_sum = std::accumulate(results, results + num_threads, 0LL);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Parallel Atomic time: " << elapsed.count() << " ms" << std::endl;

    return total_sum;
}

int main() {
    const int array_size = 10000000; // Increased size for better performance comparison
    std::vector<int> nums(array_size);
    
    for (int i = 0; i < array_size; ++i) {
        nums[i] = i + 1;
    }

    // Run the previous code with std::thread
    long long prev_sum = parallel_square_sum(nums);

    // Run the optimized codes
    long long stl_sum = parallel_square_sum_stl(nums);
    long long atomic_sum = parallel_square_sum_atomic(nums);
    
    std::cout << "Sum of squares (previous code): " << prev_sum << std::endl;
    std::cout << "Sum of squares (parallel STL): " << stl_sum << std::endl;
    std::cout << "Sum of squares (parallel atomic): " << atomic_sum << std::endl;

    return 0;
}

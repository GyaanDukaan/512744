
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <time.h>
#include <ctime>
using namespace std;

//Inline function to square a number
inline int square(int x)
{
    return x * x;
}

// Function to calculate sum of squares of elements in a particular range
// using multi-threading.
long long int parallel_sum(const vector<int>& arr, int low, int high, int threadID) {

    long long int local_sum = 0;
    for (int i = low; i <= high; i++) {
        int num = arr[i];
        local_sum += square(num);
    }
    return local_sum;
}


int main()
{
    const int array_size = 1'000'000; // 1 million
    vector<int> arr(array_size);

    // Generate random numbers between 1 and 100
    for (int i = 0; i < array_size; i++) {
        arr[i] = rand() % 100 + 1;
    }
    
     chrono::time_point<chrono::system_clock> t1, t2;
    t1 = chrono::system_clock::now();

    int num_threads = 4; // You can change the number of threads as per your requirement.
    const int chunk_size = array_size / num_threads + 1;
    vector<thread> threads;
    atomic<long long int> sum(0);

    //Create and start threads to calculate sum
    for (int thread_id = 0; thread_id < num_threads; thread_id++)
    {
        int start = thread_id * chunk_size;
        int end = min((thread_id + 1) * chunk_size - 1, array_size - 1);
         threads.push_back(thread(parallel_sum, arr, start, end, thread_id));
    }
 
    // Wait for all threads to complete their tasks
    for (auto& thread : threads)
    {
        thread.join();
    }
 
    t2 = chrono::system_clock::now();
  
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
  
    // Output the final sum
    cout << "Final sum: " << sum << endl;
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;


    return 0;
}

 

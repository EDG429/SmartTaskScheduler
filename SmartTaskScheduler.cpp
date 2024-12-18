#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>

// Thread optimized code

// Global mutex for thread safety
std::mutex mtx;

// Function that simulates workload
void task_function(int task_id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //  simulates 100 ms of work
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Task " << task_id << " completed on thread " << std::this_thread::get_id() << "\n";
}

int main()
{
    // Step 1 - Detect the number of logical cores
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2; // Fallback for unknown hardware
    std::cout << "Detected " << num_threads << " logical cores.\n";

    // Step 2 - Create a dynamic thread pool
    std::vector<std::thread> thread_pool;
    const int total_tasks = 10; // Number of tasks to perform

    // Step 3 - Assign tasks to the threads in the pool
    for (int i = 0; i < total_tasks; i++) {
        if (thread_pool.size() >= num_threads) {
            // wait for threads to finish before creating more
            for (auto& t : thread_pool) {
                if (t.joinable()) t.join();
            }
            thread_pool.clear();
        }
        // Launch a new thread
        thread_pool.emplace_back(task_function, i);
    }

    // Step 4 - Ensure all threads are joined
    for (auto& t : thread_pool) {
        if (t.joinable()) t.join();
    }


    std::cout << "All tasks completed. Program optimized for " << num_threads << " threads." << std::endl;

    return 0;
}

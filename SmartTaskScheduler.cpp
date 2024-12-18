#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>

std::mutex mtx;

void print_message(const std::string& message) {
    // Lock the mutex to ensure only one thread prints at a time
    std::lock_guard<std::mutex> lock(mtx);

    // Print the message
    std::cout << message << std::endl;
}

int main()
{
    // Vector to hold multiple threads
    std::vector<std::thread> threads;

    // Launch 5 threads
    for (int i = 0; i < 5; i++) {
        // Create a thread that runs 'print_message' with a unique message
        threads.emplace_back(print_message, "Hello from thread " + std::to_string(i));
    }

    // Wait (join) for all threads to finish execution
    for (auto& thread : threads) {
        thread.join();
    }
    // Main thread prints this message after all threads are done
    std::cout << "C++ Environment Ready!" << std::endl;

    // Comment

    return 0;
}

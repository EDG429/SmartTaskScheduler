#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

std::mutex mtx;

void print_message(const std::string& message) {
    // Lock the mutex to ensure only one thread prints at a time
    std::lock_guard<std::mutex> lock(mtx);




}

int main()
{
    std::cout << "Hello World!\n";
}

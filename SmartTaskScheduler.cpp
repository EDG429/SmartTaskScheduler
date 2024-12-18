#include "Task.h"
#include "scheduler.h"
#include <thread>
#include <chrono>

int main() {
	// Create a Task Scheduler for tasks with string priorities
	TaskScheduler<std::string> scheduler;

	// Add some example tasks
	scheduler.add_task("Complete Project", "High", "2024-12-20");
	scheduler.add_task("Buy Groceries", "Medium", "2024-12-19");
	scheduler.add_task("Pay Bills", "Low", "2024-12-22");

	// Start a background thread for autosaving tasks
	std::thread autosave_thread([&scheduler]() {
		while (true) {
			scheduler.save_to_file("tasks_data.txt"); // Save to a file every 5 seconds
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		});

	// Main thread: Simulate user interaction
	std::this_thread::sleep_for(std::chrono::seconds(15)); // Simulate program running for 15 seconds
	std::cout << "Main thread ending.\n";

	// Detach the thread (so it doesn't block the program from exiting)
	autosave_thread.detach();

	return 0;
}
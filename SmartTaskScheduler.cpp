#include "scheduler.h"
#include <thread>
#include <chrono>
#include <atomic>

int main() {
	TaskScheduler<std::string> scheduler;

	// Load tasks from file
	scheduler.load_from_file("tasks_data.txt");

	// Add some example tasks
	scheduler.add_task("Complete Project", "High", "2024-12-20");
	scheduler.add_task("Buy Groceries", "Medium", "2024-12-19");
	scheduler.add_task("Pay Bills", "Low", "2024-12-22");

	// Atomic flag to control the autosave thread
	std::atomic<bool> running{ true };

	// Start the autosave thread
	std::thread autosave_thread([&scheduler, &running]() {
		while (running) {
			scheduler.save_to_file("tasks_data.txt");
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		});

	// Simulate main program activity
	scheduler.list_tasks();
	std::this_thread::sleep_for(std::chrono::seconds(15)); // Simulate user activity

	// Stop the autosave thread
	running = false;
	autosave_thread.join(); // Ensure thread finishes before exiting

	std::cout << "Program exiting cleanly.\n";

	return 0;
}

#include "scheduler.h"
#include <thread>
#include <chrono>
#include <atomic>

int main() {
	TaskScheduler<std::string> scheduler;

	// Load tasks from file at startup
	scheduler.load_from_file("tasks_data.txt");

	// Atomic flag to control the autosave thread
	std::atomic<bool> running{ true };

	// Start the background autosave thread
	std::thread autosave_thread([&scheduler, &running]() {
		while (running) {
			scheduler.save_to_file("tasks_data.txt");
			std::this_thread::sleep_for(std::chrono::seconds(5)); // Default autosave interval
		}
		});

	// Menu-driven program loop
	int choice = 0;
	while (choice != 4) {
		std::cout << "\n=== Task Scheduler ===\n";
		std::cout << "1. Add Task\n";
		std::cout << "2. Remove Task\n";
		std::cout << "3. List Tasks\n";
		std::cout << "4. Exit\n";
		std::cout << "Enter your choice: ";
		std::cin >> choice;

		if (choice == 1) {
			std::string name, priority, deadline;
			std::cout << "Enter task name: ";
			std::cin.ignore();
			std::getline(std::cin, name);
			std::cout << "Enter priority (High/Medium/Low): ";
			std::getline(std::cin, priority);
			std::cout << "Enter deadline (YYYY-MM-DD): ";
			std::getline(std::cin, deadline);
			scheduler.add_task(name, priority, deadline);
		}
		else if (choice == 2) {
			std::string name;
			std::cout << "Enter task name to remove: ";
			std::cin.ignore();
			std::getline(std::cin, name);
			scheduler.remove_task(name);
		}
		else if (choice == 3) {
			scheduler.list_tasks();
		}
		else if (choice == 4) {
			std::cout << "Exiting program...\n";
			running = false; // Signal autosave thread to stop
		}
		else {
			std::cout << "Invalid choice. Try again.\n";
		}
	}

	// Wait for autosave thread to finish
	autosave_thread.join();
	std::cout << "Program exited cleanly.\n";

	return 0;
}
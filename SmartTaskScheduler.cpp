#include "scheduler.h"
#include <thread>
#include <chrono>
#include <atomic>

int main() {
	TaskScheduler<std::string> scheduler;

	// Load tasks at startup
	scheduler.load_from_file("tasks_data.txt");

	std::atomic<bool> running{ true };
	std::thread autosave_thread([&scheduler, &running]() {
		while (running) {
			scheduler.save_to_file("tasks_data.txt");
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		});

	int choice = 0;
	while (choice != 5) {
		std::cout << "\n=== Task Scheduler ===\n";
		std::cout << "1. Add Task\n";
		std::cout << "2. Remove Task\n";
		std::cout << "3. List Tasks\n";
		std::cout << "4. Sort Tasks\n";
		std::cout << "5. Exit\n";
		std::cout << "Enter your choice: ";
		std::cin >> choice;

		if (choice == 1) {
			std::string name, priority, deadline;
			std::cin.ignore();
			std::cout << "Enter task name: "; std::getline(std::cin, name);
			std::cout << "Enter priority (e.g., High): "; std::getline(std::cin, priority);
			std::cout << "Enter deadline (YYYY-MM-DD): "; std::getline(std::cin, deadline);
			scheduler.add_task(name, priority, deadline);
		}
		else if (choice == 2) {
			std::string name;
			std::cin.ignore();
			std::cout << "Enter task name to remove: "; std::getline(std::cin, name);
			scheduler.remove_task(name);
		}
		else if (choice == 3) {
			scheduler.list_tasks();
		}
		else if (choice == 4) {
			std::string criteria;
			std::cout << "Sort by 'name', 'priority', or 'deadline': ";
			std::cin >> criteria;
			scheduler.sort_tasks(criteria);
		}
		else if (choice == 5) {
			running = false;
			std::cout << "Exiting program...\n";
		}
		else {
			std::cout << "Invalid choice. Try again.\n";
		}
	}

	autosave_thread.join();
	std::cout << "Program exited cleanly.\n";

	return 0;
}
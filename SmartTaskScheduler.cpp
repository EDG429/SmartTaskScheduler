#include "scheduler.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <regex>

// Function to validate the date format (YYYY-MM-DD)
bool is_valid_date(const std::string& date) {
	std::regex date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");
	if (!std::regex_match(date, date_pattern)) return false;

	int year, month, day;
	sscanf_s(date.c_str(), "%4d-%2d-%2d", &year, &month, &day);

	// Basic checks for month and day range
	if (month < 1 || month > 12) return false;
	if (day < 1 || day > 31) return false;

	return true;
}

// Function to validate task priority
bool is_valid_priority(const std::string& priority) {
	return (priority == "Low" || priority == "Medium" || priority == "High");
}

int main() {
	TaskScheduler<std::string> scheduler;

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
		std::cout << "1. Add Task\n2. Remove Task\n3. List Tasks\n4. Sort Tasks\n5. Exit\n";
		std::cin >> choice;

		if (choice == 1) {
			std::string name, priority, deadline;
			std::cin.ignore();

			// Input task name
			std::cout << "Task name: ";
			std::getline(std::cin, name);

			// Input and validate priority
			do {
				std::cout << "Priority (Low/Medium/High): ";
				std::getline(std::cin, priority);
				if (!is_valid_priority(priority)) {
					std::cout << "Invalid priority! Please enter 'Low', 'Medium', or 'High'.\n";
				}
			} while (!is_valid_priority(priority));

			// Input and validate deadline
			do {
				std::cout << "Deadline (YYYY-MM-DD): ";
				std::getline(std::cin, deadline);
				if (!is_valid_date(deadline)) {
					std::cout << "Invalid date format! Please enter in YYYY-MM-DD format.\n";
				}
			} while (!is_valid_date(deadline));

			// Add the task
			scheduler.add_task(name, priority, deadline);
			std::cout << "Task added successfully!\n";
		}
		else if (choice == 2) {
			std::string name;
			std::cin.ignore();
			std::cout << "Task to remove: ";
			std::getline(std::cin, name);
			scheduler.remove_task(name);
		}
		else if (choice == 3) {
			scheduler.list_tasks();
		}
		else if (choice == 4) {
			std::string criteria;
			std::cout << "Sort by (name/priority/deadline): ";
			std::cin >> criteria;
			scheduler.sort_tasks(criteria);
		}
		else if (choice == 5) {
			running = false;
			std::cout << "Exiting program...\n";
		}
		else {
			std::cout << "Invalid choice! Please try again.\n";
		}
	}

	autosave_thread.join();
	std::cout << "Program exited cleanly.\n";
	return 0;
}

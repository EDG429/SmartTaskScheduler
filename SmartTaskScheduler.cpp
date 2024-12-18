#include "Task.h"
#include <vector>

int main() {
	// Example usage of the Task class with different priority types
	Task<int> task1("Complete Project", 1, std::string("2024-12-20"));
	Task<std::string> task2("Buy Groceries", std::string("High"), std::string("2024-12-19"));
	Task<double> task3("Pay Bills", 2.5, std::string("2024-12-22"));

	// Store tasks in a vector (we'll use smart pointers later)
	std::vector<Task<std::string>> string_priority_tasks;
	std::vector<Task<int>> int_priority_tasks;

	int_priority_tasks.emplace_back(std::string("Finish Homework"), 2, std::string("2024-12-18"));
	string_priority_tasks.emplace_back(std::string("Call Doctor"), std::string("Medium"), std::string("2024-12-21"));

	// Display tasks
	std::cout << "Testing Task Class:" << std::endl;
	task1.display();
	task2.display();
	task3.display();

	std::cout << "\nTasks in Vector:" << std::endl;
	for (const auto& task : int_priority_tasks) {
		task.display();
	}
	for (const auto& task : string_priority_tasks) {
		task.display();
	}

	return 0;
}

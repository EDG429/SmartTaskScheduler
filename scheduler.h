#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include <vector>
#include <memory> // << unique_ptr
#include <algorithm> // for std::remove_if, used in removing tasks
#include <iostream>   // For std::cout and std::endl
#include <fstream>    // For file I/O
#include <mutex>      // For std::mutex
#include <sstream>    // For std::istringstream


// Templated Task Scheduler class is responsible for managing a collection of tasks dynamically.
// It uses templates so it can handle tasks with different types of priorities (e.g., int, string).
template <typename T>
class TaskScheduler {
private:
	// A vector to store tasks. Each task is managed by a unique pointer to ensure automatic cleanup.	
	std::vector<std::unique_ptr<Task<T>>> tasks;
	// A mutex to protect tasks from concurrent access
	std::mutex tasks_mutex;

public:
	// Function to add a new task dynamically with a name, priority, and deadline. Then stores the new task in the vector.
	void add_task(const std::string& name, const T& priority, const std::string& deadline) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		tasks.push_back(std::make_unique<Task<T>>(name, priority, deadline));
	}

	// Function to remove a task by its name,prevents the task list from growing indefinitely and keeps it relevant.
	void remove_task(const std::string& name) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		// std::remove_if is used to find tasks that match the given name.
		auto remove_position = std::remove_if(tasks.begin(), tasks.end(),
			/*The lambda defines the condition for removal: task->get_name() == name.
			It keeps the logic inline and avoids writing a separate function.
			The lambda returns true if the task name matches the one we want to remove.*/
			[&](const std::unique_ptr<Task<T>>& task) -> bool {				
				return task->get_name() == name;
			});

		// If std::remove_if moved any tasks, they need to be erased.
		// Why erase tasks here?
		// - std::remove_if doesn't actually delete elements; it only rearranges them.
		// - The erase call cleans up the logically "removed" tasks from the vector.
		if (remove_position != tasks.end()) {
			tasks.erase(remove_position, tasks.end());
			std::cout << "Task \"" << name << "\" removed. \n";
		}
		else {
			// If no tasks were removed, notify the user.
			std::cout << "Task \"" << name << "\" not found.\n";
		}
	}

	// Function to display all tasks
	void list_tasks() {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		// If there are no tasks, let the user know.
		if (tasks.empty()) {
			std::cout << "No tasks to display.\n";
			return;
		}
	}

	// Function to save tasks to a file
	void save_to_file(const std::string& filename) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		std::ofstream file(filename);
		if (file.is_open()) {
			for (const auto& task : tasks) {
				file << task->get_name() << ","
					<< task->get_priority() << ","
					<< task->get_deadline() << "\n";
			}
			file.close();
			std::cout << "Tasks saved to " << filename << "\n";
		}
		else {
			std::cerr << "Failed to open file: " << filename << "\n";
		}
		
	}

	// Function to load from a file
	void load_from_file(const std::string& filename) {
		std::lock_guard<std::mutex> lock(tasks_mutex);

		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << "\n";
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string name, priority_str, deadline;

			// Parse the .csv file
			if (std::getline(iss, name, ',') &&
				std::getline(iss, priority_str, ',') &&
				std::getline(iss, deadline)) {

				// Convert the priority to the correct type
				std::istringstream priority_stream(priority_str);
				T priority;
				priority_stream >> priority;

				// Add the task to the scheduler
				tasks.push_back(std::make_unique<Task<T>>(name, priority, deadline));
			}
		}
		file.close();
		std::cout << "Tasks loaded from " << filename << "\n";
	}
};

#endif // !SCHEDULER_H


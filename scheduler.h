#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include <vector>
#include <memory> // << unique_ptr
#include <algorithm> // for std::remove_if, used in removing tasks

// Templated Task Scheduler class is responsible for managing a collection of tasks dynamically.
// It uses templates so it can handle tasks with different types of priorities (e.g., int, string).
template <typename T>
class TaskScheduler {
private:
	// A vector to store tasks. Each task is managed by a unique pointer to ensure automatic cleanup.	
	std::vector<std::unique_ptr<Task<T>>> tasks;

public:
	// Function to add a new task dynamically with a name, priority, and deadline. Then stores the new task in the vector.
	void add_task(const std::string& name, const T& priority, const std::string& deadline) {
		tasks.push_back(std::make_unique<Task<T>>(name, priority, deadline));
	}

	// Function to remove a task by its name,prevents the task list from growing indefinitely and keeps it relevant.
	void remove_task(const std::string& name) {
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
	void list_tasks() const {
		// If there are no tasks, let the user know.
		if (tasks.empty()) {
			std::cout << "No tasks to display.\n";
			return;
		}
	}
};

#endif // !SCHEDULER_H


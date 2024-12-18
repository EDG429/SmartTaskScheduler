#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>

template <typename T>
class TaskScheduler {
private:
	std::vector<std::unique_ptr<Task<T>>> tasks; // Task storage
	std::mutex tasks_mutex;                     // Mutex to ensure thread safety
	std::vector<std::thread> thread_pool;       // Worker threads for background tasks
	unsigned int available_threads;             // Number of threads available

public:
	TaskScheduler() {
		available_threads = std::thread::hardware_concurrency();
		if (available_threads == 0) available_threads = 2; // Fallback for systems reporting 0
		std::cout << "Detected " << available_threads << " hardware threads.\n";
	}

	~TaskScheduler() {
		for (auto& t : thread_pool) {
			if (t.joinable()) t.join();
		}
	}

	// Add task
	void add_task(const std::string& name, const T& priority, const std::string& deadline) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		tasks.push_back(std::make_unique<Task<T>>(name, priority, deadline));
	}

	// Remove task
	void remove_task(const std::string& name) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		auto remove_position = std::remove_if(tasks.begin(), tasks.end(),
			[&](const std::unique_ptr<Task<T>>& task) {
				return task->get_name() == name;
			});

		if (remove_position != tasks.end()) {
			tasks.erase(remove_position, tasks.end());
			std::cout << "Task \"" << name << "\" removed.\n";
		}
		else {
			std::cout << "Task \"" << name << "\" not found.\n";
		}
	}

	// Display tasks
	void list_tasks() {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		if (tasks.empty()) {
			std::cout << "No tasks to display.\n";
			return;
		}
		std::cout << "Tasks:\n";
		for (const auto& task : tasks) {
			task->display();
		}
	}

	// Sort tasks in parallel
	void sort_tasks(const std::string& criteria) {
		auto sort_worker = [&](const std::string& sort_criteria) {
			std::lock_guard<std::mutex> lock(tasks_mutex);
			if (sort_criteria == "name") {
				std::sort(tasks.begin(), tasks.end(),
					[](const std::unique_ptr<Task<T>>& a, const std::unique_ptr<Task<T>>& b) {
						return a->get_name() < b->get_name();
					});
				std::cout << "Tasks sorted by name.\n";
			}
			else if (sort_criteria == "priority") {
				std::sort(tasks.begin(), tasks.end(),
					[](const std::unique_ptr<Task<T>>& a, const std::unique_ptr<Task<T>>& b) {
						return a->get_priority() < b->get_priority();
					});
				std::cout << "Tasks sorted by priority.\n";
			}
			else if (sort_criteria == "deadline") {
				std::sort(tasks.begin(), tasks.end(),
					[](const std::unique_ptr<Task<T>>& a, const std::unique_ptr<Task<T>>& b) {
						return a->get_deadline() < b->get_deadline();
					});
				std::cout << "Tasks sorted by deadline.\n";
			}
			};

		// Launch sorting on a separate thread for optimization
		thread_pool.emplace_back(std::thread(sort_worker, criteria));
	}

	// Save tasks to file
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
		}
	}

	// Load tasks from file
	void load_from_file(const std::string& filename) {
		std::lock_guard<std::mutex> lock(tasks_mutex);
		std::ifstream file(filename);
		if (!file.is_open()) return;

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string name, priority_str, deadline;

			if (std::getline(iss, name, ',') &&
				std::getline(iss, priority_str, ',') &&
				std::getline(iss, deadline)) {
				std::istringstream priority_stream(priority_str);
				T priority;
				priority_stream >> priority;
				tasks.push_back(std::make_unique<Task<T>>(name, priority, deadline));
			}
		}
		file.close();
		std::cout << "Tasks loaded from " << filename << "\n";
	}
};

#endif // SCHEDULER_H

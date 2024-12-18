#ifndef TASK_H
#define TASK_H

#include <string>
#include <iostream>

// Templated class to allow flexible priority types
template <typename T>
class Task {
private:
	std::string name; 
	T priority;  
	std::string deadline;
public:
	// Constructor
	Task(const std::string& task_name, const T& task_priority, std::string& task_deadline) :
		name(task_name), priority(task_priority), deadline(task_deadline) { }
	
	// Getter functions
	std::string get_name() const { return name; }
	T get_priority() const { return priority; }
	std::string get_deadline() const { return deadline; }

	// Display task (for debugging user interface)
	void display() const {
		std::cout << "Task: " << name
			<< " | Priority: " << priority
			<< " | Deadline: " << deadline << std::endl;
	}
};

#endif // TASK_H

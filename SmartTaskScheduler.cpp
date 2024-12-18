#include "Task.h"
#include "scheduler.h"

int main() {
	// Create a Task Scheduler for tasks with string priorities
	TaskScheduler<std::string> scheduler;

	// Add tasks to the scheduler
	scheduler.add_task("Complete Project", "High", "2024-12-20");
	scheduler.add_task("Buy Groceries", "Medium", "2024-12-19");
	scheduler.add_task("Pay Bills", "Low", "2024-12-22");

	// List tasks
	std::cout << "Initial Task List:\n";
	scheduler.list_tasks();

	// Remove a task by name
	scheduler.remove_task("Buy Groceries");

	// List tasks after removal
	std::cout << "\nTask List After Removal:\n";
	scheduler.list_tasks();

	// Attempt to remove a non-existent task
	scheduler.remove_task("Nonexistent Task");

	return 0;
}
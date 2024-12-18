# SmartTaskScheduler
Smart Task Scheduler is a dynamic and efficient task management console application written in C++. It intelligently adapts to the user's system resources (CPU cores and threads) to optimize task execution and background processes. Designed to be lightweight, fast, and scalable, this project showcases practical use of multi-threading, mutexes, templates, and modern C++ concepts.

# Features
## Dynamic Thread Optimization
Automatically detects the number of logical CPU cores (threads) and adjusts task execution accordingly.
Efficient use of system resources for multi-threaded operations.

## Task Management
Add, list, remove, and prioritize tasks.
Tasks can include names, deadlines, and priorities.

## Background Autosave
A dedicated background thread periodically saves tasks to a file.
Ensures no data is lost, even in the event of unexpected shutdowns.

## Thread-Safe Operations
Mutexes ensure safe access to shared resources across multiple threads, preventing data corruption.

## Templates for Flexibility
Task priorities can be stored as integers, strings, or custom-defined types using C++ templates.

# Technologies Used
C++17 
Multi-threading 
Dynamic Memory Management 
Templates for reusable and flexible code
File I/O for task persistence

# How it works
Thread Management: The app detects the system’s hardware concurrency (logical cores) and adjusts its threads for optimal performance.
Task Handling: Users can add, list, and remove tasks. Tasks are stored dynamically and can include priorities, deadlines, and descriptions.
Autosave: A background thread saves tasks to a file every few seconds.
Thread Safety: Mutexes ensure all threads access the task list safely.

# License
This project is licensed under the MIT License.

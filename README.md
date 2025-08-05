# QuantaSerene

## About QuantaSerene
QuantaSerene is a powerful tool for managing conflicting tasks between AI agents. It provides a robust framework for prioritizing tasks, resolving scheduling conflicts, and ensuring smooth and efficient multi-agent operations.

## Key Features
- **Task Prioritization:** Dynamically prioritize tasks based on urgency and importance. The scheduler sorts tasks by their priority number (lower is higher).
- **Conflict Resolution:** Automatically detects and flags tasks with unmet dependencies. The current implementation identifies if a precedent task is not yet completed.
- **Scheduling Engine:** An intelligent scheduling engine to optimize task allocation and agent workloads. The current version assigns tasks to the first available (IDLE) agent.
- **Agent Management:** Define agents with specific capabilities and track their status (IDLE, BUSY, OFFLINE).

## Getting Started

### Prerequisites
- A C++ compiler (g++)
- Make

### Building the Application
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/QuantaSerene.git
   ```
2. Navigate to the project directory:
   ```bash
   cd QuantaSerene
   ```
3. Build the application using the Makefile:
   ```bash
   make
   ```
   This will compile the source files and create an executable named `quantaserene` in the root directory.

## Usage
Here is a basic example of how to use the QuantaSerene scheduler in your C++ application.

```cpp
#include "src/Scheduler.h"
#include "src/Agent.h"
#include "src/Task.h"

int main() {
    // Initialize the scheduler
    Scheduler scheduler;

    // Create agents
    Agent agent1(1, "Agent Alpha", IDLE, {101, 102});
    Agent agent2(2, "Agent Beta", IDLE, {103, 104});
    scheduler.addAgent(agent1);
    scheduler.addAgent(agent2);

    // Create tasks
    Task task1(1, "Analyze data", 1, 60, PENDING, {});
    Task task2(2, "Generate report", 2, 30, PENDING, {1}); // Depends on task 1
    Task task3(3, "Send email", 3, 10, PENDING, {2});    // Depends on task 2
    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.addTask(task3);

    // Run the scheduler
    scheduler.scheduleTasks();

    // Print the status
    scheduler.printTasks();
    scheduler.printAgents();

    return 0;
}
```

### Running the Example
To run the main application which includes a sample execution:
```bash
./quantaserene
```

## Future Enhancements
For more details on planned features and future enhancements, please see the `docs/plan.md` and `docs/enhancements.md` files.

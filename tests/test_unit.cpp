#include "../src/Scheduler.h"
#include "../src/Task.h"
#include "../src/Agent.h"
#include <iostream>
#include <vector>
#include <cstdlib> // For exit()

// Native C++ assertion macro
#define NATIVE_ASSERT(condition) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: (" #condition "), function " << __func__ \
                  << ", file " << __FILE__ << ", line " << __LINE__ << "." << std::endl; \
        exit(1); \
    }

// Note: The following is a common, albeit intrusive, technique for testing private members.
// Since we are not allowed to modify the original source code, we redefine the class
// with public members for testing purposes. This is a workaround for the "no-compile"
// and "no-modify" constraints. A better approach would be to refactor the Scheduler
// class to be more testable (e.g., using dependency injection or making state accessible).
#define private public
#include "../src/Scheduler.h"
#undef private

void test_task_prioritization_and_assignment() {
    std::cout << "Running test: test_task_prioritization_and_assignment..." << std::endl;

    // 1. Setup
    Scheduler scheduler;
    Agent agent1(1, "Agent1", IDLE, {});
    Agent agent2(2, "Agent2", IDLE, {});
    scheduler.addAgent(agent1);
    scheduler.addAgent(agent2);

    // Tasks are added out of priority order. Lower number is higher priority.
    Task task1(101, "Low priority task", 10, 60, PENDING, {});
    Task task2(102, "High priority task", 5, 30, PENDING, {});
    scheduler.addTask(task1);
    scheduler.addTask(task2);

    // 2. Execute
    scheduler.scheduleTasks();

    // 3. Assert
    // This test assumes we can access the private 'tasks' vector of the Scheduler.
    // The #define private public trick allows this for the test compilation.
    NATIVE_ASSERT(scheduler.tasks.size() == 2);

    // Check if the tasks were prioritized correctly before assignment.
    // The highest priority task (priority 5) should be first in the internal list after prioritization.
    // and thus assigned first.
    const Task& firstTask = scheduler.tasks[0];
    const Task& secondTask = scheduler.tasks[1];

    NATIVE_ASSERT(firstTask.id == 102); // High priority task should be first
    NATIVE_ASSERT(firstTask.status == IN_PROGRESS);
    NATIVE_ASSERT(firstTask.assignedAgentId == 1); // Assigned to the first available agent

    NATIVE_ASSERT(secondTask.id == 101); // Low priority task should be second
    NATIVE_ASSERT(secondTask.status == IN_PROGRESS);
    NATIVE_ASSERT(secondTask.assignedAgentId == 2); // Assigned to the second available agent

    std::cout << "PASSED" << std::endl;
}

void test_conflict_resolution() {
    std::cout << "Running test: test_conflict_resolution..." << std::endl;

    // 1. Setup
    Scheduler scheduler;
    Agent agent1(1, "Agent1", IDLE, {});
    scheduler.addAgent(agent1);

    // Task 202 depends on Task 201, which is not completed.
    Task task201(201, "Dependency task", 1, 60, PENDING, {});
    Task task202(202, "Dependent task", 2, 30, PENDING, {201}); // Depends on 201
    scheduler.addTask(task201);
    scheduler.addTask(task202);

    // 2. Execute
    scheduler.scheduleTasks();

    // 3. Assert
    const Task& dependencyTask = scheduler.tasks[0]; // Task 201
    const Task& dependentTask = scheduler.tasks[1];  // Task 202

    // The dependency task should be scheduled.
    NATIVE_ASSERT(dependencyTask.id == 201);
    NATIVE_ASSERT(dependencyTask.status == IN_PROGRESS);
    NATIVE_ASSERT(dependencyTask.assignedAgentId == 1);

    // The dependent task should be marked as CONFLICT because its dependency is not COMPLETED.
    NATIVE_ASSERT(dependentTask.id == 202);
    NATIVE_ASSERT(dependentTask.status == CONFLICT);
    NATIVE_ASSERT(dependentTask.assignedAgentId == -1); // Should not be assigned

    std::cout << "PASSED" << std::endl;
}

int main() {
    test_task_prioritization_and_assignment();
    test_conflict_resolution();
    return 0;
}

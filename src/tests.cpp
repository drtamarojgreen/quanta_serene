#include <cassert>
#include "Scheduler.h"

void test_addTask() {
    Scheduler scheduler;
    Task task(1, "Test Task", 1, 10, PENDING, {});
    scheduler.addTask(task);
    // This is a proxy test. We'll check the output of printTasks.
    // A more robust test would involve inspecting the scheduler's internal state.
    std::cout << "Running test_addTask..." << std::endl;
    scheduler.printTasks();
}

void test_addAgent() {
    Scheduler scheduler;
    Agent agent(1, "Test Agent", IDLE, {});
    scheduler.addAgent(agent);
    std::cout << "Running test_addAgent..." << std::endl;
    scheduler.printAgents();
}

void test_prioritization() {
    Scheduler scheduler;
    Task task1(1, "Low Prio", 3, 10, PENDING, {});
    Task task2(2, "High Prio", 1, 10, PENDING, {});
    scheduler.addTask(task1);
    scheduler.addTask(task2);

    // The scheduleTasks method calls prioritizeTasks internally
    scheduler.scheduleTasks();

    std::cout << "Running test_prioritization..." << std::endl;
    std::cout << "Tasks should be scheduled with High Prio first." << std::endl;
    scheduler.printTasks();
}

void test_scheduling() {
    Scheduler scheduler;
    Task task(1, "Test Task", 1, 10, PENDING, {});
    Agent agent(1, "Test Agent", IDLE, {});
    scheduler.addTask(task);
    scheduler.addAgent(agent);
    scheduler.scheduleTasks();

    std::cout << "Running test_scheduling..." << std::endl;
    scheduler.printTasks();
    scheduler.printAgents();
}


int main() {
    std::cout << "--- Running Tests ---" << std::endl;
    test_addTask();
    test_addAgent();
    test_prioritization();
    test_scheduling();
    std::cout << "--- Tests Complete ---" << std::endl;
    return 0;
}

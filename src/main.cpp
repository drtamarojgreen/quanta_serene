#include "Scheduler.h"

int main() {
    Scheduler scheduler;

    // Create some sample tasks
    Task task1 = {1, "Deploy web server", 1, 60, PENDING, {}};
    Task task2 = {2, "Update database schema", 1, 120, PENDING, {1}};
    Task task3 = {3, "Run security scan", 2, 90, PENDING, {}};

    // Create some sample agents
    Agent agent1 = {101, "Agent Smith", IDLE, {1, 2}};
    Agent agent2 = {102, "Agent Jones", IDLE, {3}};

    // Add tasks and agents to the scheduler
    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.addTask(task3);

    scheduler.addAgent(agent1);
    scheduler.addAgent(agent2);

    // Print the current state
    scheduler.printTasks();
    scheduler.printAgents();

    return 0;
}

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include "Agent.h"
#include <vector>
#include <iostream>

/**
 * @class Scheduler
 * @brief Manages tasks and agents, and runs the scheduling logic.
 *
 * The Scheduler class is the core of the QuantaSerene application. It maintains
 * lists of tasks and agents, and provides methods for adding them, prioritizing
 * tasks, resolving conflicts, and assigning tasks to agents.
 */
class Scheduler {
public:
    /**
     * @brief Adds a new task to the scheduler.
     * @param task The task to be added.
     */
    void addTask(const Task& task);

    /**
     * @brief Adds a new agent to the scheduler.
     * @param agent The agent to be added.
     */
    void addAgent(const Agent& agent);

    /**
     * @brief Prints a list of all current tasks and their status.
     */
    void printTasks() const;

    /**
     * @brief Prints a list of all current agents and their status.
     */
    void printAgents() const;

    /**
     * @brief Runs the main scheduling process.
     *
     * This method orchestrates the scheduling process by calling prioritization,
     * conflict resolution, and task assignment in the correct order.
     */
    void scheduleTasks();

private:
    /**
     * @brief Sorts tasks based on their priority.
     */
    void prioritizeTasks();

    /**
     * @brief Resolves conflicts between tasks, such as unmet dependencies.
     */
    void resolveConflicts();

    std::vector<Task> tasks;   ///< The list of tasks to be scheduled.
    std::vector<Agent> agents; ///< The list of available agents.
};

#endif // SCHEDULER_H

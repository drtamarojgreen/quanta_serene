#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <chrono>

/**
 * @brief Represents the current status of a task.
 */
enum TaskStatus {
    PENDING,      ///< The task is waiting to be scheduled.
    IN_PROGRESS,  ///< The task is currently being executed.
    COMPLETED,    ///< The task has been successfully completed.
    CONFLICT      ///< The task cannot be executed due to a conflict (e.g., unmet dependencies).
};

/**
 * @struct Task
 * @brief Represents a task to be executed by an agent.
 *
 * This struct holds all the information related to a task, including its
 * identity, priority, dependencies, and current status.
 */
struct Task {
    int id;                                  ///< Unique identifier for the task.
    std::string description;                 ///< A brief description of the task.
    int priority;                            ///< Priority of the task (lower number means higher priority).
    int duration;                            ///< Estimated time in minutes to complete the task.
    TaskStatus status;                       ///< Current status of the task.
    std::vector<int> dependencies;           ///< List of task IDs that must be completed before this one.
    int assignedAgentId = -1;                ///< ID of the agent assigned to this task (-1 if unassigned).
    std::chrono::system_clock::time_point scheduled_time; ///< The time when the task was scheduled.

    /**
     * @brief Construct a new Task object.
     * @param i The task's ID.
     * @param d The task's description.
     * @param p The task's priority.
     * @param dur The task's estimated duration.
     * @param s The task's initial status.
     * @param deps The task's dependencies.
     */
    Task(int i, const std::string& d, int p, int dur, TaskStatus s, const std::vector<int>& deps)
        : id(i), description(d), priority(p), duration(dur), status(s), dependencies(deps) {}
};

#endif // TASK_H

#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <chrono>
#include "Json.h"

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
    long long target_start_time = 0;         ///< The target start time (Unix timestamp). 0 means immediate.

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

    Task(int i, const std::string& d, int p, int dur, TaskStatus s, const std::vector<int>& deps, long long target)
        : id(i), description(d), priority(p), duration(dur), status(s), dependencies(deps), target_start_time(target) {}

    Task() : id(0), priority(0), duration(0), status(PENDING) {}
};

inline JsonValue task_to_json(const Task& t) {
    JsonValue j = JsonValue::object();
    j["id"] = t.id;
    j["description"] = t.description;
    j["priority"] = t.priority;
    j["duration"] = t.duration;
    j["status"] = static_cast<int>(t.status);
    JsonValue deps = JsonValue::array();
    for (int d : t.dependencies) deps.push_back(d);
    j["dependencies"] = deps;
    j["assignedAgentId"] = t.assignedAgentId;
    j["target_start_time"] = t.target_start_time;
    return j;
}

inline Task task_from_json(const JsonValue& j) {
    Task t;
    if (j.contains("id")) t.id = j["id"].get_int();
    if (j.contains("description")) t.description = j["description"].get_string();
    if (j.contains("priority")) t.priority = j["priority"].get_int();
    if (j.contains("duration")) t.duration = j["duration"].get_int();
    if (j.contains("status")) t.status = static_cast<TaskStatus>(j["status"].get_int());
    if (j.contains("dependencies")) {
        for (const auto& d : j["dependencies"].get_array()) t.dependencies.push_back(d.get_int());
    }
    if (j.contains("assignedAgentId")) t.assignedAgentId = j["assignedAgentId"].get_int();
    if (j.contains("target_start_time")) t.target_start_time = static_cast<long long>(j["target_start_time"].get_number());
    return t;
}

#endif // TASK_H

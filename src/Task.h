#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <chrono>

enum TaskStatus {
    PENDING,
    IN_PROGRESS,
    COMPLETED,
    CONFLICT
};

struct Task {
    int id;
    std::string description;
    int priority; // Lower number means higher priority
    int duration; // Estimated time in minutes
    TaskStatus status;
    std::vector<int> dependencies; // List of task IDs that must be completed before this one
    int assignedAgentId = -1; // ID of the agent assigned to this task
    std::chrono::system_clock::time_point scheduled_time;

    // Constructor to handle initialization
    Task(int i, const std::string& d, int p, int dur, TaskStatus s, const std::vector<int>& deps)
        : id(i), description(d), priority(p), duration(dur), status(s), dependencies(deps) {}
};

#endif // TASK_H

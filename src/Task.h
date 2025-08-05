#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

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
};

#endif // TASK_H

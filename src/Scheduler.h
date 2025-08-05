#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include "Agent.h"
#include <vector>
#include <iostream>

class Scheduler {
public:
    void addTask(const Task& task);
    void addAgent(const Agent& agent);
    void printTasks() const;
    void printAgents() const;

private:
    std::vector<Task> tasks;
    std::vector<Agent> agents;
};

#endif // SCHEDULER_H

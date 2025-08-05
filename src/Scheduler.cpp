#include "Scheduler.h"
#include <algorithm>

void Scheduler::addTask(const Task& task) {
    tasks.push_back(task);
    std::cout << "Task " << task.id << " added." << std::endl;
}

void Scheduler::addAgent(const Agent& agent) {
    agents.push_back(agent);
    std::cout << "Agent " << agent.id << " added." << std::endl;
}

void Scheduler::printTasks() const {
    std::cout << "--- Tasks ---" << std::endl;
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.id << ", Desc: " << task.description
                  << ", Prio: " << task.priority << ", Status: " << task.status
                  << ", Agent: " << task.assignedAgentId << std::endl;
    }
}

void Scheduler::printAgents() const {
    std::cout << "--- Agents ---" << std::endl;
    for (const auto& agent : agents) {
        std::cout << "ID: " << agent.id << ", Name: " << agent.name
                  << ", Status: " << agent.status << ", Tasks: " << agent.assignedTasks.size() << std::endl;
    }
}

void Scheduler::prioritizeTasks() {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.priority < b.priority;
    });
}

void Scheduler::resolveConflicts() {
    // Basic conflict resolution: check dependencies
    for (auto& task : tasks) {
        for (int depId : task.dependencies) {
            bool dependencyMet = false;
            for (const auto& otherTask : tasks) {
                if (otherTask.id == depId && otherTask.status == COMPLETED) {
                    dependencyMet = true;
                    break;
                }
            }
            if (!dependencyMet) {
                task.status = CONFLICT;
                break; // No need to check other dependencies if one is not met
            }
        }
    }
}

void Scheduler::scheduleTasks() {
    prioritizeTasks();
    resolveConflicts();

    for (auto& task : tasks) {
        if (task.status == PENDING) {
            for (auto& agent : agents) {
                if (agent.status == IDLE) {
                    // Simple assignment: first available agent takes the task
                    // A more advanced scheduler would check agent capabilities
                    task.status = IN_PROGRESS;
                    task.assignedAgentId = agent.id;
                    task.scheduled_time = std::chrono::system_clock::now();
                    agent.status = BUSY;
                    agent.assignedTasks.push_back(task.id);
                    std::cout << "Task " << task.id << " assigned to Agent " << agent.id << std::endl;
                    break; // Move to the next task
                }
            }
        }
    }
}

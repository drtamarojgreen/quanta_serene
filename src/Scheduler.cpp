#include "Scheduler.h"

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
        std::cout << "ID: " << task.id << ", Desc: " << task.description << ", Prio: " << task.priority << std::endl;
    }
}

void Scheduler::printAgents() const {
    std::cout << "--- Agents ---" << std::endl;
    for (const auto& agent : agents) {
        std::cout << "ID: " << agent.id << ", Name: " << agent.name << std::endl;
    }
}

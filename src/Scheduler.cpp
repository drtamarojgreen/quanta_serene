#include "Scheduler.h"
#include <algorithm>
#include <fstream>
#include <iomanip>

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

void Scheduler::loadTasks(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        JsonValue j = JsonValue::parse(ss.str());
        tasks.clear();
        for (const auto& item : j.get_array()) {
            tasks.push_back(task_from_json(item));
        }
        std::cout << "Loaded " << tasks.size() << " tasks from " << filename << std::endl;
    } else {
        std::cerr << "Could not open file " << filename << " for reading tasks." << std::endl;
    }
}

void Scheduler::saveTasks(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        JsonValue j = JsonValue::array();
        for (const auto& t : tasks) {
            j.push_back(task_to_json(t));
        }
        file << j.serialize() << std::endl;
        std::cout << "Saved tasks to " << filename << std::endl;
    } else {
        std::cerr << "Could not open file " << filename << " for writing tasks." << std::endl;
    }
}

void Scheduler::loadAgents(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        JsonValue j = JsonValue::parse(ss.str());
        agents.clear();
        for (const auto& item : j.get_array()) {
            agents.push_back(agent_from_json(item));
        }
        std::cout << "Loaded " << agents.size() << " agents from " << filename << std::endl;
    } else {
        std::cerr << "Could not open file " << filename << " for reading agents." << std::endl;
    }
}

void Scheduler::saveAgents(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        JsonValue j = JsonValue::array();
        for (const auto& a : agents) {
            j.push_back(agent_to_json(a));
        }
        file << j.serialize() << std::endl;
        std::cout << "Saved agents to " << filename << std::endl;
    } else {
        std::cerr << "Could not open file " << filename << " for writing agents." << std::endl;
    }
}

void Scheduler::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        JsonValue config = JsonValue::parse(ss.str());
        if (config.contains("tasks_file")) {
            loadTasks(config["tasks_file"].get_string());
        }
        if (config.contains("agents_file")) {
            loadAgents(config["agents_file"].get_string());
        }
        std::cout << "Configuration loaded from " << filename << std::endl;
    } else {
        std::cerr << "Could not open config file " << filename << std::endl;
    }
}

void Scheduler::generateReport(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        JsonValue report = JsonValue::object();

        JsonValue tasks_j = JsonValue::array();
        for (const auto& t : tasks) tasks_j.push_back(task_to_json(t));
        report["tasks"] = tasks_j;

        JsonValue agents_j = JsonValue::array();
        for (const auto& a : agents) agents_j.push_back(agent_to_json(a));
        report["agents"] = agents_j;

        JsonValue summary = JsonValue::object();
        summary["total_tasks"] = static_cast<int>(tasks.size());
        summary["total_agents"] = static_cast<int>(agents.size());
        report["summary"] = summary;

        file << report.serialize() << std::endl;
        std::cout << "Report generated: " << filename << std::endl;
    } else {
        std::cerr << "Could not open file " << filename << " for generating report." << std::endl;
    }
}

void Scheduler::createBashScriptFromJson(const std::string& payload_json) const {
    JsonValue j = JsonValue::parse(payload_json);
    std::string script_name = j.contains("script_name") ? j["script_name"].get_string() : "generated_script.sh";

    std::ofstream file(script_name);
    if (file.is_open()) {
        file << "#!/bin/bash" << std::endl;
        if (j.contains("commands")) {
            for (const auto& cmd : j["commands"].get_array()) {
                file << cmd.get_string() << std::endl;
            }
        }
        file.close();
        std::cout << "Bash script created: " << script_name << std::endl;
    } else {
        std::cerr << "Could not open file " << script_name << " for writing bash script." << std::endl;
    }
}

void Scheduler::scheduleTasks() {
    prioritizeTasks();
    resolveConflicts();

    auto now = std::chrono::system_clock::now();
    auto now_ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    for (auto& task : tasks) {
        if (task.status == PENDING) {
            // Check if target start time has been reached
            if (task.target_start_time > 0 && task.target_start_time > now_ts) {
                std::cout << "Task " << task.id << " is scheduled for later (" << task.target_start_time << ")" << std::endl;
                continue;
            }

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

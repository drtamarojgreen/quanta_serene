#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>
#include "Json.h"

/**
 * @brief Represents the operational status of an agent.
 */
enum AgentStatus {
    IDLE,    ///< The agent is available for new tasks.
    BUSY,    ///< The agent is currently executing a task.
    OFFLINE  ///< The agent is not available.
};

/**
 * @struct Agent
 * @brief Represents an AI agent in the system.
 *
 * This struct holds all the information related to an agent, including its
 * identity, status, capabilities, and assigned tasks.
 */
struct Agent {
    int id;                        ///< Unique identifier for the agent.
    std::string name;              ///< Name of the agent.
    AgentStatus status;            ///< Current status of the agent.
    std::vector<int> capabilities; ///< List of skills/capabilities the agent has (e.g., represented by IDs).
    std::vector<int> assignedTasks;///< List of task IDs currently assigned to this agent.

    /**
     * @brief Construct a new Agent object.
     * @param i The agent's ID.
     * @param n The agent's name.
     * @param s The agent's initial status.
     * @param caps The agent's capabilities.
     */
    Agent(int i, const std::string& n, AgentStatus s, const std::vector<int>& caps)
        : id(i), name(n), status(s), capabilities(caps) {}

    Agent() : id(0), status(IDLE) {}
};

inline JsonValue agent_to_json(const Agent& a) {
    JsonValue j = JsonValue::object();
    j["id"] = a.id;
    j["name"] = a.name;
    j["status"] = static_cast<int>(a.status);
    JsonValue caps = JsonValue::array();
    for (int c : a.capabilities) caps.push_back(c);
    j["capabilities"] = caps;
    JsonValue tasks = JsonValue::array();
    for (int t : a.assignedTasks) tasks.push_back(t);
    j["assignedTasks"] = tasks;
    return j;
}

inline Agent agent_from_json(const JsonValue& j) {
    Agent a;
    if (j.contains("id")) a.id = j["id"].get_int();
    if (j.contains("name")) a.name = j["name"].get_string();
    if (j.contains("status")) a.status = static_cast<AgentStatus>(j["status"].get_int());
    if (j.contains("capabilities")) {
        for (const auto& c : j["capabilities"].get_array()) a.capabilities.push_back(c.get_int());
    }
    if (j.contains("assignedTasks")) {
        for (const auto& t : j["assignedTasks"].get_array()) a.assignedTasks.push_back(t.get_int());
    }
    return a;
}

#endif // AGENT_H

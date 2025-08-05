#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>

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
};

#endif // AGENT_H

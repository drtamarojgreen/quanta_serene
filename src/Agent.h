#ifndef AGENT_H
#define AGENT_H

#include <string>
#include <vector>

enum AgentStatus {
    IDLE,
    BUSY,
    OFFLINE
};

struct Agent {
    int id;
    std::string name;
    AgentStatus status;
    std::vector<int> capabilities; // List of skills/capabilities the agent has
    std::vector<int> assignedTasks; // List of task IDs assigned to this agent

    // Constructor to handle initialization
    Agent(int i, const std::string& n, AgentStatus s, const std::vector<int>& caps)
        : id(i), name(n), status(s), capabilities(caps) {}
};

#endif // AGENT_H

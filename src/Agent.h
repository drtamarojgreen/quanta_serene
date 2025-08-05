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
};

#endif // AGENT_H

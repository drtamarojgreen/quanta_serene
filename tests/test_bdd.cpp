#include "../src/Scheduler.h"
#include "../src/Task.h"
#include "../src/Agent.h"
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cstdlib> // For exit()

// Native C++ assertion macro
#define NATIVE_ASSERT(condition) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: (" #condition "), function " << __func__ \
                  << ", file " << __FILE__ << ", line " << __LINE__ << "." << std::endl; \
        exit(1); \
    }

// As with the unit tests, we use this technique to access private members for verification
// without modifying the original source code.
#define private public
#include "../src/Scheduler.h"
#undef private

// A simple structure to hold the state for our BDD scenario
struct BDDContext {
    std::unique_ptr<Scheduler> scheduler;
    Agent agent1;
    Agent agent2;
    Task highPriorityTask;
    Task lowPriorityTask;
    Task conflictedTask;
};

void given_a_scheduler_with_agents_and_tasks(BDDContext& context) {
    std::cout << "GIVEN a scheduler with two idle agents and three tasks" << std::endl;
    context.scheduler.reset(new Scheduler());

    // Two idle agents are available
    context.agent1 = Agent(1, "Agent1", IDLE, {});
    context.agent2 = Agent(2, "Agent2", IDLE, {});
    context.scheduler->addAgent(context.agent1);
    context.scheduler->addAgent(context.agent2);

    // Three tasks are pending
    std::cout << "  - A high-priority task" << std::endl;
    context.highPriorityTask = Task(102, "High priority task", 5, 30, PENDING, {});
    std::cout << "  - A low-priority task" << std::endl;
    context.lowPriorityTask = Task(101, "Low priority task", 10, 60, PENDING, {});
    std::cout << "  - A task with an unmet dependency" << std::endl;
    context.conflictedTask = Task(202, "Dependent task", 2, 30, PENDING, {201}); // Depends on non-existent task 201

    context.scheduler->addTask(context.lowPriorityTask); // Added out of order
    context.scheduler->addTask(context.highPriorityTask);
    context.scheduler->addTask(context.conflictedTask);
}

void given_a_config_file(const std::string& config_file, const std::string& tasks_file) {
    std::cout << "GIVEN a config file " << config_file << " pointing to " << tasks_file << std::endl;
    std::ofstream t_file(tasks_file);
    t_file << R"([{"id": 301, "description": "JSON Task", "priority": 1, "duration": 10, "status": 0, "dependencies": []}])" << std::endl;
    t_file.close();

    std::ofstream c_file(config_file);
    c_file << "{\"tasks_file\": \"" << tasks_file << "\"}" << std::endl;
    c_file.close();
}

void when_the_scheduler_runs(BDDContext& context) {
    std::cout << "WHEN the scheduling process is run" << std::endl;
    context.scheduler->scheduleTasks();
}

void then_tasks_are_assigned_correctly_and_conflicts_are_identified(BDDContext& context) {
    std::cout << "THEN the high-priority task is assigned to the first agent" << std::endl;
    const auto& tasks = context.scheduler->getTasks();

    // Find the high-priority task in the scheduler's list
    bool highPrioTaskFoundAndCorrect = false;
    for(const auto& task : tasks) {
        if (task.id == context.highPriorityTask.id) {
            NATIVE_ASSERT(task.status == IN_PROGRESS);
            NATIVE_ASSERT(task.assignedAgentId == context.agent1.id);
            highPrioTaskFoundAndCorrect = true;
            break;
        }
    }
    NATIVE_ASSERT(highPrioTaskFoundAndCorrect);

    std::cout << "AND the low-priority task is assigned to the second agent" << std::endl;
    bool lowPrioTaskFoundAndCorrect = false;
    for(const auto& task : tasks) {
        if (task.id == context.lowPriorityTask.id) {
            NATIVE_ASSERT(task.status == IN_PROGRESS);
            NATIVE_ASSERT(task.assignedAgentId == context.agent2.id);
            lowPrioTaskFoundAndCorrect = true;
            break;
        }
    }
    NATIVE_ASSERT(lowPrioTaskFoundAndCorrect);

    std::cout << "AND the dependent task is marked as a conflict" << std::endl;
    bool conflictTaskFoundAndCorrect = false;
    for(const auto& task : tasks) {
        if (task.id == context.conflictedTask.id) {
            NATIVE_ASSERT(task.status == CONFLICT);
            NATIVE_ASSERT(task.assignedAgentId == -1);
            conflictTaskFoundAndCorrect = true;
            break;
        }
    }
    NATIVE_ASSERT(conflictTaskFoundAndCorrect);
}

int main() {
    std::cout << "--- BDD Scenario: Task Scheduling and Conflict Resolution ---" << std::endl;

    BDDContext context;
    given_a_scheduler_with_agents_and_tasks(context);
    when_the_scheduler_runs(context);
    then_tasks_are_assigned_correctly_and_conflicts_are_identified(context);

    std::cout << "\n--- BDD SCENARIO PASSED ---" << std::endl;

    std::cout << "\n--- BDD Scenario: Configuration Loading and Reporting ---" << std::endl;
    Scheduler scheduler;
    given_a_config_file("test_config.json", "test_tasks_bdd.json");
    std::cout << "WHEN the scheduler loads the config" << std::endl;
    scheduler.loadConfig("test_config.json");

    std::cout << "THEN the tasks are loaded from the JSON file" << std::endl;
    NATIVE_ASSERT(scheduler.getTasks().size() == 1);
    NATIVE_ASSERT(scheduler.getTasks()[0].id == 301);

    std::cout << "AND a report can be generated" << std::endl;
    scheduler.generateReport("test_report_bdd.json");
    std::ifstream report_file("test_report_bdd.json");
    NATIVE_ASSERT(report_file.is_open());
    report_file.close();

    std::remove("test_config.json");
    std::remove("test_tasks_bdd.json");
    std::remove("test_report_bdd.json");

    std::cout << "\n--- BDD SCENARIO PASSED ---" << std::endl;

    return 0;
}

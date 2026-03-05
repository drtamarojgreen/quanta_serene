#include <iostream>
#include <fstream>
#include <cstdio> // For remove()

#include "../src/Scheduler.h"

#define NATIVE_ASSERT(condition) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: (" #condition "), function " << __func__ \
                  << ", file " << __FILE__ << ", line " << __LINE__ << "." << std::endl; \
        exit(1); \
    }

void test_json_serialization() {
    std::cout << "Running test_json_serialization..." << std::endl;
    Task task(1, "Test Task", 1, 60, PENDING, {2}, 1700000000);
    JsonValue j = task_to_json(task);

    NATIVE_ASSERT(j["id"].get_int() == 1);
    NATIVE_ASSERT(j["description"].get_string() == "Test Task");
    NATIVE_ASSERT(j["target_start_time"].get_number() == 1700000000);

    Task task2 = task_from_json(j);
    NATIVE_ASSERT(task2.id == 1);
    NATIVE_ASSERT(task2.target_start_time == 1700000000);
    std::cout << "PASSED" << std::endl;
}

void test_file_io() {
    std::cout << "Running test_file_io..." << std::endl;
    Scheduler scheduler;
    Task task(1, "File IO Task", 1, 60, PENDING, {});
    scheduler.addTask(task);

    std::string filename = "test_tasks.json";
    scheduler.saveTasks(filename);

    Scheduler scheduler2;
    scheduler2.loadTasks(filename);

    NATIVE_ASSERT(scheduler2.getTasks().size() == 1);
    NATIVE_ASSERT(scheduler2.getTasks()[0].description == "File IO Task");

    std::remove(filename.c_str());
    std::cout << "PASSED" << std::endl;
}

int main() {
    test_json_serialization();
    test_file_io();
    return 0;
}

#include "../src/Scheduler.h"
#include <iostream>
#include <fstream>
#include <cstdio>

#define NATIVE_ASSERT(condition) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: (" #condition "), function " << __func__ \
                  << ", file " << __FILE__ << ", line " << __LINE__ << "." << std::endl; \
        exit(1); \
    }

void test_script_generation() {
    std::cout << "Running test_script_generation..." << std::endl;
    Scheduler scheduler;
    std::string payload = R"({
        "script_name": "test_script.sh",
        "commands": ["echo 'Hello'", "ls -l"]
    })";

    scheduler.createBashScriptFromJson(payload);

    std::ifstream file("test_script.sh");
    NATIVE_ASSERT(file.is_open());

    std::string line;
    std::getline(file, line);
    NATIVE_ASSERT(line == "#!/bin/bash");
    std::getline(file, line);
    NATIVE_ASSERT(line == "echo 'Hello'");

    file.close();
    std::remove("test_script.sh");
    std::cout << "PASSED" << std::endl;
}

int main() {
    test_script_generation();
    return 0;
}

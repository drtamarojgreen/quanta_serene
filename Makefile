# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Isrc

# Source files and object files
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = QuantaSerene

# Default target
all: $(EXECUTABLE)

# Linking the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) test_runner

# Target for running tests
test:
	$(CXX) $(CXXFLAGS) -o test_runner src/tests.cpp src/Scheduler.cpp src/Json.cpp
	./test_runner
	$(CXX) $(CXXFLAGS) -o test_json_runner tests/test_json_functionality.cpp src/Scheduler.cpp src/Json.cpp
	./test_json_runner
	$(CXX) $(CXXFLAGS) -o test_script_runner tests/test_script_generation.cpp src/Scheduler.cpp src/Json.cpp
	./test_script_runner
	$(CXX) $(CXXFLAGS) -o test_bdd_runner tests/test_bdd.cpp src/Scheduler.cpp src/Json.cpp
	./test_bdd_runner

.PHONY: all clean test

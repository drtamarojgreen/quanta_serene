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
	$(CXX) $(CXXFLAGS) -o test_runner src/tests.cpp src/Scheduler.cpp
	./test_runner

.PHONY: all clean test

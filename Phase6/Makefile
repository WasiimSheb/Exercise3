# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -pthread

# Targets
TARGET_SERVER = server

# Source files
SRC_REACTOR = Reactor.cpp
SRC_SERVER = Server.cpp

# Object files
OBJ_REACTOR = Reactor.o

# Rules
all: $(TARGET_SERVER)

$(OBJ_REACTOR): $(SRC_REACTOR) Reactor.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_REACTOR) -o $(OBJ_REACTOR)

$(TARGET_SERVER): $(SRC_SERVER) $(OBJ_REACTOR)
	$(CXX) $(CXXFLAGS) -o $(TARGET_SERVER) $(SRC_SERVER) $(OBJ_REACTOR)

clean:
	rm -f $(TARGET_SERVER) $(OBJ_REACTOR) gmon.out

run_server: $(TARGET_SERVER)
	./$(TARGET_SERVER)

.PHONY: all clean run_server

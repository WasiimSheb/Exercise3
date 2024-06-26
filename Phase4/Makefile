# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -pg

# Target
TARGET = server

# Source file
SRC = Server.cpp

# Rules
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) gmon.out analysis.txt

run: $(TARGET)
	./$(TARGET)

profile: run
	gprof $(TARGET) gmon.out > analysis.txt

.PHONY: all clean run profile

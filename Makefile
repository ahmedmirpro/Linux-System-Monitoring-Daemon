CXX = g++
CXXFLAGS = -std=c++17 -g -O0 -Wall -Wextra

TARGET = Monitor
SRC = Main.cpp Monitor.cpp
PID ?= 370
INTERVAL ?= 2

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

debug:
	$(CXX) $(CXXFLAGS) -DDEBUG_MODE $(SRC) -o $(TARGET)

r: all
	./$(TARGET) $(PID) $(INTERVAL)

rd: debug
	./$(TARGET) $(PID) $(INTERVAL)

c:clean
clean:
	rm -f $(TARGET)

cl:clear
clear:
	clear

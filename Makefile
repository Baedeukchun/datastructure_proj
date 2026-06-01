CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude

SOURCES := \
	src/main.cpp \
	src/Game.cpp \
	src/Player.cpp \
	src/Room.cpp \
	src/Item.cpp \
	src/Monster.cpp \
	src/Inventory.cpp \
	src/DungeonGraph.cpp \
	src/ScoreTree.cpp \
	src/Sorting.cpp

TEST_SOURCES := \
	tests/smoke_tests.cpp \
	src/Room.cpp \
	src/Item.cpp \
	src/Monster.cpp \
	src/Inventory.cpp \
	src/DungeonGraph.cpp \
	src/ScoreTree.cpp \
	src/Sorting.cpp

TARGET := dungeon_explorer
TEST_TARGET := smoke_tests

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all run test clean

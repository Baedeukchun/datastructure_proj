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

TARGET := dungeon_explorer

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

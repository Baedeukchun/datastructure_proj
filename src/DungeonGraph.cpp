#include "ds/DungeonGraph.h"
#include <iostream>

DungeonGraph::DungeonGraph() : roomCount(0) {}

DungeonGraph::~DungeonGraph() {
    for (int i = 0; i < roomCount; ++i) {
        delete nodes[i].room;
        nodes[i].room = nullptr;
    }
}

bool DungeonGraph::isValidRoomId(int roomId) const {
    return roomId >= 0 && roomId < roomCount && nodes[roomId].room != nullptr;
}

bool DungeonGraph::hasEdge(int fromRoomId, int toRoomId) const {
    const DynamicArray<int>& adj = nodes[fromRoomId].neighbors;
    for (int i = 0; i < adj.size(); ++i) {
        if (adj[i] == toRoomId) return true;
    }
    return false;
}

int DungeonGraph::addRoom(const std::string& name, const std::string& description) {
    if (roomCount >= MAX_ROOMS) return -1;
    int newId = roomCount;
    nodes[newId].room = new Room(newId, name, description);
    ++roomCount;
    return newId;
}

bool DungeonGraph::connectRooms(int fromRoomId, int toRoomId) {
    if (!isValidRoomId(fromRoomId) || !isValidRoomId(toRoomId)) return false;
    if (fromRoomId == toRoomId) return false;
    if (!hasEdge(fromRoomId, toRoomId)) nodes[fromRoomId].neighbors.pushBack(toRoomId);
    if (!hasEdge(toRoomId, fromRoomId)) nodes[toRoomId].neighbors.pushBack(fromRoomId);
    return true;
}

int DungeonGraph::findNeighborByName(int fromRoomId, const std::string& roomName) const {
    if (!isValidRoomId(fromRoomId)) return -1;
    const DynamicArray<int>& adj = nodes[fromRoomId].neighbors;
    for (int i = 0; i < adj.size(); ++i) {
        int nid = adj[i];
        if (nodes[nid].room != nullptr && nodes[nid].room->getName() == roomName) {
            return nid;
        }
    }
    return -1;
}

int DungeonGraph::neighborCount(int roomId) const {
    if (!isValidRoomId(roomId)) return 0;
    return nodes[roomId].neighbors.size();
}

int DungeonGraph::getNeighbor(int roomId, int index) const {
    if (!isValidRoomId(roomId)) return -1;
    const DynamicArray<int>& adj = nodes[roomId].neighbors;
    if (index < 0 || index >= adj.size()) return -1;
    return adj[index];
}

Room* DungeonGraph::getRoom(int roomId) {
    if (!isValidRoomId(roomId)) return nullptr;
    return nodes[roomId].room;
}

const Room* DungeonGraph::getRoom(int roomId) const {
    if (!isValidRoomId(roomId)) return nullptr;
    return nodes[roomId].room;
}

int DungeonGraph::findRoomIdByName(const std::string& roomName) const {
    for (int i = 0; i < roomCount; ++i) {
        if (nodes[i].room != nullptr && nodes[i].room->getName() == roomName) {
            return i;
        }
    }
    return -1;
}

int DungeonGraph::size() const { return roomCount; }

void DungeonGraph::printConnectionsOf(int roomId) const {
    if (!isValidRoomId(roomId)) return;
    const DynamicArray<int>& adj = nodes[roomId].neighbors;
    if (adj.isEmpty()) {
        std::cout << "  (no connections)\n";
        return;
    }
    for (int i = 0; i < adj.size(); ++i) {
        int nid = adj[i];
        if (nodes[nid].room != nullptr) {
            std::cout << "  - " << nodes[nid].room->getName() << "\n";
        }
    }
}

void DungeonGraph::printMap() const {
    std::cout << "=========== Dungeon Map ===========\n";
    for (int i = 0; i < roomCount; ++i) {
        if (nodes[i].room == nullptr) continue;
        std::cout << "[" << nodes[i].room->getName() << "]\n";
        printConnectionsOf(i);
    }
    std::cout << "===================================\n";
}

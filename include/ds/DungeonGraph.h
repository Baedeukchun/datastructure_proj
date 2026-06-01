#ifndef DS_DUNGEON_GRAPH_H
#define DS_DUNGEON_GRAPH_H

#include "Room.h"
#include "ds/DynamicArray.h"
#include <string>

// Undirected graph of rooms.
// Used in: Game.h -> dungeon.
// Why: rooms have variable numbers of neighbors (KENTECH Hall has 5),
//      so direction-based adjacency is too narrow. We store each room
//      together with a DynamicArray of neighbor room IDs (adjacency list).
//      Movement is by room name and must follow an edge in the graph.

class DungeonGraph {
private:
    struct GraphNode {
        Room* room;
        DynamicArray<int> neighbors;
        GraphNode() : room(nullptr), neighbors() {}
    };

    static const int MAX_ROOMS = 32;
    GraphNode nodes[MAX_ROOMS];
    int roomCount;

    bool isValidRoomId(int roomId) const;
    bool hasEdge(int fromRoomId, int toRoomId) const;

    DungeonGraph(const DungeonGraph&) = delete;
    DungeonGraph& operator=(const DungeonGraph&) = delete;

public:
    DungeonGraph();
    ~DungeonGraph();

    int addRoom(const std::string& name, const std::string& description);

    // Adds an undirected edge between the two rooms (idempotent: duplicate
    // edges are ignored).
    bool connectRooms(int fromRoomId, int toRoomId);

    // Returns the room ID that the player would move into when typing
    // `move <roomName>` from `fromRoomId`. Returns -1 if no such neighbor.
    int findNeighborByName(int fromRoomId, const std::string& roomName) const;

    int neighborCount(int roomId) const;
    int getNeighbor(int roomId, int index) const;

    Room* getRoom(int roomId);
    const Room* getRoom(int roomId) const;
    int findRoomIdByName(const std::string& roomName) const;
    int size() const;

    void printMap() const;
    void printConnectionsOf(int roomId) const;
};

#endif

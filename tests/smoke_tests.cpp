#include "Item.h"
#include "ds/DungeonGraph.h"
#include "ds/Inventory.h"
#include "ds/Queue.h"
#include "ds/ScoreTree.h"
#include "ds/Sorting.h"
#include "ds/Stack.h"
#include <cassert>
#include <iostream>

static void testInventory() {
    Inventory inventory;
    assert(inventory.isEmpty());

    inventory.addItem(Item("Map", "Shows the campus.", 10));
    inventory.addItem(Item("Coffee", "Restores one heart.", 7));

    assert(inventory.size() == 2);
    assert(inventory.findItem("Map") != nullptr);
    assert(inventory.findItem("Coffee") != nullptr);
    assert(inventory.removeItem("Map"));
    assert(inventory.findItem("Map") == nullptr);
    assert(!inventory.removeItem("Missing"));
}

static void testStack() {
    Stack<int> stack;
    int value = -1;

    assert(stack.isEmpty());
    assert(!stack.pop(value));

    stack.push(10);
    stack.push(20);
    assert(stack.size() == 2);
    assert(stack.peek(value) && value == 20);
    assert(stack.pop(value) && value == 20);
    assert(stack.pop(value) && value == 10);
    assert(stack.isEmpty());
}

static void testQueue() {
    Queue<int> queue;
    int value = -1;

    assert(queue.isEmpty());
    assert(!queue.dequeue(value));

    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    assert(queue.size() == 3);
    assert(queue.peek(value) && value == 1);
    assert(queue.dequeue(value) && value == 1);
    assert(queue.dequeue(value) && value == 2);
    assert(queue.dequeue(value) && value == 3);
    assert(queue.isEmpty());
}

static void testScoreTreeAndSorting() {
    ScoreTree tree;
    tree.insert(ScoreRecord("Alice", 15));
    tree.insert(ScoreRecord("Bob", 30));
    tree.insert(ScoreRecord("Carol", 20));

    assert(tree.size() == 3);
    assert(tree.containsPlayer("Bob"));
    assert(!tree.containsPlayer("Dana"));

    ScoreRecord records[3];
    int count = tree.copyInto(records, 3);
    assert(count == 3);
    assert(records[0].score <= records[1].score);
    assert(records[1].score <= records[2].score);

    sortScoresDescending(records, count);
    assert(records[0].score == 30);
    assert(records[1].score == 20);
    assert(records[2].score == 15);
}

static void testDungeonGraph() {
    DungeonGraph graph;
    int hall = graph.addRoom("Hall", "Start room");
    int lab = graph.addRoom("Lab", "Test room");
    int cafe = graph.addRoom("Cafe", "Rest room");

    assert(hall == 0);
    assert(lab == 1);
    assert(cafe == 2);
    assert(graph.connectRooms(hall, lab));
    assert(graph.connectRooms(lab, cafe));

    assert(graph.findNeighborByName(hall, "Lab") == lab);
    assert(graph.findNeighborByName(hall, "Cafe") == -1);
    assert(graph.neighborCount(lab) == 2);
    assert(graph.getNeighbor(lab, 0) != -1);
    assert(graph.getRoom(cafe) != nullptr);
}

static void testItemSorting() {
    Item items[3] = {
        Item("A", "low", 5),
        Item("B", "high", 30),
        Item("C", "mid", 10),
    };

    sortItemsByValueDescending(items, 3);
    assert(items[0].getName() == "B");
    assert(items[1].getName() == "C");
    assert(items[2].getName() == "A");
}

int main() {
    testInventory();
    testStack();
    testQueue();
    testScoreTreeAndSorting();
    testDungeonGraph();
    testItemSorting();

    std::cout << "All smoke tests passed.\n";
    return 0;
}

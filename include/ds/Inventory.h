#ifndef DS_INVENTORY_H
#define DS_INVENTORY_H

#include "Item.h"
#include <string>

// Singly Linked List
// Used in: Player inventory (Player.h).
// Why: items are added/removed dynamically, and we never need random access.
//      A singly linked list gives O(1) insertion at the head and clean
//      pointer-based deletion practice.

class Inventory {
private:
    struct Node {
        Item item;
        Node* next;
        Node(const Item& it) : item(it), next(nullptr) {}
    };

    Node* head;
    int count;

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

public:
    Inventory();
    ~Inventory();

    void addItem(const Item& item);
    bool removeItem(const std::string& itemName);
    Item* findItem(const std::string& itemName);
    const Item* findItem(const std::string& itemName) const;
    int countItem(const std::string& itemName) const;
    bool isEmpty() const;
    int size() const;
    void clear();
    void print() const;
};

#endif

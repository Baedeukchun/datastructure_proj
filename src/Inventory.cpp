#include "ds/Inventory.h"
#include <iostream>

Inventory::Inventory() : head(nullptr), count(0) {}

Inventory::~Inventory() { clear(); }

// Insert at the head: O(1).
void Inventory::addItem(const Item& item) {
    Node* node = new Node(item);
    node->next = head;
    head = node;
    ++count;
}

// Search and unlink: O(n).
bool Inventory::removeItem(const std::string& itemName) {
    Node* prev = nullptr;
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->item.getName() == itemName) {
            if (prev == nullptr) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            --count;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

Item* Inventory::findItem(const std::string& itemName) {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->item.getName() == itemName) return &cur->item;
        cur = cur->next;
    }
    return nullptr;
}

const Item* Inventory::findItem(const std::string& itemName) const {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->item.getName() == itemName) return &cur->item;
        cur = cur->next;
    }
    return nullptr;
}

// Count how many items share this name (used for key pieces). O(n).
int Inventory::countItem(const std::string& itemName) const {
    int n = 0;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->item.getName() == itemName) ++n;
    }
    return n;
}

bool Inventory::isEmpty() const { return count == 0; }
int Inventory::size() const { return count; }

void Inventory::clear() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
    count = 0;
}

void Inventory::print() const {
    if (head == nullptr) {
        std::cout << "Inventory is empty.\n";
        return;
    }
    std::cout << "Inventory (" << count << " item(s)):\n";
    Node* cur = head;
    while (cur != nullptr) {
        std::cout << "  - ";
        cur->item.print();
        cur = cur->next;
    }
}

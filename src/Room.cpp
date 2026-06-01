#include "Room.h"
#include <iostream>

Room::Room()
    : id(-1), name(""), description(""), items(), monsters(), visited(false) {}

Room::Room(int id, const std::string& name, const std::string& description)
    : id(id), name(name), description(description),
      items(), monsters(), visited(false) {}

int Room::getId() const { return id; }
std::string Room::getName() const { return name; }
std::string Room::getDescription() const { return description; }

void Room::setVisited(bool value) { visited = value; }
bool Room::hasBeenVisited() const { return visited; }

void Room::addItem(const Item& item) { items.pushBack(item); }

bool Room::takeItem(const std::string& itemName, Item& output) {
    for (int i = 0; i < items.size(); ++i) {
        if (items[i].getName() == itemName) {
            output = items[i];
            items.removeAt(i);
            return true;
        }
    }
    return false;
}

int Room::itemCount() const { return items.size(); }
const Item& Room::getItem(int index) const { return items[index]; }

void Room::addMonster(const Monster& monster) { monsters.pushBack(monster); }
int Room::monsterCount() const { return monsters.size(); }
Monster& Room::getMonster(int index) { return monsters[index]; }
const Monster& Room::getMonster(int index) const { return monsters[index]; }

Monster* Room::getActiveMonster() {
    for (int i = 0; i < monsters.size(); ++i) {
        if (!monsters[i].isDefeated()) return &monsters[i];
    }
    return nullptr;
}

void Room::printDescription() const {
    std::cout << "\nLocation: " << name << "\n";
    std::cout << description << "\n";

    if (items.isEmpty()) {
        std::cout << "Items: none\n";
    } else {
        std::cout << "Items here:\n";
        for (int i = 0; i < items.size(); ++i) {
            std::cout << "  - ";
            items[i].print();
        }
    }

    if (!monsters.isEmpty()) {
        std::cout << "Who's here:\n";
        for (int i = 0; i < monsters.size(); ++i) {
            std::cout << "  - ";
            monsters[i].print();
        }
    }
}

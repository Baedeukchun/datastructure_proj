#ifndef PLAYER_H
#define PLAYER_H

#include "ds/Inventory.h"
#include <string>

class Player {
private:
    std::string name;
    int score;
    int hp;       // current Hearts; reaching 0 ends the game
    int maxHp;    // Hearts cap (starts at 3)
    int currentRoomId;
    Inventory inventory;

public:
    Player(const std::string& name = "Explorer");

    std::string getName() const;
    int getScore() const;
    int getCurrentRoomId() const;

    void setName(const std::string& newName);

    void setCurrentRoomId(int roomId);
    void addScore(int amount);

    // Hearts (HP). Wraith ambushes drain them; Cafe items restore them.
    int getHp() const;
    int getMaxHp() const;
    void loseHeart();          // -1, never below 0
    void heal(int amount);     // +amount, capped at maxHp
    bool isAlive() const;      // hp > 0
    bool spendScore(int amount);  // deduct if affordable; true on success

    Inventory& getInventory();
    const Inventory& getInventory() const;

    void printStatus() const;
};

#endif

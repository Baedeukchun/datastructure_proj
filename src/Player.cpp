#include "Player.h"
#include <iostream>

Player::Player(const std::string& name)
    : name(name), score(10), hp(3), maxHp(3), currentRoomId(0), inventory() {}

std::string Player::getName() const { return name; }
int Player::getScore() const { return score; }
int Player::getCurrentRoomId() const { return currentRoomId; }

void Player::setName(const std::string& newName) { name = newName; }
void Player::setCurrentRoomId(int roomId) { currentRoomId = roomId; }
void Player::addScore(int amount) { score += amount; }

int Player::getHp() const { return hp; }
int Player::getMaxHp() const { return maxHp; }

void Player::loseHeart() {
    if (hp > 0) --hp;
}

void Player::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

bool Player::isAlive() const { return hp > 0; }

bool Player::spendScore(int amount) {
    if (score < amount) return false;
    score -= amount;
    return true;
}

Inventory& Player::getInventory() { return inventory; }
const Inventory& Player::getInventory() const { return inventory; }

void Player::printStatus() const {
    std::cout << "Player: " << name
              << " | Score: " << score
              << " | Hearts: ";
    for (int i = 0; i < hp; ++i) std::cout << "<3 ";
    std::cout << "(" << hp << "/" << maxHp << ")\n";
}

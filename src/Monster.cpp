#include "Monster.h"
#include <iostream>

Monster::Monster()
    : name(""), rewardScore(0), mission(MissionType::None), defeated(false) {}

Monster::Monster(const std::string& name, int rewardScore, MissionType mission)
    : name(name), rewardScore(rewardScore), mission(mission), defeated(false) {}

std::string Monster::getName() const { return name; }
int Monster::getRewardScore() const { return rewardScore; }
MissionType Monster::getMission() const { return mission; }
bool Monster::isDefeated() const { return defeated; }
void Monster::markDefeated() { defeated = true; }

void Monster::print() const {
    std::cout << name;
    if (defeated) std::cout << " [cleared]";
    std::cout << "\n";
}

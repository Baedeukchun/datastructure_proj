#ifndef MONSTER_H
#define MONSTER_H

#include <string>

// The mission tag tells Game.cpp which mini-game to launch when the player
// types `event` in the same room. Keeping the trigger inside Monster (rather
// than in Game) lets each room define its own challenge declaratively.
enum class MissionType {
    None,
    DataStructureQuiz,    // A205 - SeungHyeon Yoon
    BinaryGame,           // IoT Laboratory - Hyunwoo Lee
    EnglishDefinition,    // C404 - George
    FinalQuiz             // Bus Station - EunJeong Kim
};

class Monster {
private:
    std::string name;
    int rewardScore;
    MissionType mission;
    bool defeated;

public:
    Monster();
    Monster(const std::string& name, int rewardScore, MissionType mission);

    std::string getName() const;
    int getRewardScore() const;
    MissionType getMission() const;
    bool isDefeated() const;
    void markDefeated();

    void print() const;
};

#endif

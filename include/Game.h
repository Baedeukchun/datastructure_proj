#ifndef GAME_H
#define GAME_H

#include "GameEvent.h"
#include "Player.h"
#include "ds/DungeonGraph.h"
#include "ds/Queue.h"
#include "ds/ScoreTree.h"
#include "ds/Stack.h"
#include <string>

class Game {
private:
    DungeonGraph dungeon;
    Player player;
    Stack<int> moveHistory;       // previous room IDs (for undo)
    Queue<GameEvent> eventQueue;  // pending events
    ScoreTree scoreTree;          // ranking
    bool running;
    bool gameCleared;             // final mission solved

    // World setup
    void buildKentechWorld();
    void seedEvents();

    // Score persistence (leaderboard survives across runs via a text file).
    void loadScores();
    void saveScores();

    // UI helpers
    void printBanner() const;
    void printHelp() const;
    void look() const;

    // Commands
    void moveByRoomName(const std::string& roomName);
    void moveInteractive();
    void moveToRoom(int destinationId);
    void returnToHall();
    void undoMove();
    void takeItem(const std::string& itemName);
    void processOneEvent();
    void showScores();
    void showSortedScores();
    void showSortedRoomItems() const;
    void showMap();

    // Mission handlers (one per MissionType)
    void runEncounter();
    bool runDataStructureQuiz();
    bool runBinaryGame();
    bool runEnglishDefinition();
    bool runKentechQuiz();

    // Wandering "wraith" monsters: ambush between commands. The player must
    // retype a word/sentence within a time limit or lose a Heart.
    void maybeRandomEncounter();
    void runWraithEncounter();
    bool typeChallenge(const std::string& target);

    // Cafe STAY shop: spend score to buy Heart-restoring items, then `use` them.
    void showShop() const;
    void buyItem(const std::string& name);
    void useItem(const std::string& name);

    // Monster progression: each defeat unlocks the next and drops a key piece.
    std::string missionPrereqItem(MissionType mission) const;
    void dropMissionRewards(MissionType mission);
    void combineKeyPieces();
    void showTimetable();

    // Misc
    void libraryHint() const;
    void studyVocabulary();

    void processCommand(const std::string& line);

public:
    Game();
    void run();

    // Delete the persisted leaderboard file (used by the --reset launch flag).
    static void resetScores();
};

#endif

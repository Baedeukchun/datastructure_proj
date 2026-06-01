#include "Game.h"
#include "MapImage.h"
#include "ds/Sorting.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <ctime>

// Leaderboard file: past players' scores accumulate here across runs.
static const char* SCORES_FILE = "scores.txt";

// -----------------------------------------------------------------------------
// Small helpers
// -----------------------------------------------------------------------------

static std::string trim(const std::string& s) {
    int start = 0;
    int end = (int)s.size();
    while (start < end && std::isspace((unsigned char)s[start])) ++start;
    while (end > start && std::isspace((unsigned char)s[end - 1])) --end;
    return s.substr(start, end - start);
}

// A multiple-choice question: a prompt, exactly three options, one correct index.
struct ChoiceQuestion {
    const char* prompt;
    const char* options[3];
    char answer;  // '1', '2', or '3'
};

// Pick a random index in [0, n). Used to vary which question a monster asks.
static int pickIndex(int n) {
    return std::rand() % n;
}

// Ask one random question drawn from the pool; return true if answered correctly.
static bool askChoice(const ChoiceQuestion* pool, int n) {
    const ChoiceQuestion& q = pool[pickIndex(n)];
    std::cout << q.prompt << "\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "  " << (i + 1) << ") " << q.options[i] << "\n";
    }
    std::cout << "Answer (1-3): ";
    std::string line;
    std::getline(std::cin, line);
    line = trim(line);
    return line.size() == 1 && line[0] == q.answer;
}

// An English-vocabulary question: a word and three candidate meanings.
struct VocabQuestion {
    const char* word;
    const char* options[3];
    char answer;  // '1', '2', or '3'
};

// The word pool George asks at C404. The Vocabulary List item previews these.
static const VocabQuestion VOCAB_POOL[] = {
    {"abandon",  {"to build carefully", "to give up completely", "to celebrate"}, '2'},
    {"ancient",  {"very old", "brand new", "very large"}, '1'},
    {"fragile",  {"very heavy", "brightly colored", "easily broken"}, '3'},
    {"generous", {"often angry", "willing to give freely", "very quiet"}, '2'},
    {"rapid",    {"very fast", "very slow", "very tall"}, '1'},
    {"brave",    {"telling lies", "feeling tired", "showing courage"}, '3'},
};
static const int VOCAB_COUNT = (int)(sizeof(VOCAB_POOL) / sizeof(VOCAB_POOL[0]));

// Ask one random vocabulary question; return true if answered correctly.
static bool askVocab() {
    const VocabQuestion& q = VOCAB_POOL[pickIndex(VOCAB_COUNT)];
    std::cout << "What does the English word '" << q.word << "' mean?\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "  " << (i + 1) << ") " << q.options[i] << "\n";
    }
    std::cout << "Answer (1-3): ";
    std::string line;
    std::getline(std::cin, line);
    line = trim(line);
    return line.size() == 1 && line[0] == q.answer;
}

// Wandering wraiths quiz the player on quick typing. Undergraduate wraiths
// throw a single word; graduate wraiths throw a short sentence.
static const char* WRAITH_WORDS[] = {
    "energy", "binary", "kentech", "dungeon",
    "library", "graduate", "algorithm", "professor",
};
static const int WRAITH_WORD_COUNT =
    (int)(sizeof(WRAITH_WORDS) / sizeof(WRAITH_WORDS[0]));

static const char* WRAITH_SENTENCES[] = {
    "Stay calm and type fast",
    "KENTECH never sleeps",
    "Energy powers the future",
    "Graduate school is endless",
    "Hello from the dungeon",
};
static const int WRAITH_SENTENCE_COUNT =
    (int)(sizeof(WRAITH_SENTENCES) / sizeof(WRAITH_SENTENCES[0]));

// Chance (percent) that a wraith ambushes the player after each command.
static const int AMBUSH_PERCENT = 18;
// Seconds allowed to retype the wraith's word/sentence before it strikes.
static const int TYPE_LIMIT_SEC = 15;

// One Heart is worth this many score: it sets shop prices and the clear bonus
// (leftover Hearts are converted to score when the game is completed).
static const int HEART_SCORE = 7;

// Cafe STAY sells these for score; each restores Hearts when `use`d.
struct ShopGood {
    const char* name;
    const char* desc;
    int price;  // cost in score
    int heal;   // Hearts restored on use
};
static const ShopGood SHOP_GOODS[] = {
    {"Coffee",    "Hot KENTECH blend. Restores 1 Heart.", HEART_SCORE,     1},
    {"Financier", "A buttery cake. Restores 2 Hearts.",   HEART_SCORE * 2, 2},
};
static const int SHOP_GOOD_COUNT =
    (int)(sizeof(SHOP_GOODS) / sizeof(SHOP_GOODS[0]));

// Look up a shop good by name, or nullptr if it is not sold here.
static const ShopGood* findShopGood(const std::string& name) {
    for (int i = 0; i < SHOP_GOOD_COUNT; ++i) {
        if (name == SHOP_GOODS[i].name) return &SHOP_GOODS[i];
    }
    return nullptr;
}

// -----------------------------------------------------------------------------
// Construction & world setup
// -----------------------------------------------------------------------------

Game::Game()
    : dungeon(), player("Explorer"),
      moveHistory(), eventQueue(), scoreTree(),
      running(true), gameCleared(false) {
    std::srand((unsigned)std::time(nullptr));  // vary which quiz question appears
    buildKentechWorld();
    seedEvents();
    loadScores();
}

void Game::buildKentechWorld() {
    // Create the 7 rooms.
    int hall    = dungeon.addRoom("KENTECH Hall",
        "The main hall of KENTECH. Hallways stretch out in every direction.");
    int a205    = dungeon.addRoom("A205",
        "A classroom. The whiteboard is covered with data-structure diagrams.");
    int iot     = dungeon.addRoom("IoT Laboratory",
        "Tangled wires, glowing LEDs, and the smell of soldering iron.");
    int c404    = dungeon.addRoom("C404",
        "An English seminar room. Stacks of dictionaries line the desks.");
    int library = dungeon.addRoom("Library",
        "Quiet shelves of books. A perfect place to gather information.");
    int cafe    = dungeon.addRoom("Cafe STAY",
        "A cozy cafe. The smell of fresh coffee gives you energy.");
    int station = dungeon.addRoom("Bus Station",
        "The KENTECH bus stop. A final figure waits here with a riddle.");

    // Wire up the graph (undirected edges).
    dungeon.connectRooms(hall, a205);
    dungeon.connectRooms(hall, iot);
    dungeon.connectRooms(hall, c404);
    dungeon.connectRooms(hall, library);
    dungeon.connectRooms(hall, cafe);
    dungeon.connectRooms(iot, library);
    dungeon.connectRooms(cafe, station);

    // Items. (Cafe STAY's Coffee/Financier are sold at its shop, not free here.)
    Room* libRoom  = dungeon.getRoom(library);
    if (libRoom != nullptr) {
        libRoom->addItem(Item("VocabularyList",
            "A list of tricky English words. Useful at C404.", 15));
        libRoom->addItem(Item("Map",
            "A hand-drawn map of KENTECH.", 10));
        libRoom->addItem(Item("Timetable",
            "The professors' schedule. Shows the order to challenge them.", 5));
    }

    // Monsters / professors guarding the rooms.
    Room* a205Room    = dungeon.getRoom(a205);
    Room* iotRoom     = dungeon.getRoom(iot);
    Room* c404Room    = dungeon.getRoom(c404);
    Room* stationRoom = dungeon.getRoom(station);
    if (a205Room != nullptr) {
        a205Room->addMonster(Monster("SeungHyeon Yoon",
            30, MissionType::DataStructureQuiz));
    }
    if (iotRoom != nullptr) {
        iotRoom->addMonster(Monster("Hyunwoo Lee",
            30, MissionType::BinaryGame));
    }
    if (c404Room != nullptr) {
        c404Room->addMonster(Monster("George",
            30, MissionType::EnglishDefinition));
    }
    if (stationRoom != nullptr) {
        stationRoom->addMonster(Monster("EunJeong Kim",
            100, MissionType::FinalQuiz));
    }

    // Player starts at KENTECH Hall.
    player.setCurrentRoomId(hall);
    Room* startRoom = dungeon.getRoom(hall);
    if (startRoom != nullptr) startRoom->setVisited(true);
}

void Game::seedEvents() {
    eventQueue.enqueue(GameEvent(
        "A cold wind passes through the KENTECH hallway.", 0));
    eventQueue.enqueue(GameEvent(
        "You notice a note on the wall: 'Try every room.'", 5));
    eventQueue.enqueue(GameEvent(
        "You feel a sudden burst of motivation.", 10));
    eventQueue.enqueue(GameEvent(
        "A loose tile makes you stumble.", 0));
}

// Load past players from the leaderboard file into the BST. Each line is
// "<score> <name>"; the name may contain spaces. Missing file = empty board.
void Game::loadScores() {
    std::ifstream in(SCORES_FILE);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int score;
        if (!(iss >> score)) continue;
        std::string name;
        std::getline(iss, name);
        name = trim(name);
        if (!name.empty()) scoreTree.insert(ScoreRecord(name, score));
    }
}

// Remove the leaderboard file so the next run starts with an empty ranking.
void Game::resetScores() {
    if (std::remove(SCORES_FILE) == 0) {
        std::cout << "Leaderboard reset (" << SCORES_FILE << " removed).\n";
    } else {
        std::cout << "Nothing to reset (" << SCORES_FILE << " does not exist).\n";
    }
}

// Rewrite the whole leaderboard from the BST so the latest play persists.
void Game::saveScores() {
    int n = scoreTree.size();
    ScoreRecord* records = new ScoreRecord[n > 0 ? n : 1];
    int count = scoreTree.copyInto(records, n);

    std::ofstream out(SCORES_FILE);
    if (out) {
        for (int i = 0; i < count; ++i) {
            out << records[i].score << ' ' << records[i].playerName << "\n";
        }
    }
    delete[] records;
}

// -----------------------------------------------------------------------------
// UI
// -----------------------------------------------------------------------------

void Game::printBanner() const {
    std::cout << "====================================\n";
    std::cout << "     KENTECH Dungeon Explorer\n";
    std::cout << "====================================\n";
}

void Game::printHelp() const {
    std::cout << "\nCommands:\n"
              << "  help                  Show this help message\n"
              << "  look                  Describe the current room\n"
              << "  move [room name]      Move to a connected room (type 'move' alone to pick by number)\n"
              << "  return                Jump straight back to KENTECH Hall\n"
              << "  take <item name>      Pick up an item in the current room\n"
              << "  inventory             Show your inventory\n"
              << "  undo                  Step back to the previous room (stack)\n"
              << "  event                 Process one queued event (queue)\n"
              << "  scores                Show the score ranking (BST in-order)\n"
              << "  sortscores            Show scores via manual sorting\n"
              << "  sortitems             Show this room's items, sorted by value\n"
              << "  vocab                 Read your Vocabulary List (needs the item)\n"
              << "  timetable             Show the order to defeat professors (needs the item)\n"
              << "  combine               Combine 3 key pieces into the escape Key\n"
              << "  shop                  Show the Cafe STAY menu (at Cafe STAY)\n"
              << "  buy <item name>       Buy a Heart-restoring item (at Cafe STAY)\n"
              << "  use <item name>       Use an item to restore Hearts\n"
              << "  map                   Show the dungeon graph + image (needs Map item)\n"
              << "  status                Show player status\n"
              << "  quit                  Exit the game\n";
}

void Game::look() const {
    const Room* room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr) {
        std::cout << "Current location is invalid.\n";
        return;
    }

    room->printDescription();

    std::cout << "Connected Rooms:\n";
    dungeon.printConnectionsOf(room->getId());

    std::cout << "\nCommands: help, look, move [room], take [item], "
                 "inventory, undo, event, scores, map, quit\n";
}

// -----------------------------------------------------------------------------
// Commands
// -----------------------------------------------------------------------------

// Typing `move <room name>` jumps straight to a connected room by name.
void Game::moveByRoomName(const std::string& roomName) {
    if (roomName.empty()) {
        // Bare `move`: show the choices and let the player pick by number.
        moveInteractive();
        return;
    }

    int current = player.getCurrentRoomId();
    int next = dungeon.findNeighborByName(current, roomName);
    if (next == -1) {
        std::cout << "You cannot reach '" << roomName
                  << "' directly from here. (Type 'map' to see connections.)\n";
        return;
    }

    moveToRoom(next);
}

// Bare `move`: list the rooms reachable from here and move to the chosen number.
void Game::moveInteractive() {
    int current = player.getCurrentRoomId();
    int n = dungeon.neighborCount(current);
    if (n == 0) {
        std::cout << "There is nowhere to move from here.\n";
        return;
    }

    std::cout << "Where do you want to go?\n";
    for (int i = 0; i < n; ++i) {
        const Room* r = dungeon.getRoom(dungeon.getNeighbor(current, i));
        if (r != nullptr) {
            std::string label = r->getName();
            if (r->getName() == "Bus Station"
                && player.getInventory().findItem("Key") == nullptr) {
                label += " (locked - needs Key)";
            }
            std::cout << "  " << (i + 1) << ") " << label << "\n";
        }
    }
    std::cout << "Select a number (or blank to cancel): ";

    std::string line;
    std::getline(std::cin, line);
    line = trim(line);
    if (line.empty()) {
        std::cout << "Move cancelled.\n";
        return;
    }

    int choice = 0;
    std::istringstream iss(line);
    if (!(iss >> choice) || choice < 1 || choice > n) {
        std::cout << "Invalid selection.\n";
        return;
    }

    moveToRoom(dungeon.getNeighbor(current, choice - 1));
}

// Shared movement: record history for undo, arrive, score, and describe.
void Game::moveToRoom(int destinationId) {
    // The Bus Station gate stays locked until the player forges the Key.
    const Room* dest = dungeon.getRoom(destinationId);
    if (dest != nullptr && dest->getName() == "Bus Station"
        && player.getInventory().findItem("Key") == nullptr) {
        std::cout << "The gate to the Bus Station is locked. "
                     "You need the Key to open it.\n";
        return;
    }

    moveHistory.push(player.getCurrentRoomId());
    player.setCurrentRoomId(destinationId);

    Room* arrived = dungeon.getRoom(destinationId);
    if (arrived != nullptr) arrived->setVisited(true);

    // Room-specific arrival behavior.
    if (arrived != nullptr && arrived->getName() == "Library") {
        libraryHint();
    }

    look();
}

// Jump straight back to KENTECH Hall (the central hub) from anywhere.
void Game::returnToHall() {
    int hallId = dungeon.findRoomIdByName("KENTECH Hall");
    if (hallId == -1) {
        std::cout << "KENTECH Hall is not on the map.\n";
        return;
    }
    if (player.getCurrentRoomId() == hallId) {
        std::cout << "You are already at KENTECH Hall.\n";
        return;
    }
    moveToRoom(hallId);
}

void Game::undoMove() {
    int previousRoom = -1;
    if (!moveHistory.pop(previousRoom)) {
        std::cout << "No previous move to undo.\n";
        return;
    }
    player.setCurrentRoomId(previousRoom);
    std::cout << "You step back to the previous room.\n";
    look();
}

void Game::takeItem(const std::string& itemName) {
    if (itemName.empty()) {
        std::cout << "Take what? Example: take Coffee\n";
        return;
    }

    Room* room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr) {
        std::cout << "Current room is invalid.\n";
        return;
    }

    Item item;
    if (!room->takeItem(itemName, item)) {
        std::cout << "There is no item named '" << itemName << "' here.\n";
        return;
    }

    player.getInventory().addItem(item);
    player.addScore(item.getValue());
    std::cout << "You picked up: " << item.getName() << "\n";
}

void Game::processOneEvent() {
    GameEvent event;
    if (!eventQueue.dequeue(event)) {
        std::cout << "No queued events.\n";
        return;
    }
    std::cout << "Event: " << event.description << "\n";
    player.printStatus();
}

void Game::showScores() {
    std::cout << "Score records (BST in-order, high -> low):\n";
    scoreTree.printDescending();
}

// Pull every record out of the BST, append the live (in-progress) score, then
// run our manual selection sort. Demonstrates the Sorting algorithm separately
// from the BST's own ordering.
void Game::showSortedScores() {
    int n = scoreTree.size();
    ScoreRecord* records = new ScoreRecord[n + 1];

    int count = scoreTree.copyInto(records, n);
    records[count++] = ScoreRecord(player.getName(), player.getScore());

    sortScoresDescending(records, count);

    std::cout << "Sorted scores (manual selection sort):\n";
    for (int i = 0; i < count; ++i) {
        std::cout << "  " << (i + 1) << ". "
                  << records[i].playerName << " - "
                  << records[i].score << "\n";
    }

    delete[] records;
}

void Game::showSortedRoomItems() const {
    const Room* room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr) {
        std::cout << "Current room is invalid.\n";
        return;
    }

    int count = room->itemCount();
    if (count == 0) {
        std::cout << "There are no items to sort here.\n";
        return;
    }

    Item* copy = new Item[count];
    for (int i = 0; i < count; ++i) copy[i] = room->getItem(i);

    sortItemsByValueDescending(copy, count);

    std::cout << "Items sorted by value (descending):\n";
    for (int i = 0; i < count; ++i) {
        std::cout << "  - ";
        copy[i].print();
    }

    delete[] copy;
}

// The map is only available once the player has picked up the Map item in the
// Library. We print the graph adjacency (the data structure) and also pop up a
// drawn image of the dungeon.
void Game::showMap() {
    if (player.getInventory().findItem("Map") == nullptr) {
        std::cout << "You don't have a Map yet. Find it in the Library and 'take Map'.\n";
        return;
    }
    dungeon.printMap();
    // Render the drawn map inline. The Sixel bytes are compiled into the program
    // (see MapImage.h), so nothing is read from disk at run time. Terminals
    // without Sixel support will show this as stray characters.
    std::cout << "(Map image below -- requires a Sixel-capable terminal.)\n";
    std::cout << MAP_SIXEL << std::endl;
}

// -----------------------------------------------------------------------------
// Encounters / missions
// -----------------------------------------------------------------------------

void Game::runEncounter() {
    Room* room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr) return;
    Monster* monster = room->getActiveMonster();
    if (monster == nullptr) {
        std::cout << "There is no one to challenge here right now.\n";
        return;
    }

    // Enforce the defeat order: you need the item the previous monster dropped.
    std::string need = missionPrereqItem(monster->getMission());
    if (!need.empty() && player.getInventory().findItem(need) == nullptr) {
        std::cout << monster->getName() << " won't face you yet.\n"
                  << "You need '" << need << "' first. "
                     "(Type 'timetable' to see the order.)\n";
        return;
    }

    std::cout << "\n--- " << monster->getName() << " challenges you! ---\n";
    bool win = false;
    switch (monster->getMission()) {
        case MissionType::DataStructureQuiz: win = runDataStructureQuiz(); break;
        case MissionType::BinaryGame:        win = runBinaryGame(); break;
        case MissionType::EnglishDefinition: win = runEnglishDefinition(); break;
        case MissionType::FinalQuiz:         win = runKentechQuiz(); break;
        default:
            std::cout << "(No mission attached.)\n";
            return;
    }

    if (win) {
        monster->markDefeated();
        int reward = monster->getRewardScore();
        player.addScore(reward);
        if (monster->getMission() != MissionType::FinalQuiz) {
            std::cout << "You cleared the challenge! +" << reward << " score.\n";
        }
        dropMissionRewards(monster->getMission());
        if (monster->getMission() == MissionType::FinalQuiz) {
            gameCleared = true;
            std::cout << "+" << reward << " score.\n";
            std::cout << "\n*** You completed the final mission. ***\n";
            // Convert any Hearts left standing into score.
            int hpBonus = player.getHp() * HEART_SCORE;
            player.addScore(hpBonus);
            std::cout << "Hearts bonus: " << player.getHp() << " x " << HEART_SCORE
                      << " = +" << hpBonus << " score.\n";
        }
    } else {
        player.loseHeart();
        if (monster->getMission() != MissionType::FinalQuiz) {
            std::cout << "You lost the challenge. -1 Heart.\n";
        } else {
            std::cout << "-1 Heart.\n";
        }
        if (!player.isAlive()) {
            std::cout << "You have run out of Hearts...\n";
        }
    }
    player.printStatus();
}

bool Game::runDataStructureQuiz() {
    static const ChoiceQuestion pool[] = {
        {"Which data structure uses LIFO (Last In, First Out) order?",
         {"Queue", "Stack", "Binary Search Tree"}, '2'},
        {"Which data structure processes items in FIFO (First In, First Out) order?",
         {"Stack", "Queue", "Graph"}, '2'},
        {"An in-order traversal of a binary search tree visits the keys in what order?",
         {"Sorted ascending order", "Random order", "Reverse insertion order"}, '1'},
        {"Inserting a new node at the head of a singly linked list takes how long?",
         {"O(n)", "O(log n)", "O(1)"}, '3'},
        {"Which structure best represents rooms that each connect to many neighbors?",
         {"Graph", "Stack", "Queue"}, '1'},
    };
    return askChoice(pool, (int)(sizeof(pool) / sizeof(pool[0])));
}

bool Game::runBinaryGame() {
    struct BinPair { int dec; const char* bin; };
    static const BinPair pool[] = {
        {10, "1010"}, {7, "111"}, {12, "1100"},
        {5, "101"}, {15, "1111"}, {9, "1001"},
    };
    const BinPair& p = pool[pickIndex((int)(sizeof(pool) / sizeof(pool[0])))];
    std::cout << "Binary game: convert decimal " << p.dec << " to binary.\n"
              << "Answer: ";
    std::string line;
    std::getline(std::cin, line);
    return trim(line) == p.bin;
}

bool Game::runEnglishDefinition() {
    return askVocab();
}

bool Game::runKentechQuiz() {
    const int answer = pickIndex(50) + 1;
    std::cout << "Final mission: 1부터 50까지 숫자 중 하나를 맞혀 보세요!\n"
              << "기회는 총 5번입니다.\n";

    for (int attempt = 1; attempt <= 5; ++attempt) {
        std::cout << "Guess " << attempt << "/5: ";
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);

        int guess = 0;
        std::istringstream iss(line);
        if (!(iss >> guess) || guess < 1 || guess > 50) {
            std::cout << "1부터 50까지의 숫자를 입력해주세요.\n";
            --attempt;
            continue;
        }

        if (guess == answer) {
            std::cout << "안전하게 좌석 02에 탑승했습니다! 축하해요~ Home sweet Home ╰(*°▽°*)╯\n";
            return true;
        }
        if (guess < answer) {
            std::cout << "떙@@!@!!! 낮습니다! 좀 더 높여보세요~\n";
        } else {
            std::cout << "떙! 높습니다!! 낮춰주세요 ~~\n";
        }
    }

    std::cout << "좌석 02가 떠나갔습니다.. 다시 도전하세요 ㅠㅡㅠ\n";
    return false;
}

// Show the target text and read one line back, timing how long it took.
// Success requires an exact match typed within TYPE_LIMIT_SEC seconds.
// Note: the timer is checked when Enter is pressed (no mid-typing cutoff),
// which keeps this portable to any terminal.
bool Game::typeChallenge(const std::string& target) {
    std::cout << "Type this within " << TYPE_LIMIT_SEC << " seconds:\n"
              << "   " << target << "\n> ";
    time_t start = std::time(nullptr);
    std::string line;
    std::getline(std::cin, line);
    long elapsed = (long)(std::time(nullptr) - start);

    if (trim(line) != target) {
        std::cout << "Mistyped! The wraith lands a hit.\n";
        return false;
    }
    if (elapsed > TYPE_LIMIT_SEC) {
        std::cout << "Too slow (" << elapsed << "s)! The wraith lands a hit.\n";
        return false;
    }
    return true;
}

// A wandering wraith ambushes the player. Half the time it is an undergraduate
// (one word), half a graduate (a short sentence). Failure costs a Heart.
void Game::runWraithEncounter() {
    bool grad = pickIndex(2) == 0;
    std::string name = grad ? "Wraith of a KENTECH Graduate"
                            : "Wraith of a KENTECH Undergraduate";
    std::string target = grad
        ? WRAITH_SENTENCES[pickIndex(WRAITH_SENTENCE_COUNT)]
        : WRAITH_WORDS[pickIndex(WRAITH_WORD_COUNT)];

    std::cout << "\n!!! A " << name << " suddenly appears! !!!\n";

    if (typeChallenge(target)) {
        std::cout << "You typed it in time and banished the " << name << "!\n";
    } else {
        player.loseHeart();
        std::cout << "-1 Heart.\n";
        player.printStatus();
        if (!player.isAlive()) {
            std::cout << "You have run out of Hearts...\n";
        }
    }
}

// After each command there is a chance a wraith jumps out (idle or moving).
void Game::maybeRandomEncounter() {
    if (!running || gameCleared || !player.isAlive()) return;
    if (pickIndex(100) < AMBUSH_PERCENT) {
        runWraithEncounter();
    }
}

// The item a player must hold before this monster will accept a challenge.
// Empty string = no prerequisite (George is the first in the order).
std::string Game::missionPrereqItem(MissionType mission) const {
    switch (mission) {
        case MissionType::BinaryGame:        return "HyunwooPass";
        case MissionType::DataStructureQuiz: return "SeungHyeonPass";
        case MissionType::FinalQuiz:         return "Key";
        default:                             return "";
    }
}

// On defeat, each professor drops a key piece plus the pass for the next one.
void Game::dropMissionRewards(MissionType mission) {
    Inventory& inv = player.getInventory();
    switch (mission) {
        case MissionType::EnglishDefinition:  // George (1st)
            inv.addItem(Item("KeyPiece", "A fragment of the escape key.", 0));
            inv.addItem(Item("HyunwooPass",
                "Lets you challenge Hyunwoo Lee at the IoT Laboratory.", 0));
            std::cout << "You obtained: KeyPiece, HyunwooPass.\n";
            break;
        case MissionType::BinaryGame:         // Hyunwoo Lee (2nd)
            inv.addItem(Item("KeyPiece", "A fragment of the escape key.", 0));
            inv.addItem(Item("SeungHyeonPass",
                "Lets you challenge SeungHyeon Yoon at A205.", 0));
            std::cout << "You obtained: KeyPiece, SeungHyeonPass.\n";
            break;
        case MissionType::DataStructureQuiz:  // SeungHyeon Yoon (3rd)
            inv.addItem(Item("KeyPiece", "A fragment of the escape key.", 0));
            std::cout << "You obtained: KeyPiece. "
                         "(Use 'combine' once you have 3 pieces.)\n";
            break;
        default:
            break;  // Final quiz drops nothing.
    }
}

// Merge three key pieces into a single Key (needed for the final mission).
void Game::combineKeyPieces() {
    Inventory& inv = player.getInventory();
    if (inv.findItem("Key") != nullptr) {
        std::cout << "You already hold the Key.\n";
        return;
    }
    int pieces = inv.countItem("KeyPiece");
    if (pieces < 3) {
        std::cout << "You have " << pieces << "/3 key pieces. "
                     "Defeat more professors first.\n";
        return;
    }
    inv.removeItem("KeyPiece");
    inv.removeItem("KeyPiece");
    inv.removeItem("KeyPiece");
    inv.addItem(Item("Key",
        "The complete escape key. Use it against EunJeong Kim.", 0));
    std::cout << "You combined 3 key pieces into a Key!\n";
}

// Show the order in which the professors must be defeated (needs the item).
void Game::showTimetable() {
    if (player.getInventory().findItem("Timetable") == nullptr) {
        std::cout << "You don't have a Timetable. Find it in the Library "
                     "and 'take Timetable'.\n";
        return;
    }
    std::cout << "=========== Timetable ===========\n";
    std::cout << "Defeat the professors in this order:\n";
    std::cout << "  1. George          (C404)\n";
    std::cout << "  2. Hyunwoo Lee     (IoT Laboratory)\n";
    std::cout << "  3. SeungHyeon Yoon (A205)\n";
    std::cout << "Then 'combine' the key pieces and face EunJeong Kim (Bus Station).\n";
    std::cout << "=================================\n";
}

// -----------------------------------------------------------------------------
// Cafe STAY shop
// -----------------------------------------------------------------------------

// List what Cafe STAY sells. Buying is only allowed while standing in the cafe.
void Game::showShop() const {
    const Room* room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr || room->getName() != "Cafe STAY") {
        std::cout << "The shop is at Cafe STAY. Come back when you're there.\n";
        return;
    }
    std::cout << "=========== Cafe STAY Menu ===========\n";
    for (int i = 0; i < SHOP_GOOD_COUNT; ++i) {
        const ShopGood& g = SHOP_GOODS[i];
        std::cout << "  " << g.name << " - " << g.price << " score : "
                  << g.desc << "\n";
    }
    std::cout << "Your score: " << player.getScore() << "\n";
    std::cout << "Use 'buy <item>' to purchase, then 'use <item>' to recover.\n";
    std::cout << "======================================\n";
}

// Spend score to add a Cafe good to the inventory (only while at Cafe STAY).
void Game::buyItem(const std::string& name) {
    const Room* room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr || room->getName() != "Cafe STAY") {
        std::cout << "You can only buy things at Cafe STAY.\n";
        return;
    }
    if (name.empty()) {
        std::cout << "Buy what? Example: buy Coffee\n";
        return;
    }
    const ShopGood* good = findShopGood(name);
    if (good == nullptr) {
        std::cout << "Cafe STAY doesn't sell '" << name << "'. Type 'shop'.\n";
        return;
    }
    if (!player.spendScore(good->price)) {
        std::cout << "Not enough score. '" << good->name << "' costs "
                  << good->price << ", you have " << player.getScore() << ".\n";
        return;
    }
    player.getInventory().addItem(Item(good->name, good->desc, 0));
    std::cout << "You bought a " << good->name << " (-" << good->price
              << " score). Score: " << player.getScore() << ".\n";
}

// Consume a Heart-restoring item from the inventory.
void Game::useItem(const std::string& name) {
    if (name.empty()) {
        std::cout << "Use what? Example: use Coffee\n";
        return;
    }
    const ShopGood* good = findShopGood(name);
    if (good == nullptr) {
        std::cout << "You can't use '" << name << "'.\n";
        return;
    }
    if (player.getInventory().findItem(name) == nullptr) {
        std::cout << "You don't have a " << name << ". Buy one at Cafe STAY.\n";
        return;
    }
    if (player.getHp() >= player.getMaxHp()) {
        std::cout << "Your Hearts are already full.\n";
        return;
    }
    player.getInventory().removeItem(name);
    player.heal(good->heal);
    std::cout << "You used a " << name << " and recovered Hearts.\n";
    player.printStatus();
}

// -----------------------------------------------------------------------------
// Room-specific perks
// -----------------------------------------------------------------------------

void Game::libraryHint() const {
    std::cout << "(Library) The librarian whispers:\n"
              << "  - A205: a quiz on data structures (LIFO matters).\n"
              << "  - IoT Lab: a binary number puzzle.\n"
              << "  - C404: an English vocabulary check.\n"
              << "  - Bus Station: a quiz about KENTECH itself.\n";
}

// Preview the C404 vocabulary, but only if the player carries the Vocabulary
// List item picked up in the Library.
void Game::studyVocabulary() {
    if (player.getInventory().findItem("VocabularyList") == nullptr) {
        std::cout << "You don't have a Vocabulary List yet. "
                     "Find it in the Library and 'take VocabularyList'.\n";
        return;
    }
    std::cout << "=========== Vocabulary List ===========\n";
    std::cout << "Study these words -- George may ask them at C404:\n";
    for (int i = 0; i < VOCAB_COUNT; ++i) {
        const VocabQuestion& q = VOCAB_POOL[i];
        std::cout << "  - " << q.word << " : "
                  << q.options[q.answer - '1'] << "\n";
    }
    std::cout << "=======================================\n";
}

// -----------------------------------------------------------------------------
// Command dispatch & main loop
// -----------------------------------------------------------------------------

void Game::processCommand(const std::string& line) {
    std::istringstream input(line);
    std::string command;
    input >> command;

    if (command.empty()) return;

    if (command == "help") {
        printHelp();
    } else if (command == "look") {
        look();
    } else if (command == "move") {
        // Everything after "move " is the room name (rooms can have spaces).
        std::string rest;
        std::getline(input, rest);
        moveByRoomName(trim(rest));
    } else if (command == "take") {
        std::string itemName;
        input >> itemName;
        takeItem(itemName);
    } else if (command == "inventory") {
        player.getInventory().print();
    } else if (command == "return") {
        returnToHall();
    } else if (command == "undo") {
        undoMove();
    } else if (command == "event") {
        // First, see if this room has an active monster encounter.
        Room* room = dungeon.getRoom(player.getCurrentRoomId());
        if (room != nullptr && room->getActiveMonster() != nullptr) {
            runEncounter();
        } else {
            processOneEvent();
        }
    } else if (command == "scores") {
        showScores();
    } else if (command == "sortscores") {
        showSortedScores();
    } else if (command == "sortitems") {
        showSortedRoomItems();
    } else if (command == "vocab") {
        studyVocabulary();
    } else if (command == "timetable") {
        showTimetable();
    } else if (command == "combine") {
        combineKeyPieces();
    } else if (command == "shop") {
        showShop();
    } else if (command == "buy") {
        std::string rest;
        std::getline(input, rest);
        buyItem(trim(rest));
    } else if (command == "use") {
        std::string rest;
        std::getline(input, rest);
        useItem(trim(rest));
    } else if (command == "map") {
        showMap();
    } else if (command == "status") {
        player.printStatus();
    } else if (command == "quit") {
        running = false;
    } else {
        std::cout << "Unknown command. Type 'help' for options.\n";
    }
}

void Game::run() {
    printBanner();

    // Ask who is playing so their score joins the leaderboard.
    std::cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    name = trim(name);
    if (!name.empty()) player.setName(name);

    std::cout << "Welcome, " << player.getName() << "!\n";

    // Show the standings from everyone who has played so far.
    if (!scoreTree.isEmpty()) {
        std::cout << "\n--- Leaderboard so far ---\n";
        scoreTree.printDescending();
    }

    std::cout << "\nType 'help' for commands.\n";
    look();

    while (running && !gameCleared) {
        std::cout << "\n> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        processCommand(line);

        // A wraith may ambush between commands; losing all Hearts ends the run.
        maybeRandomEncounter();
        if (!player.isAlive()) {
            std::cout << "\n*** GAME OVER -- you ran out of Hearts. ***\n";
            running = false;
        }
    }

    // Record the final score and persist the leaderboard for future runs.
    scoreTree.insert(ScoreRecord(player.getName(), player.getScore()));
    saveScores();

    std::cout << "\n--- Final status ---\n";
    player.printStatus();
    std::cout << "\n--- Final ranking ---\n";
    scoreTree.printDescending();

    if (gameCleared) {
        std::cout << "\nThanks for playing KENTECH Dungeon Explorer!\n";
    } else if (!player.isAlive()) {
        std::cout << "\nThe dungeon claims another explorer. Better luck next time.\n";
    } else {
        std::cout << "\nGoodbye.\n";
    }
}

# KENTECH Dungeon Explorer

A text-based dungeon exploration game built in C++17 as a guided Data
Structures PBL project. The player starts at KENTECH Hall, moves between
campus locations, collects items, faces missions from professors, and ends
the game by clearing the final mission at the Bus Station.

## Build & Run

From this directory:

```bash
make                      # build the executable
./dungeon_explorer        # play (asks your name, shows the leaderboard)
./dungeon_explorer --reset # clear the saved leaderboard, then exit
# or
make run                  # build + run in one step
make clean                # remove the binary
```

Requires `g++` with C++17 support.

Scores accumulate across runs in `scores.txt` (created in this directory on
first play). Use `--reset` to wipe it.

### In VS Code

1. Open this folder in VS Code.
2. Open the integrated terminal (`` Ctrl+` `` / `` Cmd+` ``).
3. Run `make` to build, then `./dungeon_explorer` to play.

## Commands

| Command              | Effect |
|----------------------|--------|
| `help`               | Show command list |
| `look`               | Describe the current room |
| `move <room name>`   | Travel to a connected room (e.g. `move Library`) |
| `take <item name>`   | Pick up an item in the current room |
| `inventory`          | Show your inventory (linked list) |
| `undo`               | Step back to the previous room (stack) |
| `event`              | Either run the room's mission, or pop the next event (queue) |
| `scores`             | Show the score ranking (BST reverse in-order) |
| `sortscores`         | Show scores via manual sorting |
| `sortitems`          | Show this room's items sorted by value |
| `map`                | Print the dungeon graph |
| `status`             | Show HP and score |
| `quit`               | Exit |

## Folder Layout

```
include/
  ds/          data structures (DynamicArray, Inventory, Stack, Queue,
               ScoreTree, DungeonGraph, Sorting)
  Item.h Monster.h Room.h Player.h GameEvent.h Game.h
src/
  *.cpp        implementation files
Makefile       build script
```

## Data-Structure Mapping

| Data Structure        | Where it's used                              |
|-----------------------|----------------------------------------------|
| Dynamic Array         | `Room` stores items and monsters             |
| Singly Linked List    | `Inventory` (player items)                   |
| Stack                 | `Game::moveHistory` for `undo`               |
| Queue                 | `Game::eventQueue` for atmosphere events     |
| Binary Search Tree    | `Game::scoreTree` (ranking)                  |
| Graph                 | `DungeonGraph` (room connectivity)           |
| Sorting (manual)      | `sortitems`, `sortscores` commands           |

## Rooms

- **KENTECH Hall** (start) — central hub
- **A205** — SeungHyeon Yoon, data-structure quiz
- **IoT Laboratory** — Hyunwoo Lee, binary number game
- **C404** — George, English-definition quiz
- **Library** — info hints; items: `VocabularyList`, `Map`
- **Cafe STAY** — restores HP; items: `Coffee`, `Financier`
- **Bus Station** — EunJeong Kim, final mission (type `RC`)

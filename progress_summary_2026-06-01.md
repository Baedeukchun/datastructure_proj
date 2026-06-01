# Progress Summary

Date: 2026-06-01

## Current state

The project is no longer at starter-code stage. The core game in `include/` and `src/` has been substantially customized into a KENTECH-themed dungeon game, and the required data structures are integrated into visible gameplay features.

## What appears complete

- Dynamic array: used by `Room`/graph-related storage.
- Singly linked list: implemented inventory in `include/ds/Inventory.h` and `src/Inventory.cpp`.
- Stack: used for `undo` move history.
- Queue: used for room/event processing.
- Binary search tree: used for score ranking and persisted leaderboard.
- Graph: used for room connectivity and map adjacency display.
- Sorting: exposed through in-game score/item sorting commands.

## Evidence from current code

- `src/Game.cpp` contains a complete world setup, command loop, room movement, encounters, shop/items, map display, and score persistence.
- `src/ScoreTree.cpp` includes BST insertion, descending ranking output, and array copy-out for save/sort flows.
- `src/DungeonGraph.cpp` implements undirected room connections, neighbor lookup by room name, and adjacency printing.
- `src/Inventory.cpp` implements add, remove, find, count, clear, and print for the linked-list inventory.
- `README.md` documents playable commands and maps each required data structure to a game feature.
- `implementation.md` shows that multiple feature extensions were already added beyond the starter requirements.

## Notable extensions beyond the base assignment

- KENTECH campus story/theme and professor-based missions.
- Randomized question pools for encounters.
- Vocabulary-study feature tied to an item.
- Interactive `move` selection by room number.
- Persistent leaderboard stored in `scores.txt`.
- Map item gating and image-based map support through `MapImage.h`.
- Additional commands such as `return`, `vocab`, `timetable`, `combine`, `shop`, `buy`, and `use`.

## Gaps or risks I can already see

- The local environment currently lacks `make`, so I could not recompile from source in this shell.
- The assignment rubric expects smoke tests to be attempted; your current top-level project does not appear to include a `test` target like the starter project.
- Final report and contribution/disclosure artifacts are still separate deliverables; code completeness alone will not finish the assignment.
- `implementation.md` is a useful work log, but it is not the same thing as the final report the rubric asks for.

## Bottom line

Your implementation looks close to a playable, feature-complete submission on the code side. The main remaining risk is not the core data structures, but build/test packaging and the non-code submission items required by the rubric.

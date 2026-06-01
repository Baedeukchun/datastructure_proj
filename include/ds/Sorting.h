#ifndef DS_SORTING_H
#define DS_SORTING_H

#include "Item.h"
#include "ds/ScoreTree.h"

// Hand-written sorting algorithms (no std::sort).
// Used in: Game.cpp -> showSortedRoomItems(), showSortedScores().
// Why: the project requires manual implementation. We use selection sort
//      because it is simple to read and easy for students to explain.
// Complexity: O(n^2) for both, which is fine for short item/score lists.

void sortItemsByValueDescending(Item* items, int count);
void sortScoresDescending(ScoreRecord* records, int count);

#endif

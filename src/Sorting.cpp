#include "ds/Sorting.h"

// Selection sort, descending by Item::getValue().
void sortItemsByValueDescending(Item* items, int count) {
    for (int i = 0; i < count - 1; ++i) {
        int bestIndex = i;
        for (int j = i + 1; j < count; ++j) {
            if (items[j].getValue() > items[bestIndex].getValue()) {
                bestIndex = j;
            }
        }
        if (bestIndex != i) {
            Item tmp = items[i];
            items[i] = items[bestIndex];
            items[bestIndex] = tmp;
        }
    }
}

// Selection sort, descending by ScoreRecord::score.
void sortScoresDescending(ScoreRecord* records, int count) {
    for (int i = 0; i < count - 1; ++i) {
        int bestIndex = i;
        for (int j = i + 1; j < count; ++j) {
            if (records[j].score > records[bestIndex].score) {
                bestIndex = j;
            }
        }
        if (bestIndex != i) {
            ScoreRecord tmp = records[i];
            records[i] = records[bestIndex];
            records[bestIndex] = tmp;
        }
    }
}

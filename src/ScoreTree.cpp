#include "ds/ScoreTree.h"
#include <iostream>

ScoreTree::ScoreTree() : root(nullptr), count(0) {}

ScoreTree::~ScoreTree() { clear(); }

// Lower scores go left, higher-or-equal scores go right.
ScoreTree::Node* ScoreTree::insertRecursive(Node* current, const ScoreRecord& record) {
    if (current == nullptr) {
        ++count;
        return new Node(record);
    }
    if (record.score < current->record.score) {
        current->left = insertRecursive(current->left, record);
    } else {
        current->right = insertRecursive(current->right, record);
    }
    return current;
}

void ScoreTree::insert(const ScoreRecord& record) {
    root = insertRecursive(root, record);
}

// Names are not the BST key (score is), so we must check both subtrees.
bool ScoreTree::containsRecursive(Node* current, const std::string& name) const {
    if (current == nullptr) return false;
    if (current->record.playerName == name) return true;
    return containsRecursive(current->left, name)
        || containsRecursive(current->right, name);
}

bool ScoreTree::containsPlayer(const std::string& playerName) const {
    return containsRecursive(root, playerName);
}

// Reverse in-order: right, current, left  -> descending scores.
void ScoreTree::printDescendingRecursive(Node* current, int& rank) const {
    if (current == nullptr) return;
    printDescendingRecursive(current->right, rank);
    std::cout << "  " << rank++ << ". "
              << current->record.playerName
              << " - " << current->record.score << "\n";
    printDescendingRecursive(current->left, rank);
}

void ScoreTree::printDescending() const {
    if (root == nullptr) {
        std::cout << "No scores recorded.\n";
        return;
    }
    std::cout << "Score ranking (high -> low):\n";
    int rank = 1;
    printDescendingRecursive(root, rank);
}

// In-order copy of every record into the caller's array (up to capacity).
void ScoreTree::copyRecursive(Node* current, ScoreRecord* out, int& index, int capacity) const {
    if (current == nullptr || index >= capacity) return;
    copyRecursive(current->left, out, index, capacity);
    if (index < capacity) out[index++] = current->record;
    copyRecursive(current->right, out, index, capacity);
}

int ScoreTree::copyInto(ScoreRecord* out, int capacity) const {
    int index = 0;
    copyRecursive(root, out, index, capacity);
    return index;
}

void ScoreTree::clearRecursive(Node* current) {
    if (current == nullptr) return;
    clearRecursive(current->left);
    clearRecursive(current->right);
    delete current;
}

int ScoreTree::size() const { return count; }
bool ScoreTree::isEmpty() const { return count == 0; }

void ScoreTree::clear() {
    clearRecursive(root);
    root = nullptr;
    count = 0;
}

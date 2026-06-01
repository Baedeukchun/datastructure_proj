#ifndef DS_SCORE_TREE_H
#define DS_SCORE_TREE_H

#include <string>

// One score record: which player got which score.
struct ScoreRecord {
    std::string playerName;
    int score;

    ScoreRecord(const std::string& name = "", int s = 0)
        : playerName(name), score(s) {}
};

// Binary Search Tree ordered by score.
// Used in: Game.h -> scoreTree.
// Why: BST keeps records sorted at all times. A right-root-left (reverse
//      in-order) traversal prints scores from highest to lowest in O(n),
//      which gives us a ranking display without a separate sort step.

class ScoreTree {
private:
    struct Node {
        ScoreRecord record;
        Node* left;
        Node* right;
        Node(const ScoreRecord& r) : record(r), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int count;

    Node* insertRecursive(Node* current, const ScoreRecord& record);
    bool containsRecursive(Node* current, const std::string& name) const;
    void printDescendingRecursive(Node* current, int& rank) const;
    void copyRecursive(Node* current, ScoreRecord* out, int& index, int capacity) const;
    void clearRecursive(Node* current);

    ScoreTree(const ScoreTree&) = delete;
    ScoreTree& operator=(const ScoreTree&) = delete;

public:
    ScoreTree();
    ~ScoreTree();

    void insert(const ScoreRecord& record);
    bool containsPlayer(const std::string& playerName) const;
    void printDescending() const;

    // Copy up to `capacity` records into `out` (in-order). Returns how many.
    int copyInto(ScoreRecord* out, int capacity) const;
    int size() const;
    bool isEmpty() const;
    void clear();
};

#endif

#ifndef DS_STACK_H
#define DS_STACK_H

// Stack (LIFO) implemented with linked nodes.
// Used in: Game.h -> moveHistory (Stack<int> of room IDs).
// Why: undo must return the *previous* room. The most recently pushed room
//      should pop first, which is exactly LIFO behavior.

template <typename T>
class Stack {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* topNode;
    int count;

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void push(const T& value) {
        Node* node = new Node(value);
        node->next = topNode;
        topNode = node;
        ++count;
    }

    bool pop(T& output) {
        if (topNode == nullptr) return false;
        Node* old = topNode;
        output = old->value;
        topNode = old->next;
        delete old;
        --count;
        return true;
    }

    bool peek(T& output) const {
        if (topNode == nullptr) return false;
        output = topNode->value;
        return true;
    }

    void clear() {
        while (topNode != nullptr) {
            Node* old = topNode;
            topNode = topNode->next;
            delete old;
        }
        count = 0;
    }
};

#endif

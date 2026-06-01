#ifndef DS_QUEUE_H
#define DS_QUEUE_H

// Queue (FIFO) implemented with linked nodes.
// Used in: Game.h -> eventQueue (Queue<GameEvent>).
// Why: events should be processed in the order they were scheduled.
//      FIFO is the natural fit.

template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    int count;

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~Queue() { clear(); }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void enqueue(const T& value) {
        Node* node = new Node(value);
        if (rearNode == nullptr) {
            frontNode = node;
            rearNode = node;
        } else {
            rearNode->next = node;
            rearNode = node;
        }
        ++count;
    }

    bool dequeue(T& output) {
        if (frontNode == nullptr) return false;
        Node* old = frontNode;
        output = old->value;
        frontNode = old->next;
        if (frontNode == nullptr) rearNode = nullptr;
        delete old;
        --count;
        return true;
    }

    bool peek(T& output) const {
        if (frontNode == nullptr) return false;
        output = frontNode->value;
        return true;
    }

    void clear() {
        while (frontNode != nullptr) {
            Node* old = frontNode;
            frontNode = frontNode->next;
            delete old;
        }
        rearNode = nullptr;
        count = 0;
    }
};

#endif

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>

// A short atmospheric beat. The queue plays these one at a time when the
// player types `event`. Score deltas are applied immediately.
struct GameEvent {
    std::string description;
    int scoreDelta;

    GameEvent(const std::string& d = "", int s = 0)
        : description(d), scoreDelta(s) {}
};

#endif

#include "Game.h"
#include <string>

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--reset") {
        Game::resetScores();
        return 0;
    }

    Game game;
    game.run();
    return 0;
}

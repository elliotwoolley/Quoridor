#ifndef QUORIDOR_GAME_MANAGER_H
#define QUORIDOR_GAME_MANAGER_H

#include <stdint.h>

enum PlayerType {
    Human,
    AI,
};

struct Player {
    enum PlayerType type;
    int difficulty;
};

void run_game(struct Player p1, struct Player p2);

#endif //QUORIDOR_GAME_MANAGER_H

#include "game_manager.h"

int main() {

    // Play 2 AI against each other, p1 always goes first
    struct Player p1 = (struct Player) { .type = AI, .difficulty = 6 };
    struct Player p2 = (struct Player) { .type = AI, .difficulty = 6 };

    run_game(p1, p2);

    // Play against AI
    p1 = (struct Player) { .type = Human };
    p2 = (struct Player) { .type = AI, .difficulty = 7 };

    run_game(p1, p2);

    // Run match between 2 humans
    p1 = (struct Player) { .type = Human };
    p2 = (struct Player) { .type = Human };

    run_game(p1, p2);
}

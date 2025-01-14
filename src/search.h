#ifndef QUORIDOR_SEARCH_H
#define QUORIDOR_SEARCH_H

#include <stdint.h>
#include "state.h"
#include "move.h"

struct Move get_best_move(struct State state, int depth);

struct Move get_best_move_iterative_deepening(struct State state, int depth);

#endif //QUORIDOR_SEARCH_H

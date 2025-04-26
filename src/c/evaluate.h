#ifndef QUORIDOR_EVALUATE_H
#define QUORIDOR_EVALUATE_H

#include "state.h"

#define WINNING_SCORE 1000

/**
 * @brief Evaluates the current game state using a heuristic function.
 *
 * For now this just wraps `optimal_path_dif_heuristic`.
 *
 * @param state The current game state.
 * @return int A heuristic value representing how favorable the state is to the current player.
 */
int evaluate(struct State state);

#endif //QUORIDOR_EVALUATE_H

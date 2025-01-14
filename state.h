#ifndef QUORIDOR_STATE_H
#define QUORIDOR_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "move.h"

#define GAME_BOARD_SIZE 9
#define GAME_BOARD_MAX_INDEX (GAME_BOARD_SIZE - 1)  // TODO : Rename
#define square64(index) ((uint64_t) 1 << (index))
#define square128(index) (((__uint128_t) 1) << (index))

/**
 * We choose 0 for two reasons:
 *  1. 0 is not a valid evaluation, Turns are taken sequentially, so
 *     it is not possible for each player to reach their goal at the
 *     same time (which is what a evaluation of 0 would indicate).
 *  2. While we are performing our game tree search (negamax), the
 *     sign of the state evaluation flips at each level of the tree.
 *     Using 0 means that we can handle this more easily.
 */
#define NO_PATH_FOUND 0

#define player_1_win_check(state)  ((state).player_1_row == 0)
#define player_2_win_check(state)  ((state).player_2_row == GAME_BOARD_MAX_INDEX)

struct State {
    uint64_t vertical_fences;
    uint64_t horizontal_fences;
    uint8_t player_1_row;
    uint8_t player_1_col;
    uint8_t player_2_row;
    uint8_t player_2_col;
    uint8_t player_1_fence_count;
    uint8_t player_2_fence_count;
    uint8_t player_to_move;
};

struct State new_state();

void print_state(struct State state);

bool equal_states(struct State state1, struct State state2);

void make_horizontal_fence_move(struct State *state, FenceMove move);

void unmake_horizontal_fence_move(struct State *state, FenceMove move);

void make_vertical_fence_move(struct State *state, FenceMove move);

void unmake_vertical_fence_move(struct State *state, FenceMove move);

void make_pawn_move(struct State *state, PawnMove move);

void unmake_pawn_move(struct State *state, PawnMove move);

void make_move(struct State *state, struct Move move);

void unmake_move(struct State *state, struct Move move);

bool move_is_fully_legal(struct State *state, struct Move move);

#endif //QUORIDOR_STATE_H

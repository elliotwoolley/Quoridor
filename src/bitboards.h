#ifndef BITBOARDS_H
#define BITBOARDS_H

#include <stdint.h>
#include "state.h"

#define bitboard_up(board) ((board) >> 11)
#define bitboard_down(board) ((board) << 11)
#define bitboard_left(board) ((board) >> 1)
#define bitboard_right(board) ((board) << 1)

/**
 * @struct Bitboards
 * @brief Contains fence-based bitmasks used for pathfinding and move generation.
 *
 * This struct holds four 128-bit bitboards that represent the presence of fences in a
 * Quoridor board. Each field encodes which squares are blocked when moving in that
 * respective direction, i.e. `up_fences` blocks upwards movement.
 */
struct Bitboards {
    __uint128_t up_fences;
    __uint128_t right_fences;
    __uint128_t down_fences;
    __uint128_t left_fences;
};

/**
 * @brief Generate fence-based bitboards from the given state.
 *
 * This function takes the State struct and produces bitmasks representing fences
 * and the board boundaries for use in pathfinding and move generation.
 *
 * @param state The game state.
 * @param bb Pointer to a Bitboards structure to store the result.
 */
void generate_bitboards(const struct State *state, struct Bitboards *bb);

#endif // BITBOARDS_H
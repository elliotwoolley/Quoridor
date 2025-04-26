#include <assert.h>
#include "evaluate.h"
#include "bitboards.h"

/**
 * @brief Calculates the shortest path length from a player's position to their goal row.
 *
 * This function performs a Breadth-First Search expansion to find the shortest
 * path, taking into account fences that block movement.
 *
 * @param bb Struct of bitmasks representing fences blocking movement in each direction.
 * @param player_row Row position of the player.
 * @param player_col Column position of the player.
 * @param goal_row_mask Bitmask identifying the player's goal row.
 * @return int
 * - The shortest path length as an integer.
 * - `NO_PATH_FOUND` if there no possible path to the goal.
 */
int do_optimal_path_length_loop(struct Bitboards *bb, uint8_t player_row, uint8_t player_col, __uint128_t goal_row_mask) {

    __uint128_t player_mask = square128(12 + player_row * 11 + player_col);
    __uint128_t previous_board, previous_goal_mask;

    int path_length = 0;

    while (!(player_mask & goal_row_mask)) {
        previous_board = player_mask;
        previous_goal_mask = goal_row_mask;

        __uint128_t board_up    = bitboard_up(previous_board) & ~bb->up_fences;
        __uint128_t board_down  = bitboard_down(previous_board) & ~bb->down_fences;
        __uint128_t board_left  = bitboard_left(previous_board) & ~bb->left_fences;
        __uint128_t board_right = bitboard_right(previous_board) & ~bb->right_fences;

        player_mask |= board_up | board_down | board_left | board_right;

        path_length++;

        if (player_mask & goal_row_mask) {
            return path_length;
        }

        __uint128_t goal_up    = bitboard_up(previous_goal_mask) & ~bb->up_fences;
        __uint128_t goal_down  = bitboard_down(previous_goal_mask) & ~bb->down_fences;
        __uint128_t goal_left  = bitboard_left(previous_goal_mask) & ~bb->left_fences;
        __uint128_t goal_right = bitboard_right(previous_goal_mask) & ~bb->right_fences;

        goal_row_mask |= goal_up | goal_down | goal_left | goal_right;

        //  Stops if there are no more game squares to expand to. This indicates that not path was found.
        if (previous_board == player_mask || previous_goal_mask == goal_row_mask) {
            return NO_PATH_FOUND;
        }

        path_length++;
    }

    return path_length;
}

/**
 * @brief Computes the shortest path length for a specific player to their goal row.
 *
 * This function initializes player-specific parameters and uses
 * `do_optimal_path_length_loop` to perform the calculation.
 *
 * @param state The game state.
 * @param player The player identifier (1 or 2).
 * @param bb Struct of bitmasks representing fences blocking movement in each direction.
 * @return int
 * - The shortest path length as an integer.
 * - `NO_PATH_FOUND` if no valid path exists.
 */
int optimal_path_length_bi(struct State state, uint8_t player, struct Bitboards *bb) {
    uint8_t player_row, player_col;
    __uint128_t goal_row_mask;
    switch (player) {
        case 1:
            player_row = state.player_1_row;
            player_col = state.player_1_col;
            goal_row_mask = (__uint128_t) 0b111111111 << 12;
            break;
        case 2:
            player_row = state.player_2_row;
            player_col = state.player_2_col;
            goal_row_mask = (__uint128_t) 0b111111111 << 100;
            break;
        default:
            assert(false && "Illegal player value: %d");
    }

    return do_optimal_path_length_loop(bb, player_row, player_col, goal_row_mask);
}

/**
 * @brief Evaluates the heuristic value of the current game state based on player path differences.
 *
 * Computes the optimal path lengths for both players and returns the difference. Distances are
 * multiplied by 2 to allow us to account for whose turn it is.
 *
 * @param state The game state.
 * @return int
 * - Heuristic based on how many moves sooner the current player will reach their goal assuming no
 *   fences are played.
 * - `-NO_PATH_FOUND` if the board state is invalid.
 */
int optimal_path_dif_heuristic(struct State state) {

    struct Bitboards bb;
    generate_bitboards(&state, &bb);

    const int p1_path_length = optimal_path_length_bi(state, 1, &bb);
    const int p2_path_length = optimal_path_length_bi(state, 2, &bb);

    if (p1_path_length == NO_PATH_FOUND || p2_path_length == NO_PATH_FOUND) {
        return NO_PATH_FOUND;
    }

    assert(((state.player_to_move == 1 ? 2 : -2) * (p2_path_length - p1_path_length) + 1) != 0 && "path dif heuristic cannot be zero");
    return (state.player_to_move == 1 ? 2 : -2) * (p2_path_length - p1_path_length) + 1;
}

int evaluate(struct State state) {
    return optimal_path_dif_heuristic(state);
}

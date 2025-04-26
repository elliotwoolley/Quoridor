#include <assert.h>
#include <stdio.h>
#include "move_generation.h"
#include "bitboards.h"
#include "evaluate.h"

FenceMoves generate_pseudo_legal_vertical_fence_moves(struct State state) {
    if (state.player_to_move == 1 && state.player_1_fence_count > 0 ||
        state.player_to_move == 2 && state.player_2_fence_count > 0) {

        return ~(state.vertical_fences | state.vertical_fences << FENCE_BIT_BOARD_WIDTH |
                 state.vertical_fences >> FENCE_BIT_BOARD_WIDTH | state.horizontal_fences);
    }

    return 0;
}


FenceMoves generate_pseudo_legal_horizontal_fence_moves(struct State state) {
    if (state.player_to_move == 1 && state.player_1_fence_count > 0 ||
        state.player_to_move == 2 && state.player_2_fence_count > 0) {

        return ~(state.horizontal_fences | (state.horizontal_fences & ~RIGHT_FENCE_MASK) << 1 |
                 (state.horizontal_fences & ~LEFT_FENCE_MASK) >> 1 | state.vertical_fences);
    }

    return 0;
}

PawnMoves generate_legal_pawn_moves(struct State state) {
    uint8_t player_row, player_col, opponent_row, opponent_col;
    switch (state.player_to_move) {
        case 1:
            player_row = state.player_1_row;
            player_col = state.player_1_col;
            opponent_row = state.player_2_row;
            opponent_col = state.player_2_col;
            break;
        case 2:
            player_row = state.player_2_row;
            player_col = state.player_2_col;
            opponent_row = state.player_1_row;
            opponent_col = state.player_1_col;
            break;
        default:
            assert(false && "generate_legal_pawn_moves: Illegal state.player_to_move value.");
    }

    PawnMoves pawnMoves = 0;
    __uint128_t player_board = square128(12 + player_row * 11 + player_col);
    __uint128_t opponent_board = square128(12 + opponent_row * 11 + opponent_col);

    struct Bitboards bb;
    generate_bitboards(&state, &bb);

    if (bitboard_up(player_board) & ~bb.up_fences) {
        if (bitboard_up(player_board) & opponent_board) {
            if (bitboard_up(opponent_board) & ~bb.up_fences) {
                pawnMoves |= NorthNorth;
            } else {
                if (bitboard_left(opponent_board) & ~bb.left_fences) {
                    pawnMoves |= NorthWest;
                }
                if (bitboard_right(opponent_board) & ~bb.right_fences) {
                    pawnMoves |= NorthEast;
                }
            }
        } else {
            pawnMoves |= North;
        }
    }

    if (bitboard_right(player_board) & ~bb.right_fences) {
        if (bitboard_right(player_board) & opponent_board) {
            if (bitboard_right(opponent_board) & ~bb.right_fences) {
                pawnMoves |= EastEast;
            } else {
                if (bitboard_up(opponent_board) & ~bb.up_fences) {
                    pawnMoves |= NorthEast;
                }
                if (bitboard_down(opponent_board) & ~bb.down_fences) {
                    pawnMoves |= SouthEast;
                }
            }
        } else {
            pawnMoves |= East;
        }
    }

    if (bitboard_left(player_board) & ~bb.left_fences) {
        if (bitboard_left(player_board) & opponent_board) {
            if (bitboard_left(opponent_board) & ~bb.left_fences) {
                pawnMoves |= WestWest;
            } else {
                if (bitboard_down(opponent_board) & ~bb.down_fences) {
                    pawnMoves |= SouthWest;
                }
                if (bitboard_up(opponent_board) & ~bb.up_fences) {
                    pawnMoves |= NorthWest;
                }
            }
        } else {
            pawnMoves |= West;
        }
    }

    if (bitboard_down(player_board) & ~bb.down_fences) {
        if (bitboard_down(player_board) & opponent_board) {
            if (bitboard_down(opponent_board) & ~bb.down_fences) {
                pawnMoves |= SouthSouth;
            } else {
                if (bitboard_right(opponent_board) & ~bb.right_fences) {
                    pawnMoves |= SouthEast;
                }
                if (bitboard_left(opponent_board) & ~bb.left_fences) {
                    pawnMoves |= SouthWest;
                }
            }
        } else {
            pawnMoves |= South;
        }
    }
    return pawnMoves;
}

FenceMoves generate_fully_legal_vertical_fence_moves(struct State state) {
    FenceMoves legal_moves = 0;
    FenceMoves pseudo_legal_moves = generate_pseudo_legal_vertical_fence_moves(state);

    while (pseudo_legal_moves != 0) {
        FenceMove move = square64(__builtin_ctzll(pseudo_legal_moves));
        pseudo_legal_moves &= pseudo_legal_moves - 1;
        make_vertical_fence_move(&state, move);

        if (evaluate(state) != NO_PATH_FOUND) {
            legal_moves |= move;
        }

        unmake_vertical_fence_move(&state, move);
    }

    return legal_moves;
}

FenceMoves generate_fully_legal_horizontal_fence_moves(struct State state) {
    FenceMoves legal_moves = 0;
    FenceMoves pseudo_legal_moves = generate_pseudo_legal_horizontal_fence_moves(state);

    while (pseudo_legal_moves != 0) {
        FenceMove move = square64(__builtin_ctzll(pseudo_legal_moves));
        pseudo_legal_moves &= pseudo_legal_moves - 1;
        make_horizontal_fence_move(&state, move);

        if (evaluate(state) != NO_PATH_FOUND) {
            legal_moves |= move;
        }

        unmake_horizontal_fence_move(&state, move);
    }

    return legal_moves;
}

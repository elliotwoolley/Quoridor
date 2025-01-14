#include <assert.h>
#include "move_generation.h"
#include "bitboards.h"

FenceMoves generate_pseudo_legal_vertical_fence_moves(struct State state) {
    if (state.player_to_move == 1 && state.player_1_fence_count > 0 ||
        state.player_to_move == 2 && state.player_2_fence_count > 0) {

        return ~(state.vertical_fences | state.vertical_fences << GAME_BOARD_MAX_INDEX | state.vertical_fences >> GAME_BOARD_MAX_INDEX |
                 state.horizontal_fences);
    }

    return 0;
}


FenceMoves generate_pseudo_legal_horizontal_fence_moves(struct State state) {
    if (state.player_to_move == 1 && state.player_1_fence_count > 0 ||
        state.player_to_move == 2 && state.player_2_fence_count > 0) {

        return ~(state.horizontal_fences | state.horizontal_fences << 1 | state.horizontal_fences >> 1 |
                 state.vertical_fences);
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

//int main() {
//    printf("wowee\n");
//
//    struct state state;
//    init_state(&state);
//
//    __uint128_t board = (one_128 << 108);
////    __uint128_t board = (one_128 << 108);
//    uint64_t vertical_fences = 0;
////    uint64_t horizontal_fences = 0;
//    uint64_t horizontal_fences = (1ull << 63) | (1ull << 61) | (1ull << 59) | (1ull << 57) | (1ull << 54) |
//                                 (1ull << 52) | (1ull << 50) | (1ull << 48) | (1ull << 47) | (1ull << 45) |
//                                 (1ull << 43) | (1ull << 41) | (1ull << 38) | (1ull << 36) | (1ull << 34) |
//                                 (1ull << 32) | (1ull << 31) | (1ull << 29) | (1ull << 27) | (1ull << 25);
//
//    state.horizontal_fences = horizontal_fences;
//
//
//    volatile uint64_t result = 0;
//
//    float start = (float) clock() / CLOCKS_PER_SEC;
//    for (int i = 0; i < 1000000000; i++) {
//        result += generate_horizontal_fence_moves(state);
//    }
//    float end = (float) clock() / CLOCKS_PER_SEC;
//    printf("%llu\n\n", result);
//    printf("%f seconds\n", end - start);
//}
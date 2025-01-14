#include "bitboards.h"

// 11x11 bit mask border, Surrounds the 9x9 game board
static const __uint128_t BORDER =
        ((__uint128_t) 0b111111111111000000000110000000001100000000011000000000110 << 64)
        | 0b11000000000110000000001100000000011000000000111111111111;

void generate_bitboards(const struct State *state, struct Bitboards *bb) {
    __uint128_t up_fences_1 = ((__uint128_t) state->horizontal_fences & 0b11111111) << 12;
    __uint128_t up_fences_2 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 8)) << 15;
    __uint128_t up_fences_3 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 16)) << 18;
    __uint128_t up_fences_4 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 24)) << 21;
    __uint128_t up_fences_5 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 32)) << 24;
    __uint128_t up_fences_6 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 40)) << 27;
    __uint128_t up_fences_7 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 48)) << 30;
    __uint128_t up_fences_8 = ((__uint128_t) state->horizontal_fences & ((uint64_t)0b11111111 << 56)) << 33;

    // Each fence blocks 2 squares
    up_fences_1 |= bitboard_right(up_fences_1);
    up_fences_2 |= bitboard_right(up_fences_2);
    up_fences_3 |= bitboard_right(up_fences_3);
    up_fences_4 |= bitboard_right(up_fences_4);
    up_fences_5 |= bitboard_right(up_fences_5);
    up_fences_6 |= bitboard_right(up_fences_6);
    up_fences_7 |= bitboard_right(up_fences_7);
    up_fences_8 |= bitboard_right(up_fences_8);

    __uint128_t up_fences = up_fences_1 | up_fences_2 | up_fences_3 | up_fences_4 |
                            up_fences_5 | up_fences_6 | up_fences_7 | up_fences_8;

    __uint128_t right_fences_1 = ((__uint128_t) state->vertical_fences & 0b11111111) << 13;
    __uint128_t right_fences_2 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 8)) << 16;
    __uint128_t right_fences_3 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 16)) << 19;
    __uint128_t right_fences_4 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 24)) << 22;
    __uint128_t right_fences_5 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 32)) << 25;
    __uint128_t right_fences_6 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 40)) << 28;
    __uint128_t right_fences_7 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 48)) << 31;
    __uint128_t right_fences_8 = ((__uint128_t) state->vertical_fences & ((uint64_t)0b11111111 << 56)) << 34;

    // Each fence blocks 2 squares
    right_fences_1 |= bitboard_down(right_fences_1);
    right_fences_2 |= bitboard_down(right_fences_2);
    right_fences_3 |= bitboard_down(right_fences_3);
    right_fences_4 |= bitboard_down(right_fences_4);
    right_fences_5 |= bitboard_down(right_fences_5);
    right_fences_6 |= bitboard_down(right_fences_6);
    right_fences_7 |= bitboard_down(right_fences_7);
    right_fences_8 |= bitboard_down(right_fences_8);

    __uint128_t right_fences = right_fences_1 | right_fences_2 | right_fences_3 | right_fences_4 |
                               right_fences_5 | right_fences_6 | right_fences_7 | right_fences_8;

    // Each fence blocks 2 directions (up/down or left/right)
    __uint128_t down_fences = bitboard_down(up_fences);
    __uint128_t left_fences = bitboard_left(right_fences);

    up_fences |= BORDER;
    down_fences |= BORDER;
    left_fences |= BORDER;
    right_fences |= BORDER;

    bb->up_fences = up_fences;
    bb->right_fences = right_fences;
    bb->down_fences = down_fences;
    bb->left_fences = left_fences;
}
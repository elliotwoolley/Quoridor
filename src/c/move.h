#ifndef QUORIDOR_MOVE_H
#define QUORIDOR_MOVE_H

#include <stdint.h>
#include <stdbool.h>

enum MoveType {
    None,
    VerticalFence,
    HorizontalFence,
    Pawn,
};

typedef uint64_t FenceMoves;
typedef uint64_t FenceMove;

typedef uint16_t PawnMoves;
typedef uint16_t PawnMove;

//  Moves with 2 directions are jump moves, see https://en.wikipedia.org/wiki/Quoridor#Rules_of_the_game
enum PawnMoveTypes {
    North       = 0b000000000001,
    East        = 0b000000000010,
    South       = 0b000000000100,
    West        = 0b000000001000,

    NorthNorth  = 0b000000010000,
    EastEast    = 0b000000100000,
    SouthSouth  = 0b000001000000,
    WestWest    = 0b000010000000,

    NorthEast   = 0b000100000000,
    NorthWest   = 0b001000000000,
    SouthEast   = 0b010000000000,
    SouthWest   = 0b100000000000
};

struct Move {
    union {
        PawnMove pawnMove;
        FenceMove fenceMove;
    } move;
    enum MoveType moveType;
    int score;
};

FenceMove create_fence_move(int row, int col);

char *move_type_to_string(enum MoveType move_type);

char *pawn_move_type_to_string(enum PawnMoveTypes pawn_move_type);

void print_move(struct Move move);

#endif //QUORIDOR_MOVE_H

#include <printf.h>
#include <assert.h>
#include "move.h"
#include "state.h"


FenceMove create_fence_move(int row, int col) {
    assert(0 <= row && row < GAME_BOARD_MAX_INDEX);
    assert(0 <= col && col < GAME_BOARD_MAX_INDEX);
    return square64(row * GAME_BOARD_MAX_INDEX + col);
}

char* move_type_to_string(enum MoveType move_type) {
    switch(move_type) {
        case None:              return "None";
        case VerticalFence:     return "Vertical Fence";
        case HorizontalFence:   return "Horizontal Fence";
        case Pawn:              return "Pawn";
    }
}

char* pawn_move_type_to_string(enum PawnMoveTypes pawn_move_type) {
    switch(pawn_move_type) {
        case North:         return "North";
        case East:          return "East";
        case South:         return "South";
        case West:          return "West";
        case NorthNorth:    return "North North";
        case EastEast:      return "East East";
        case SouthSouth:    return "South South";
        case WestWest:      return "West West";
        case NorthEast:     return "North East";
        case NorthWest:     return "North West";
        case SouthEast:     return "South East";
        case SouthWest:     return "South West";
    }
}

void print_move(struct Move move) {
    printf("%s: ", move_type_to_string(move.moveType));
    switch(move.moveType) {
        case None:
            break;
        case VerticalFence:
        case HorizontalFence:
            printf("Row %d, Col %d. ", __builtin_ctzll(move.move.fenceMove) / 8, __builtin_ctzll(move.move.fenceMove) % 8);
            break;
        case Pawn:
            printf("Pawn moves %s. ", pawn_move_type_to_string(move.move.pawnMove));
            break;
    }
    printf("(Score %d)\n", move.score);
}

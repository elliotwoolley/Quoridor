#include <wchar.h>
#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include "state.h"
#include "move_generation.h"
#include "evaluate.h"

#define switch_player(state) state->player_to_move ^= 0b11

struct State new_state() {
    return (struct State) {
            .vertical_fences = 0,
            .horizontal_fences = 0,
            .player_1_row = BOARD_SIZE - 1,
            .player_1_col = (BOARD_SIZE - 1) / 2,
            .player_2_row = 0,
            .player_2_col = (BOARD_SIZE - 1) / 2,
            .player_1_fence_count = 10,
            .player_2_fence_count = 10,
            .player_to_move = 1,
    };
}

void print_state(struct State state) {
    setlocale(LC_CTYPE, "");

    int row, col;

    printf("\n");
    printf("   ");

    for (col = 0; col < BOARD_SIZE - 1; col++) {
        wprintf(L"  %d  %lc", col, (wchar_t) 0x2080 + col);
    }
    printf("  %d", col);
    printf("\n");
    for (row = 0; row < BOARD_SIZE; row++) {
        printf(" %d ", row);
        for (col = 0; col < BOARD_SIZE; col++) {  //  Prints Board Row
            if (state.player_1_col == col && state.player_1_row == row) {  //  Player 1
                printf("  X  ");
            } else if (state.player_2_col == col && state.player_2_row == row) {  //  Player 2
                printf("  O  ");
            } else {  //  No Pawns
                printf("  •  ");
            }
            if (col < BOARD_SIZE - 1) {  //  Prints Vertical Fences
                if (row < BOARD_SIZE - 1 && (state.vertical_fences & (square64(8 * row + col))) ||
                    row > 0 && (state.vertical_fences & square64(8 * (row - 1) + col))) {
                    printf("|");
                } else {
                    printf(" ");
                }
            }
        }
        if (row < BOARD_SIZE - 1) {  //  Prints in between Board Rows
            wprintf(L"\n  %lc ", (wchar_t) 0x2080 + row);
            for (col = 0; col < BOARD_SIZE - 1; col++) {
                if (state.horizontal_fences & (square64(8 * row + col))) {
                    printf("---");
                    printf("---");
                } else {
                    if (col > 0 && (state.horizontal_fences & square64(8 * row + (col - 1)))) {
                        printf("---");
                    } else {
                        printf("   ");
                    }
                    if (state.vertical_fences & (square64(8 * row + col))) {
                        printf(" | ");
                    } else {
                        printf("   ");
                    }
                }
            }
            if (state.horizontal_fences &
                square64(8 * row + (col - 1))) {  //  Handles the End of Horizontal Fences in Last Column
                printf("---");
            }
        }

        printf("\n");
    }
    printf("X Fences: %2d\nO Fences: %2d\n", state.player_1_fence_count, state.player_2_fence_count);
    printf("%c's Turn\n", state.player_to_move == 1 ? 'X' : 'O');
}

bool equal_states(struct State state1, struct State state2) {
    if (state1.vertical_fences != state2.vertical_fences) { return false; }
    if (state1.horizontal_fences != state2.horizontal_fences) { return false; }
    if (state1.player_1_row != state2.player_1_row) { return false; }
    if (state1.player_1_col != state2.player_1_col) { return false; }
    if (state1.player_2_row != state2.player_2_row) { return false; }
    if (state1.player_2_col != state2.player_2_col) { return false; }
    if (state1.player_1_fence_count != state2.player_1_fence_count) { return false; }
    if (state1.player_2_fence_count != state2.player_2_fence_count) { return false; }
    if (state1.player_to_move != state2.player_to_move) { return false; }
    return true;
}

void make_horizontal_fence_move(struct State *state, FenceMove move) {
    assert(generate_pseudo_legal_horizontal_fence_moves(*state) & move && "Illegal fence move, fence in the way.");
    state->horizontal_fences |= move;

    if (state->player_to_move == 1) {
        state->player_1_fence_count--;
    } else {
        state->player_2_fence_count--;
    }

    switch_player(state);
}

void unmake_horizontal_fence_move(struct State *state, FenceMove move) {
    assert(state->horizontal_fences & move && "No horizontal fence at this location.");
    state->horizontal_fences &= ~move;

    if (state->player_to_move == 2) {
        state->player_1_fence_count++;
    } else {
        state->player_2_fence_count++;
    }

    switch_player(state);
}

void make_vertical_fence_move(struct State *state, FenceMove move) {
    assert(generate_pseudo_legal_vertical_fence_moves(*state) & move && "Illegal fence move, fence in the way.");
    state->vertical_fences |= move;

    if (state->player_to_move == 1) {
        state->player_1_fence_count--;
    } else {
        state->player_2_fence_count--;
    }

    switch_player(state);
}

void unmake_vertical_fence_move(struct State *state, FenceMove move) {
    assert(state->vertical_fences & move && "No vertical fence at this location.");
    state->vertical_fences &= ~move;

    if (state->player_to_move == 2) {
        state->player_1_fence_count++;
    } else {
        state->player_2_fence_count++;
    }

    switch_player(state);
}

void make_pawn_move(struct State *state, PawnMove move) {
    assert(generate_legal_pawn_moves(*state) & move && "Illegal pawn move, fence in the way.");
    if (state->player_to_move == 1) {
        state->player_to_move = 2;
        switch (move) {
            case North:
                state->player_1_row -= 1;
                break;
            case East:
                state->player_1_col += 1;
                break;
            case South:
                state->player_1_row += 1;
                break;
            case West:
                state->player_1_col -= 1;
                break;
            case NorthNorth:
                state->player_1_row -= 2;
                break;
            case EastEast:
                state->player_1_col += 2;
                break;
            case SouthSouth:
                state->player_1_row += 2;
                break;
            case WestWest:
                state->player_1_col -= 2;
                break;
            case NorthEast:
                state->player_1_col += 1;
                state->player_1_row -= 1;
                break;
            case NorthWest:
                state->player_1_col -= 1;
                state->player_1_row -= 1;
                break;
            case SouthEast:
                state->player_1_col += 1;
                state->player_1_row += 1;
                break;
            case SouthWest:
                state->player_1_col -= 1;
                state->player_1_row += 1;
                break;
            default:
                assert(false && "make_pawn_move: Unrecognized pawn move.");
        }
        assert(state->player_1_row >= 0);
        assert(state->player_1_col <= BOARD_SIZE - 1);
    } else {
        state->player_to_move = 1;
        switch (move) {
            case North:
                state->player_2_row -= 1;
                break;
            case East:
                state->player_2_col += 1;
                break;
            case South:
                state->player_2_row += 1;
                break;
            case West:
                state->player_2_col -= 1;
                break;
            case NorthNorth:
                state->player_2_row -= 2;
                break;
            case EastEast:
                state->player_2_col += 2;
                break;
            case SouthSouth:
                state->player_2_row += 2;
                break;
            case WestWest:
                state->player_2_col -= 2;
                break;
            case NorthEast:
                state->player_2_col += 1;
                state->player_2_row -= 1;
                break;
            case NorthWest:
                state->player_2_col -= 1;
                state->player_2_row -= 1;
                break;
            case SouthEast:
                state->player_2_col += 1;
                state->player_2_row += 1;
                break;
            case SouthWest:
                state->player_2_col -= 1;
                state->player_2_row += 1;
                break;
            default:
                assert(false && "make_pawn_move: Unrecognized pawn move.");
        }
        assert(state->player_2_row >= 0);
        assert(state->player_2_col <= BOARD_SIZE - 1);
    }
}

void unmake_pawn_move(struct State *state, PawnMove move) {
    if (state->player_to_move == 2) {
        state->player_to_move = 1;
        switch (move) {
            case North:
                state->player_1_row += 1;
                break;
            case East:
                state->player_1_col -= 1;
                break;
            case South:
                state->player_1_row -= 1;
                break;
            case West:
                state->player_1_col += 1;
                break;
            case NorthNorth:
                state->player_1_row += 2;
                break;
            case EastEast:
                state->player_1_col -= 2;
                break;
            case SouthSouth:
                state->player_1_row -= 2;
                break;
            case WestWest:
                state->player_1_col += 2;
                break;
            case NorthEast:
                state->player_1_col -= 1;
                state->player_1_row += 1;
                break;
            case NorthWest:
                state->player_1_col += 1;
                state->player_1_row += 1;
                break;
            case SouthEast:
                state->player_1_col -= 1;
                state->player_1_row -= 1;
                break;
            case SouthWest:
                state->player_1_col += 1;
                state->player_1_row -= 1;
                break;
            default:
                assert(false && "unmake_pawn_move: Unrecognized pawn move.");
        }
        assert(state->player_1_row >= 0);
        assert(state->player_1_col <= BOARD_SIZE - 1);
    } else {
        state->player_to_move = 2;
        switch (move) {
            case North:
                state->player_2_row += 1;
                break;
            case East:
                state->player_2_col -= 1;
                break;
            case South:
                state->player_2_row -= 1;
                break;
            case West:
                state->player_2_col += 1;
                break;
            case NorthNorth:
                state->player_2_row += 2;
                break;
            case EastEast:
                state->player_2_col -= 2;
                break;
            case SouthSouth:
                state->player_2_row -= 2;
                break;
            case WestWest:
                state->player_2_col += 2;
                break;
            case NorthEast:
                state->player_2_col -= 1;
                state->player_2_row += 1;
                break;
            case NorthWest:
                state->player_2_col += 1;
                state->player_2_row += 1;
                break;
            case SouthEast:
                state->player_2_col -= 1;
                state->player_2_row -= 1;
                break;
            case SouthWest:
                state->player_2_col += 1;
                state->player_2_row -= 1;
                break;
            default:
                assert(false && "unmake_pawn_move: Unrecognized pawn move.");
        }
        assert(state->player_2_row >= 0);
        assert(state->player_2_col <= BOARD_SIZE - 1);
    }
}

void make_move(struct State *state, struct Move *move) {
    switch (move->moveType) {
        case VerticalFence:
            make_vertical_fence_move(state, move->move.fenceMove);
            break;
        case HorizontalFence:
            make_horizontal_fence_move(state, move->move.fenceMove);
            break;
        case Pawn:
            make_pawn_move(state, move->move.pawnMove);
            break;
        case None:
            assert(move->moveType != None);
            break;
        default:
            assert(false && "make_move: Illegal move.moveType.");
    }
}

void unmake_move(struct State *state, struct Move *move) {
    switch (move->moveType) {
        case VerticalFence:
            unmake_vertical_fence_move(state, move->move.fenceMove);
            break;
        case HorizontalFence:
            unmake_horizontal_fence_move(state, move->move.fenceMove);
            break;
        case Pawn:
            unmake_pawn_move(state, move->move.pawnMove);
            break;
        case None:
            assert(move->moveType != None);
            break;
        default:
            assert(false && "unmake_move: Illegal move.moveType.");
    }
}

bool move_is_fully_legal(struct State *state, struct Move move) {
    bool move_is_legal;
    FenceMoves legal_fence_moves;
    switch (move.moveType) {
        case VerticalFence:
            legal_fence_moves = generate_pseudo_legal_vertical_fence_moves(*state);

            if ((legal_fence_moves & move.move.fenceMove) == 0) {
                return false;
            }

            make_vertical_fence_move(state, move.move.fenceMove);
            move_is_legal = evaluate(*state) != NO_PATH_FOUND;
            unmake_vertical_fence_move(state, move.move.fenceMove);
            return move_is_legal;
        case HorizontalFence:
            legal_fence_moves = generate_pseudo_legal_horizontal_fence_moves(*state);

            if ((legal_fence_moves & move.move.fenceMove) == 0) {
                return false;
            }

            make_horizontal_fence_move(state, move.move.fenceMove);
            move_is_legal = evaluate(*state) != NO_PATH_FOUND;
            unmake_horizontal_fence_move(state, move.move.fenceMove);
            return move_is_legal;
        case Pawn:
            return generate_legal_pawn_moves(*state) && move.move.pawnMove;
        case None:
            assert(move.moveType != None);  // Return false instead?
            break;
        default:
            assert(false && "unmake_move: Illegal move.moveType.");
    }
}

bool win_check(struct State *state) {
    return player_1_win_check(*state) || player_2_win_check(*state);
}

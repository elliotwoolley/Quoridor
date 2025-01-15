#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "game_manager.h"
#include "move_generation.h"   // generate_pawn_moves, generate_fence_moves, ...
#include "search.h"            // get_best_move_iterative_deepening(...)
#include "evaluate.h"          // evaluate(...)
#include "state.h"             // new_state(), make_move(), unmake_move(), ...
#include "move.h"              // struct Move, MoveType

/**
 * Helper function: Prompt a human for a move, parse it, and return it.
 *
 * You can customize:
 *  - the text prompts
 *  - the exact input format
 *  - how you handle errors or invalid moves
 */
static struct Move get_move_from_user(struct State *state) {
    // In Quoridor, there are basically two categories of moves:
    //  1) Pawn move (single-step, jump, diagonal jump)
    //  2) Fence move (horizontal or vertical)

    while (true) {
        // Print a prompt
        printf("Enter move [P for pawn, HF for horizontal fence, VF for vertical fence, AI for letting the AI decide].\n");
        printf(" Example inputs:\n");
        printf("   P N     (move pawn North)\n");
        printf("   P SE    (move pawn SouthEast)\n");
        printf("   HF 2 3  (place horizontal fence at row=2, col=3)\n");
        printf("   VF 4 5  (place vertical fence at row=4, col=5)\n");
        printf("   AI 6    (have the AI choose a move for you, looking 6 moves deep)\n");
        printf("> ");

        char command[16];

        if (!fgets(command, sizeof(command), stdin)) {
            fprintf(stderr, "Error reading input. Try again.\n");
            continue;
        }
        // Strip trailing newline
        char *newline = strchr(command, '\n');
        if (newline) *newline = '\0';

        if (strncmp(command, "P", 1) == 0) {
            char direction[4];

            if (sscanf(command, "P %3s", direction) != 1) {
                printf("Invalid pawn command format.\n");
                continue;
            }

            PawnMove pawn_move;
            if (strcmp(direction, "N") == 0) pawn_move = North;
            else if (strcmp(direction, "E") == 0) pawn_move = East;
            else if (strcmp(direction, "S") == 0) pawn_move = South;
            else if (strcmp(direction, "W") == 0) pawn_move = West;
            else if (strcmp(direction, "NN") == 0) pawn_move = NorthNorth;
            else if (strcmp(direction, "EE") == 0) pawn_move = EastEast;
            else if (strcmp(direction, "SS") == 0) pawn_move = SouthSouth;
            else if (strcmp(direction, "WW") == 0) pawn_move = WestWest;
            else if (strcmp(direction, "NE") == 0) pawn_move = NorthEast;
            else if (strcmp(direction, "NW") == 0) pawn_move = NorthWest;
            else if (strcmp(direction, "SE") == 0) pawn_move = SouthEast;
            else if (strcmp(direction, "SW") == 0) pawn_move = SouthWest;
            else {
                printf("Unknown pawn direction '%s'.\n", direction);
                continue;
            }

            PawnMoves legal_pawn_moves = generate_legal_pawn_moves(*state);
            if ((legal_pawn_moves & pawn_move) == 0) {
                printf("That pawn move is not valid. Try again.\n");
                continue;
            }

            return (struct Move) { .moveType = Pawn, .move.pawnMove = pawn_move };

        } else if (strncmp(command, "HF", 2) == 0) {

            int row, col;

            if (sscanf(command, "HF %d %d", &row, &col) != 2) {
                printf("Invalid HF command format.\n");
                continue;
            }

            if (row < 0 || row >= FENCE_BIT_BOARD_WIDTH || col < 0 || col >= FENCE_BIT_BOARD_WIDTH) {
                printf("Row or col out of range. Try again.\n");
                continue;
            }

            struct Move move = (struct Move) { .moveType = HorizontalFence, .move.fenceMove = create_fence_move(row, col) };

            if (!move_is_fully_legal(state, move)) {
                printf("Move is not legal. Try again.\n");
                continue;
            }

            return move;

        } else if (strncmp(command, "VF", 2) == 0) {

            int row, col;

            if (sscanf(command, "VF %d %d", &row, &col) != 2) {
                printf("Invalid VF command format.\n");
                continue;
            }

            if (row < 0 || row >= FENCE_BIT_BOARD_WIDTH || col < 0 || col >= FENCE_BIT_BOARD_WIDTH) {
                printf("Row or col out of range. Try again.\n");
                continue;
            }

            struct Move move = (struct Move) { .moveType = VerticalFence, .move.fenceMove = create_fence_move(row, col) };

            if (!move_is_fully_legal(state, move)) {
                printf("Move is not legal. Try again.\n");
                continue;
            }

            return move;

        } else if (strncmp(command, "AI", 2) == 0) {

            int depth;

            if (sscanf(command, "AI %d", &depth) != 1) {
                printf("Invalid AI command format.\n");
                continue;
            }

            if (depth <= 0) {
                printf("AI depth must be greater than 0. Try again.\n");
                continue;
            }

            printf("AI is thinking... ");
            fflush(stdout);  // This could take a moment

            return get_best_move_iterative_deepening(*state, depth);
        } else {
            printf("Unrecognized command. Try again.\n");
            continue;
        }
    }
}

/**
 * Main game loop.
 *  - Creates a new State.
 *  - Loops until either player wins.
 *  - For Human: prompts for move from stdin.
 *  - For AI: calls get_best_move_iterative_deepening(...)
 */
void run_game(struct Player p1, struct Player p2) {
    assert(p1.type == Human || p1.difficulty > 0);
    assert(p2.type == Human || p2.difficulty > 0);

    struct State state = new_state();

    int winningPlayer = 0;

    while (true) {
        print_state(state);
        printf("\n");

        if (player_1_win_check(state)) {
            winningPlayer = 1;
            break;
        }
        if (player_2_win_check(state)) {
            winningPlayer = 2;
            break;
        }

        struct Player current_player = state.player_to_move == 1 ? p1 : p2;

        struct Move move;
        switch (current_player.type) {
            case Human:
                move = get_move_from_user(&state);
                break;
            case AI:
                printf("AI is thinking... ");
                fflush(stdout);  // This could take a moment
                move = get_best_move_iterative_deepening(state, current_player.difficulty);
                printf("AI chose move: ");
                print_move(move);
                printf("\n");
                break;
            default:
                assert(false && "Invalid Player.type value");
        }

        make_move(&state, move);
    }

    printf("\n====================\n");
    printf("   Player %d wins!\n", winningPlayer);
    printf("====================\n");
}
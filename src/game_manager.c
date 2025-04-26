#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "game_manager.h"
#include "move_generation.h"
#include "search.h"
#include "state.h"
#include "move.h"

enum PlayerType {
    Human,
    AI,
};

struct Player {
    enum PlayerType type;
    int difficulty;
};


struct Player get_player_info(int player_number) {
    while (true) {
        printf("\n===== Player %d =====\n", player_number);
        printf("Select player type (H = Human, A = AI): ");

        char line[64];
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "Error reading input. Try again.\n");
            continue;
        }

        char choice = '\0';
        if (sscanf(line, " %c", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (choice == 'H' || choice == 'h') {
            return (struct Player) { .type = Human, .difficulty = 0 };
        }

        if (choice == 'A' || choice == 'a') {
            while (true) {
                printf("Enter AI difficulty (1 to 6): ");

                if (!fgets(line, sizeof(line), stdin)) {
                    fprintf(stderr, "Error reading input. Try again.\n");
                    continue;
                }

                int difficulty;
                if (sscanf(line, "%d", &difficulty) != 1) {
                    printf("Invalid integer. Try again.\n");
                    continue;
                }

                if (difficulty < 1 || difficulty > 6) {
                    printf("Difficulty must be between 1 and 6. Try again.\n");
                    continue;
                }

                return (struct Player) { .type = AI, .difficulty = difficulty };
            }
        }

        printf("Invalid input. Try again.\n");
    }
}

struct Move get_move_from_user(struct State *state) {
    while (true) {
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

            struct Move move = (struct Move) { .moveType = HorizontalFence, .move.fenceMove = create_fence_move(row,
                                                                                                                col) };

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

            struct Move move = (struct Move) { .moveType = VerticalFence, .move.fenceMove = create_fence_move(row,
                                                                                                              col) };

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
void run_game() {
    struct Player p1 = get_player_info(1);
    struct Player p2 = get_player_info(2);

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

        make_move(&state, &move);
    }

    printf("\n====================\n");
    printf("   Player %d wins!\n", winningPlayer);
    printf("====================\n");
}
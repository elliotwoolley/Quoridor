#include <assert.h>
#include <string.h>

#include "search.h"
#include "move_generation.h"
#include "evaluate.h"

/**
 * @brief Initializes an array of moves to a default "no move" state.
 *
 * This function sets each element of the provided move list to a
 * default state indicating that no valid move is stored there.
 *
 * @param move_list Pointer to an array of `struct Move` that will be initialized.
 * @param length The number of moves (array size) to initialize.
 */
static inline void initialize_move_list(struct Move *move_list, int length) {
    for (int i = 0; i < length; i++) {
        move_list[i] = (struct Move) { .moveType = None, .score = INT32_MIN + 1 };
    }
}

/**
 * @brief Principal Variation Search function.
 *
 * Performs a principal variation search (a variant of alpha-beta search) from the given position.
 * This function attempts to refine the search window on the first move and re-searches if a move
 * falls between alpha and beta, improving the accuracy of the evaluated move.
 *
 * @param state Pointer to the current game state.
 * @param move The move being evaluated.
 * @param depth The current search depth remaining.
 * @param alpha Pointer to the current alpha value for the search window (score lower bound).
 * @param beta Pointer to the current beta value for the search window (score upper bound).
 * @param first_move Pointer to a boolean flag indicating if this is the first move considered at this node.
 *                   The first move uses a narrower search window, then a full re-search if necessary.
 * @param pv_move Pointer to the principal variation move array (the sequence of best moves found so far).
 * @param best_line Pointer to a move array that will store the best line (principal variation) found.
 *
 * @return True if the search should continue, or false if search has been pruned due to a beta cutoff.
 */
bool search(struct State *state, struct Move move, int depth, int *alpha, const int *beta,
            bool *first_move, struct Move *pv_move, struct Move best_line[]);

/**
 * @brief Conducts a search to find the best moves from a given state.
 *
 * This function performs a recursive alpha-beta search (with principal variation search enhancements)
 * to find the best line of moves starting from the given `state`. It stores the best variation found
 * in `best_line`. The search stops at terminal states or at a depth of 0, at which point the position
 * is evaluated.
 *
 * @param state The current game state.
 * @param depth The maximum search depth.
 * @param alpha The lower bound for the search window.
 * @param beta The upper bound for the search window.
 * @param pv_move A pointer to an array of moves representing the current principal variation.
 * @param best_line A pointer to an array of moves where the best sequence of moves will be stored.
 */
 void principal_variation_search(struct State state, int depth, int alpha, int beta, // NOLINT(*-no-recursion)
                                   struct Move *pv_move,
                                   struct Move best_line[]) {

    initialize_move_list(best_line, depth + 1);

    //  Terminal States...
    if (player_1_win_check(state) || player_2_win_check(state)) {
        best_line[0] = (struct Move) { .score = -(WINNING_SCORE + depth) };
        return;
    }
    if (depth == 0) {
        int evaluation = evaluate(state);
        best_line[0] = (struct Move) { .score = evaluation };
        return;
    }

    PawnMoves pawn_moves = generate_legal_pawn_moves(state);
    FenceMoves vertical_fence_moves = generate_pseudo_legal_vertical_fence_moves(state);
    FenceMoves horizontal_fence_moves = generate_pseudo_legal_horizontal_fence_moves(state);

    // first_move is used by principal variation search to determine if we do a
    // narrow window search or a full re-search on the first move.
    bool first_move = true;

    bool pv_move_is_legal =
            (pv_move->moveType == Pawn && pv_move->move.pawnMove & pawn_moves) ||
            (pv_move->moveType == VerticalFence && pv_move->move.fenceMove & vertical_fence_moves) ||
            (pv_move->moveType == HorizontalFence && pv_move->move.fenceMove & horizontal_fence_moves);

    if (pv_move_is_legal) {
        make_move(&state, pv_move);

        if (evaluate(state) != NO_PATH_FOUND) {
            struct Move child_best_move_list[depth];
            principal_variation_search(state, depth - 1, -beta, -alpha, pv_move + 1, child_best_move_list);

            int score = -child_best_move_list[0].score;

            if (score != NO_PATH_FOUND) {
                first_move = false;
                best_line[0] = *pv_move;
                best_line[0].score = score;
                memcpy(best_line + 1, child_best_move_list, sizeof(struct Move) * (depth - 1));
                if (score > alpha) {
                    alpha = score;
                }
            }

            if (score >= beta) {
                return;
            }

        }

        unmake_move(&state, pv_move);
    }

    pv_move++;

    while (pawn_moves != 0) {
        PawnMove pawn_move = 0b1 << __builtin_ctz(pawn_moves);
        pawn_moves &= pawn_moves - 1;
        make_pawn_move(&state, pawn_move);

        if (!search(&state, (struct Move) { .moveType = Pawn, .move.pawnMove = pawn_move },
                    depth, &alpha, &beta, &first_move, pv_move, best_line)) {
            return;
        }

        unmake_pawn_move(&state, pawn_move);
    }

    while (horizontal_fence_moves != 0) {
        FenceMove fence_move = square64(__builtin_ctzll(horizontal_fence_moves));
        horizontal_fence_moves &= horizontal_fence_moves - 1;
        make_horizontal_fence_move(&state, fence_move);

        if (evaluate(state) == NO_PATH_FOUND) {
            unmake_horizontal_fence_move(&state, fence_move);
            continue;
        }

        if (!search(&state, (struct Move) { .moveType = HorizontalFence, .move.fenceMove = fence_move },
                    depth, &alpha, &beta, &first_move, pv_move, best_line)) {
            return;
        }

        unmake_horizontal_fence_move(&state, fence_move);
    }

    while (vertical_fence_moves != 0) {
        FenceMove fence_move = square64(__builtin_ctzll(vertical_fence_moves));
        vertical_fence_moves &= vertical_fence_moves - 1;
        make_vertical_fence_move(&state, fence_move);

        if (evaluate(state) == NO_PATH_FOUND) {
            unmake_vertical_fence_move(&state, fence_move);
            continue;
        }

        if (!search(&state, (struct Move) { .moveType = VerticalFence, .move.fenceMove = fence_move },
                    depth, &alpha, &beta, &first_move, pv_move, best_line)) {
            return;
        }

        unmake_vertical_fence_move(&state, fence_move);
    }
}

/**
 * @brief Principal Variation Search helper function.
 *
 * This function checks if a given move can improve the current principal variation
 * and, if so, updates the best line accordingly. It may perform a re-search if the
 * move’s score falls between alpha and beta, in order to refine the score.
 *
 * @param state Pointer to the current game state.
 * @param move The current move being evaluated.
 * @param depth The current search depth remaining.
 * @param alpha Pointer to the current alpha value (score lower bound).
 * @param beta Pointer to the current beta value (score upper bound).
 * @param first_move Pointer to a boolean that indicates if this is the first move
 *                   at this node (used for principal variation logic).
 * @param pv_move Pointer to the move array representing the principal variation after this move.
 * @param best_line Pointer to the move array where the best variation found so far is stored.
 *
 * @return True if the search should continue, or false if a beta cutoff (pruning) occurred.
 */
bool search(struct State *state, const struct Move move, const int depth, int *alpha, // NOLINT(*-no-recursion)
            const int *beta, bool *first_move, struct Move *pv_move, struct Move *best_line) {

    struct Move child_line[depth];

    if (*first_move) {
        principal_variation_search(*state, depth - 1, -(*alpha) - 1, -(*alpha), pv_move, child_line);
        *first_move = false;
    } else {
        principal_variation_search(*state, depth - 1, -(*beta), -(*alpha), pv_move, child_line);
    }
    int score = -child_line[0].score;

    // If we found a move that beats alpha but is still less than beta,
    // we need a full re-search.
    if (score > *alpha && score < *beta) {
        principal_variation_search(*state, depth - 1, -(*beta), -(*alpha), pv_move, child_line);
        score = -child_line[0].score;
    }

    // If this move improves our best score or if no best move is chosen yet.
    if (score > *alpha || best_line[0].moveType == None) {
        *alpha = score;
        best_line[0].score = score;
        best_line[0].moveType = move.moveType;
        best_line[0].move = move.move;
        memcpy(best_line + 1, child_line, sizeof(struct Move) * depth);
    }

    if (score >= *beta) {
        return false;
    }

    return true;
}

struct Move get_best_move(struct State state, const int depth) {
    assert(depth > 0);

    struct Move empty_move_list[depth];
    struct Move best_move_list[depth + 1];

    for (int i = 0; i < depth; i++) {
        empty_move_list[i] = (struct Move) { .moveType = None, .score = INT32_MIN + 1 };
    }

    principal_variation_search(state, depth, INT32_MIN + 1, INT32_MAX, empty_move_list, best_move_list);

    return best_move_list[0];
}

/**
 * @brief Perform iterative deepening search to find the best move.
 *
 * This function uses iterative deepening: it searches at increasing depths up to the specified
 * `depth`. The best move line found at each iteration is stored and then used as a basis for
 * deeper searches, improving move ordering.
 *
 * @param state The current game state.
 * @param depth The maximum search depth to reach.
 *
 * @return The best move determined after searching up to the specified depth.
 */
struct Move get_best_move_iterative_deepening(struct State state, const int depth) {
    assert(depth > 0);

    struct Move current_line[depth];
    struct Move best_line[depth + 1]; // Initialized and populated by principal_variation_search

    initialize_move_list(current_line, depth);

    for (int d = 0; d < depth; d++) {
        principal_variation_search(state, d + 1, INT32_MIN + 1, INT32_MAX, current_line, best_line);
        memcpy(current_line, best_line, sizeof(struct Move) * (d + 1));
    }

    return current_line[0];
}
#ifndef QUORIDOR_MOVE_GENERATION_H
#define QUORIDOR_MOVE_GENERATION_H

#include "state.h"

/**
 * Returns a bitmask of pseudo-legal vertical fence moves for the current player.
 * If no fences are available, an empty bitmask is returned. Moves are only
 * pseudo-legal as it does not check if there is a path for all players.
 *
 * @param state The game state.
 * @return A bitmask (`FenceMoves`) of valid vertical fence positions.
 */
FenceMoves generate_pseudo_legal_vertical_fence_moves(struct State state);

/**
 * Returns a bitmask of pseudo-legal horizontal fence moves for the current player.
 * If no fences are available, an empty bitmask is returned. Moves are only
 * pseudo-legal as it does not check if there is a path for all players.
 *
 * @param state The game state.
 * @return A bitmask (`FenceMoves`) of valid horizontal fence positions.
 */
FenceMoves generate_pseudo_legal_horizontal_fence_moves(struct State state);

/**
 * @param state The game state.
 * @return A bitmask (`PawnMoves`) of valid pawn moves (`PawnMove`).
 */
PawnMoves generate_legal_pawn_moves(struct State state);


#endif //QUORIDOR_MOVE_GENERATION_H

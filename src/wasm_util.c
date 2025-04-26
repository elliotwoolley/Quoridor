#include <stdlib.h>
#include "wasm_util.h"

struct State *get_state_ptr() {
    struct State *state_ptr = malloc(sizeof(struct State));

    if (state_ptr == NULL) {
        exit(1);  // malloc failed, should probably log this somehow.
    }

    return state_ptr;
}

struct Move *get_move_ptr() {
    struct Move *move_ptr = malloc(sizeof(struct Move));

    if (move_ptr == NULL) {
        exit(1);  // malloc failed, should probably log this somehow.
    }

    return move_ptr;
}

struct State *new_state_ptr() {
    struct State *state_ptr = malloc(sizeof(struct State));

    if (state_ptr == NULL) {
        exit(1);  // malloc failed, should probably log this somehow.
    }

    *state_ptr = (struct State) {
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
    return state_ptr;
}

#ifndef QUORIDOR_WASMUTIL_H
#define QUORIDOR_WASMUTIL_H

#include "state.h"
#include "move.h"

// rename to alloc?
struct State *get_state_ptr();

struct Move *get_move_ptr();

struct State *new_state_ptr();

#endif //QUORIDOR_WASMUTIL_H

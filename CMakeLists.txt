cmake_minimum_required(VERSION 3.10)
project(Quoridor C)

set(CMAKE_C_STANDARD 17)

add_executable(Quoridor
        src/c/main.c
        src/c/move_generation.c
        src/c/move_generation.h
        src/c/state.h
        src/c/evaluate.c
        src/c/evaluate.h
        src/c/state.c
        src/c/move.c
        src/c/move.h
        src/c/search.c
        src/c/search.h
        src/c/bitboards.c
        src/c/bitboards.h
        src/c/game_manager.h
        src/c/game_manager.c
        src/c/wasm_util.h
        src/c/wasm_util.c
)

if (DEFINED EMSCRIPTEN)
    set(EXPORTED_FUNCTIONS
            _malloc
            _print_state
            _free
            _get_state_ptr
            _get_move_ptr
            _new_state_ptr
            _make_move
            _unmake_move
            _generate_legal_pawn_moves
            _generate_fully_legal_vertical_fence_moves
            _generate_fully_legal_horizontal_fence_moves
            _get_best_move_iterative_deepening
            _win_check
    )

    string(REPLACE ";" "," EXPORTED_FUNCS_CSV "${EXPORTED_FUNCTIONS}")

    target_link_options(Quoridor PRIVATE
            -sEXPORTED_FUNCTIONS=${EXPORTED_FUNCS_CSV}
            -sMODULARIZE=1
            -sEXPORT_ES6=1
            -sEXPORT_NAME=createModule
    )
endif ()

cmake_minimum_required(VERSION 3.10)
project(Quoridor)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -fsanitize=address,undefined -fstack-protector-all")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(Quoridor main.c
        move_generation.c
        move_generation.h
        state.h
        evaluate.c
        evaluate.h
        state.c
        move.c
        move.h
        search.c
        search.h
        bitboards.c
        bitboards.h
        game_manager.h
        game_manager.c
)



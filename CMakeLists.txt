cmake_minimum_required(VERSION 3.10)
project(Quoridor)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -fsanitize=address,undefined -fstack-protector-all")

add_executable(Quoridor
        src/main.c
        src/move_generation.c
        src/move_generation.h
        src/state.h
        src/evaluate.c
        src/evaluate.h
        src/state.c
        src/move.c
        src/move.h
        src/search.c
        src/search.h
        src/bitboards.c
        src/bitboards.h
        src/game_manager.h
        src/game_manager.c
)



cmake_minimum_required(VERSION 3.20)

project(cengine_game)


set(CENGINE_GAME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/game)

include_directories(${CENGINE_GAME_INCLUDE_DIR})

file(
    GLOB
    game_sourcefiles
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.c
)


add_executable(cengine_game_e ${game_sourcefiles})

target_compile_options(cengine_game_e PUBLIC -g -Wall)

target_link_libraries(cengine_game_e PUBLIC cengine_static GL glfw GLEW)

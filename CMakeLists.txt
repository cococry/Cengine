cmake_minimum_required(VERSION 3.20)

project(cengine)


set(CENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine/src)

include_directories(${CENGINE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine/vendor)

file(
    GLOB
    sourcefiles
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/core/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/vendor/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/vendor/stb_image/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/ecs/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/math/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/physics/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/platform/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/platform/opengl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/src/cengine/renderer/*.c
)


add_library(cengine SHARED ${sourcefiles})
add_library(cengine_static STATIC ${sourcefiles})

target_compile_options(cengine PUBLIC -g -Wall)
target_compile_options(cengine_static PUBLIC -g -Wall)

target_include_directories(
  cengine
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CENGINE_INCLUDE_DIR})



target_include_directories(
  cengine_static
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CENGINE_INCLUDE_DIR})


set(CENGINE_DEPS m GL dl glfw GLEW)
    
target_link_libraries(cengine PUBLIC ${CENGINE_DEPS})
target_link_libraries(cengine_static PUBLIC ${CENGINE_DEPS})
    
add_subdirectory(game)

    

cmake_minimum_required(VERSION 2.6)

SET(EXECUTABLE_OUTPUT_PATH ../bin)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -std=c++11")

SET(CMAKE_BUILD_TYPE Debug)

project(Pacman)

include_directories(
  include
  /usr/include/SDL2
)

file(
  GLOB_RECURSE
  src_files
  src/*
)

add_executable(
  pacman
  ${src_files}
)

link_directories(
  /usr/lib/x86_64-linux-gnu
)

target_link_libraries(
  pacman
  SDL2main
  SDL2
  SDL2_image
  SDL2_mixer
  SDL2_ttf
)


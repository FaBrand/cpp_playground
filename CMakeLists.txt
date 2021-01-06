# Central cmake list to add stuff

cmake_minimum_required(VERSION 3.5)
project(playground
  LANGUAGES CXX
  VERSION 42.0.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(utils)
add_subdirectory(strategyPatternPlayground)
add_subdirectory(boostAsioUdpPlayground)
add_subdirectory(boostSmlPlayground)
add_subdirectory(boostMsmPlayground)
#add_subdirectory(variant)
add_subdirectory(constrained_se3)
add_subdirectory(misc)
add_subdirectory(functional_programming)


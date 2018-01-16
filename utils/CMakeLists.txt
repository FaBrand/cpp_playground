cmake_minimum_required(VERSION 3.5)
project(utils
  LANGUAGES CXX
)

add_library(utils STATIC
    ./src/utils.cpp
)

add_library(utils::utils ALIAS utils)

target_include_directories(utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


cmake_minimum_required(VERSION 3.5)
project(strategy_pattern
  LANGUAGES CXX
)

add_library(utils
    ./src/concrete_strategy_a.cpp
    ./src/concrete_strategy_b.cpp
    ./src/client.cpp
)

add_executable(strategy_pattern ./src/unique_strategy.cpp)
target_link_libraries(strategy_pattern utils)

cmake_minimum_required(VERSION 3.5)
project(strategy_pattern
  LANGUAGES CXX
)

add_library(utils
    ./src/util_types/concrete_strategy_a.cpp
    ./src/util_types/concrete_strategy_b.cpp
    ./src/util_types/reference_client.cpp
    ./src/util_types/client.cpp
    ./src/util_types/free_strategies.cpp
    ./src/util_types/free_context.cpp
)
target_include_directories(utils
    PUBLIC
        ./include
)

add_executable(strategy_pattern ./src/unique_strategy.cpp)
target_link_libraries(strategy_pattern utils)

add_executable(strategy_pattern2 ./src/unique_strategy_v2.cpp)
target_link_libraries(strategy_pattern2 utils)

add_executable(reference_strategy_pattern ./src/reference_strategy.cpp)
target_link_libraries(reference_strategy_pattern  utils)

add_executable(free_strategy_pattern ./src/free_strategy.cpp)
target_link_libraries(free_strategy_pattern  utils)

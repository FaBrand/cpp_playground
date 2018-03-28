cmake_minimum_required(VERSION 3.5)
project(observer_pattern
  LANGUAGES CXX
)

add_library(observer_pattern_utils
    ./src/subject.cpp
    ./src/client.cpp
)


add_executable(observer_pattern ./src/observer_pattern.cpp)
target_link_libraries(observer_pattern
  PRIVATE
    observer_pattern_utils
)


cmake_minimum_required(VERSION 3.5)
project(adapter_pattern
  LANGUAGES CXX
)

add_library(adapter_pattern_utils
    ./src/client.cpp
    ./src/print_worker.cpp
    ./src/foreign_print_worker.cpp
    ./src/translator.cpp
)


add_executable(adapter_pattern ./src/base_adapter.cpp)
target_link_libraries(adapter_pattern
  PRIVATE
    adapter_pattern_utils
)

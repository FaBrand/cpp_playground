cmake_minimum_required(VERSION 3.5)
project(observer_pattern
  LANGUAGES CXX
)

# add_library(observer_pattern_utils
# )


add_executable(observer_pattern ./src/observer.cpp)
# target_link_libraries(adapter_pattern
#   PRIVATE
#     observer_pattern_utils
# )

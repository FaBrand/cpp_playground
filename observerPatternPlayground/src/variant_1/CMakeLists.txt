add_library(observer_pattern_utils_1
    subject.cpp
    client.cpp
)


add_executable(observer_pattern_1 observer_pattern.cpp)
target_link_libraries(observer_pattern_1
  PRIVATE
    observer_pattern_utils_1
)

add_library(observer_pattern_utils_2
    subject.cpp
    client.cpp
)


add_executable(observer_pattern_2 observer_pattern.cpp)
target_link_libraries(observer_pattern_2
  PRIVATE
    observer_pattern_utils_2
)

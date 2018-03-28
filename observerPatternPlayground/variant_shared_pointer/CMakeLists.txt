add_library(observer_pattern_utils_shared_pointer
    subject.cpp
    client.cpp
)


add_executable(observer_pattern_shared_pointer observer_pattern.cpp)
target_link_libraries(observer_pattern_shared_pointer
  PRIVATE
    observer_pattern_utils_shared_pointer
)

add_library(observer_pattern_utils_raw_pointer
    subject.cpp
    concrete_observer.cpp
)


add_executable(observer_pattern_raw_pointer observer_pattern.cpp)
target_link_libraries(observer_pattern_raw_pointer
  PRIVATE
    observer_pattern_utils_raw_pointer
)

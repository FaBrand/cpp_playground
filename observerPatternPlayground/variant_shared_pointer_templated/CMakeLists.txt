add_library(observer_pattern_utils_shared_pointer_tmp
    subject.cpp
    concrete_observer.cpp
    notification_helper.cpp
)


add_executable(observer_pattern_shared_pointer_tmp observer_pattern.cpp)
target_link_libraries(observer_pattern_shared_pointer_tmp
  PRIVATE
    observer_pattern_utils_shared_pointer_tmp
)

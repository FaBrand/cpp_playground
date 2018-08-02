cmake_minimum_required(VERSION 3.5)
project(ModelViewControllerPlayground
  LANGUAGES CXX
)

add_executable(${PROJECT_NAME}
    ./src/mvc_main.cpp
    ./src/model.cpp
    ./src/view.cpp
    ./src/controller.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ./include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        utils::utils
        signals_slots::signal
)


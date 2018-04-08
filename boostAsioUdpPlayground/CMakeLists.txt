cmake_minimum_required(VERSION 3.5)
project(boost_asio_udp
  LANGUAGES CXX
)


find_package(Boost 1.58 REQUIRED COMPONENTS system thread)


add_library(asio_utils
    ./src/udp_server.cpp
    ./src/udp_server_synchronous.cpp
    ./src/udp_server_dynamic_buffer.cpp
)

target_include_directories(asio_utils
    PUBLIC
        ./include
)
target_link_libraries(asio_utils
    PUBLIC
        Boost::system
        Boost::thread
        signals_slots::signal
)

add_executable(${PROJECT_NAME}
    ./src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    asio_utils
    Boost::system
)

add_executable(${PROJECT_NAME}_synchronous
    ./src/main.cpp
)

target_link_libraries(${PROJECT_NAME}_synchronous
    asio_utils
    Boost::system
)

add_executable(${PROJECT_NAME}_dynamic
    ./src/main_dynamic.cpp
)

target_link_libraries(${PROJECT_NAME}_dynamic
    asio_utils
    Boost::system
)

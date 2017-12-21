cmake_minimum_required(VERSION 3.5)
project(boost_asio_udp
  LANGUAGES CXX
)


find_package(Boost 1.58 REQUIRED COMPONENTS system thread)


add_library(asio_utils
    ./src/udp_server.cpp
)

target_include_directories(asio_utils
    PUBLIC
        ./include
)
target_link_libraries(asio_utils
    PUBLIC
        Boost::system
        Boost::thread
)

add_executable(${PROJECT_NAME}
    ./src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    asio_utils
    Boost::system
)

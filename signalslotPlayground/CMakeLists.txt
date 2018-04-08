cmake_minimum_required(VERSION 3.5)
project(signals_slots
  LANGUAGES CXX
)

add_library(signal INTERFACE)
target_include_directories(signal
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(signals_slots::signal ALIAS signal)


add_executable(sigSlot ./src/signal_slot.cpp)
target_link_libraries(sigSlot PUBLIC signal)

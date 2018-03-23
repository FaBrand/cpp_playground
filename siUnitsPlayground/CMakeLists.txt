cmake_minimum_required(VERSION 3.5)
project(si_units
  LANGUAGES CXX
)


add_executable(si_units ./src/dummy.cpp)
target_include_directories(si_units PUBLIC ./include)

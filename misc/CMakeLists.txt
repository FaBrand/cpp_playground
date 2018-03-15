cmake_minimum_required(VERSION 3.5)
project(miscellanious
  LANGUAGES CXX
)

add_executable(constructors ./src/constructors.cpp)

target_compile_options(constructors PRIVATE
    -Wno-unused-but-set-variable
    )

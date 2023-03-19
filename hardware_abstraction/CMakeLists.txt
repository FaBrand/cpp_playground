cmake_minimum_required(VERSION 3.5)
project(portexpander
  LANGUAGES CXX C
)

add_executable(portexpander portexpander.c PortExp.c PortExp_cfg.c)

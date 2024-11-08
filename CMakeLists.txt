cmake_minimum_required(VERSION 3.20)

project("transmitter" C CXX ASM)

include(cmake/avr-project.cmake)

add_executable(${PROJECT_NAME})
add_avr_target_properties(${PROJECT_NAME})
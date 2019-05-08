cmake_minimum_required(VERSION 3.13)
project(lab9 C)

set(CMAKE_C_STANDARD 99)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(lab9 main.c)
target_link_libraries(${PROJECT_NAME} pthread)
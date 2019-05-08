cmake_minimum_required(VERSION 3.13)
project(lab11 C)

set(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(lab11_Server Server.c)
add_executable(lab11_Client Client.c)
target_link_libraries(${PROJECT_NAME}_Client pthread)
target_link_libraries(${PROJECT_NAME}_Server pthread)
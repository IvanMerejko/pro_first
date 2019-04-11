cmake_minimum_required(VERSION 3.10)
project(PRO C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread")

add_executable(PRO main.c queue.h queue.c threads.h threads.c)
cmake_minimum_required(VERSION 3.2)
project(Lab_2_final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Lab_2_final ${SOURCE_FILES} Scheduler.h Scheduler.cpp)
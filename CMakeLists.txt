cmake_minimum_required(VERSION 3.3)
project(Lecture_Cpp)

# the application needs to be executed in the runtime directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp LectureDemo.cpp LectureDemo.h )
add_executable(Lecture_Cpp ${SOURCE_FILES})


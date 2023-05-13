# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

project ("parallel-computing-course")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost COMPONENTS asio)

# Add the Boost include directories
include_directories(${Boost_INCLUDE_DIRS})

# Include sub-projects.
add_subdirectory ("lab1")
add_subdirectory ("lab2")
add_subdirectory ("lab3")
add_subdirectory ("lab4")

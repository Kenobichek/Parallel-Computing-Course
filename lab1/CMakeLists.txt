# CMakeList.txt : CMake project for lab1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
file(GLOB lab1_files "*.h" "*.cpp")

add_executable(lab1 ${lab1_files})
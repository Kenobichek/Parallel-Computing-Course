# CMakeList.txt : CMake project for lab3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
file(GLOB lab3_files "*.h" "*.cpp")

add_executable(lab3 ${lab3_files})
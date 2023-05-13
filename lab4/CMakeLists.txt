# CMakeList.txt : CMake project for lab4, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
file(GLOB client_files "client.cpp")
file(GLOB server_files "server.cpp")

add_executable(lab4_client ${client_files})
add_executable(lab4_server ${server_files})

target_link_libraries(lab4_client PRIVATE ${Boost_LIBRARIES})
target_link_libraries(lab4_server PRIVATE ${Boost_LIBRARIES})
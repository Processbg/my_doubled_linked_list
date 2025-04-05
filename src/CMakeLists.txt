
# 1: Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.10)
# 2: Create a project named linkedList
project(linkedList)
# 7: Set the project version number as 1.0 in the above project command
project(linkedList VERSION 1.0)
# 6: Set the variable CMAKE_CXX_STANDARD to 11
#         and the variable CMAKE_CXX_STANDARD_REQUIRED to True
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Add debug symbols for gdb
set(CMAKE_BUILD_TYPE Debug)
# 3: Add an executable called BinaryTree to the project
add_executable(linkedList main.cpp)

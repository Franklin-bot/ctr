cmake_minimum_required(VERSION 3.23)

project(ctr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src/common)
add_subdirectory(src/parser)
add_subdirectory(test/parser)

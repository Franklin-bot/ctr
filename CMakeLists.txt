cmake_minimum_required(VERSION 3.23)

project(ctr)

include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory(src/common)
add_subdirectory(src/parser)
add_subdirectory(test/parser)

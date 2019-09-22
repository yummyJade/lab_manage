cmake_minimum_required(VERSION 3.14)
project(myc1)

include_directories(src/include)
include_directories(src/main)

aux_source_directory(src/main dir_srcs)
set(CMAKE_CXX_STANDARD 14)
set(MYC1 ${dir_srcs})
add_executable(myc1 src/main/main.cpp)

add_subdirectory(Google_tests)
add_subdirectory(src/include)
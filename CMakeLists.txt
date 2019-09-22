cmake_minimum_required(VERSION 3.14)
project(myc1)
#c++14 required
set(CMAKE_CXX_STANDARD 14)

include_directories(include)

aux_source_directory(src dir_srcs)

# sources to be compiled
set(MYC1 ${dir_srcs})
add_executable(myc1 test/main.cpp )
cmake_minimum_required(VERSION 3.14)
project(myc1)
#c++14 required
set(CMAKE_CXX_STANDARD 14)

include_directories(include)

aux_source_directory(src dir_srcs)

# sources to be compiled
set(MYC1 ${dir_srcs})
#add_executable(myc1 test/main.cpp)

# yu的执行函数
set(SOURCE_FILES test/menuTest.cpp)
add_executable(myc1 ${SOURCE_FILES})

## Dev-Tan的执行函数
#set(SOURCE_FILES dev-Tan/main.cpp thirdParty/sqlite3/sqlite3.c)
#add_executable(myc1 ${SOURCE_FILES})


# 下面这些是测试的,别删
#add_executable(myc1 dev-Tan/main.cpp)
#add_subdirectory(include)
#add_subdirectory(Google_tests)
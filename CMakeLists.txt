cmake_minimum_required(VERSION 3.14)
project(myc1)

set(CMAKE_CXX_STANDARD 14)

# 头文件配置
SET(HEADER_FILES_PATH
        include
        )
INCLUDE_DIRECTORIES(${HEADER_FILES_PATH})
#include_directories(include)

# src配置
aux_source_directory(src dir_srcs)

set(MYC1 ${dir_srcs})
#add_executable(myc1 test/main.cpp)

## 测试
#set(SOURCE_FILES dev-test/menuTest.cpp)
#add_executable(myc1 ${SOURCE_FILES})

## yu的执行函数
#set(SOURCE_FILES yu/Usertest.cpp)
#add_executable(myc1 ${SOURCE_FILES})

# Dev-Tan的执行函数
set(SOURCE_FILES dev-Tan/main.cpp)
add_executable(myc1 ${SOURCE_FILES})
add_subdirectory(include)
add_subdirectory(test)



cmake_minimum_required(VERSION 3.8)

set(SOURCE_FILES_TESTS UserTest.cpp) 
#set(SOURCE_FILES_TESTS Library.cpp) #数据导入
add_executable(Catch_tests_run ${SOURCE_FILES_TESTS})
target_link_libraries(Catch_tests_run Include)

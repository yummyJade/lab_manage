cmake_minimum_required(VERSION 3.8)

#set(SOURCE_FILES_TESTS Library.cpp) # 初始化数据库 数据
#set(SOURCE_FILES_TESTS UserTest.cpp) # 增加一些逾期的数据
set(SOURCE_FILES_TESTS DateTest.cpp) #增加一个预约未娶的借阅情况
add_executable(Catch_tests_run ${SOURCE_FILES_TESTS})
target_link_libraries(Catch_tests_run Include)

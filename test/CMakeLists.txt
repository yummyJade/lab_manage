
set(SOURCE_FILES_TESTS DateTest.cpp)
add_executable(Catch_tests_run ${SOURCE_FILES_TESTS})
target_link_libraries(Catch_tests_run Include)
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "libm.h"
#include "../src/libcpp.h"

TEST_CASE("导入数据", "[Database]") {
	// 清表

	// 导入图书
	Book::importBooksService("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv");
	// 导入用户
	User::importUsers("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\users.csv");



}




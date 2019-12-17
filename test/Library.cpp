#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <iomanip>
#include<string>
#include<windows.h>
#include <vector>
#include "libm.h"
#include "../src/libcpp.h"
#include "../src/libcpp.h"
#include "../linkDatebase/datacpp.h"
#include "../yu/Verify.h"
#include "../yu/Verify.cpp"
#include "../src/core/Input.cpp"
using namespace std;


TEST_CASE("导入数据", "[Database]") {
	// 链表数据库没有提供删除操作

	// 清空书表
	DbAdapter db1("Book");
	db1.clearTable();
	CHECK(db1.searchAll().empty()); //清空表后表的数据为空

	// 导入图书
	Book::importBooksService("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv");

	CHECK(db1.searchAll().size()==12); //清空表后表的数据为空

	// 清空用户表
	DbAdapter db2= DbAdapter("User");
	db2.clearTable();
	CHECK(db2.searchAll().empty()); //清空表后表的数据为空
	// 导入用户
	User::importUsers("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\users.csv");
	CHECK(db2.searchAll().size() == 3); //清空表后表的数据为空



}




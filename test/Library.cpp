#define CATCH_CONFIG_MAIN
#include "../src/libcpp.h"
#include "catch.hpp"
#include "libm.h"


TEST_CASE("��������", "[Database]") {
	// ���

	// ����ͼ��
	Book::importBooksService("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv");
	// �����û�
	User::importUsers("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\users.csv");



}




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


TEST_CASE("��������", "[Database]") {
	// �������ݿ�û���ṩɾ������

	// ������
	DbAdapter db1("Book");
	db1.clearTable();
	CHECK(db1.searchAll().empty()); //��ձ��������Ϊ��

	// ����ͼ��
	Book::importBooksService("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv");

	CHECK(db1.searchAll().size()==12); //��ձ��������Ϊ��

	// ����û���
	DbAdapter db2= DbAdapter("User");
	db2.clearTable();
	CHECK(db2.searchAll().empty()); //��ձ��������Ϊ��
	// �����û�
	User::importUsers("E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\users.csv");
	CHECK(db2.searchAll().size() == 3); //��ձ��������Ϊ��



}




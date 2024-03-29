#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "statu.h"
using namespace std;



TEST_CASE( "测试Statu", "[statu]" ) {
	//测试是否会因为外部vector改变而改变

	Statu * statu = Statu::getInstance();
	

	SECTION("创建第一张表"){
		statu->clear();
		string name = "测试表1";
		vector<string> col_name(3, "字段");
		col_name[0] += "1";
		col_name[1] += "2";
		col_name[2] += "3";
		REQUIRE(col_name[2] == "字段3");
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'F');
		vector<char> isUnique(3, 'F');

		REQUIRE(statu->createTable(name, col_name, size, isHash, isUnique) == 0);
		string str = "测试表1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "字段1");
		REQUIRE(statu->table_col_name[0][1] == "字段2");
		REQUIRE(statu->table_col_name[0][2] == "字段3");

		
	}

	SECTION("第二次打开"){
		string str = "测试表1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "字段1");
		REQUIRE(statu->table_col_name[0][1] == "字段2");
		REQUIRE(statu->table_col_name[0][2] == "字段3");
	}


	SECTION("改了改vector看是否影响，并创建第二张表"){
		string name = "测试表2";
		vector<string> col_name(4, "字段");
		col_name[0] += "4";
		col_name[1] += "5";
		col_name[2] += "6";
		col_name[3] += "7";
		vector<ll> size(4, 100);
		vector<char> isHash(4, 'F');
		vector<char> isUnique(4, 'F');

		REQUIRE(statu->createTable(name, col_name, size, isHash, isUnique) == 0);
		
		col_name[0] = "字段10";
		REQUIRE(statu->table_name[1] == "测试表2");
		REQUIRE(statu->table_col_num[1] == 4);
		
		REQUIRE(statu->table_col_name[1][0] == "字段4");
		REQUIRE(statu->table_col_name[1][1] == "字段5");
		REQUIRE(statu->table_col_name[1][2] == "字段6");
		REQUIRE(statu->table_col_name[1][3] == "字段7");
	}

	SECTION("测试获取位置"){
		REQUIRE(statu->getIdx("测试表1") == 0);
		REQUIRE(statu->getIdx("测试表2") == 1);
	}

	SECTION("测试清空数据"){
		statu->clear();
		REQUIRE(statu->table_number == 0);
	}
	// SECTION("first"){
	// 	REQUIRE( Factorial(0) == 1);
	// }
	// SECTION("second"){
	// 	REQUIRE( Factorial(1) == 1 );
	// 	REQUIRE( Factorial(2) == 2 );
	// 	REQUIRE( Factorial(3) == 6 );
	// 	REQUIRE( Factorial(10) == 3628800 );
	// 	REQUIRE( Factorial(0) == 1);
	// }
	// SECTION("third"){
	// 	REQUIRE( Factorial(0) == 1);
	// }
}


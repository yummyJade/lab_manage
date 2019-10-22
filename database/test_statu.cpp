#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "statu.h"

using namespace std;



TEST_CASE( "����Statu", "[statu]" ) {
//�����Ƿ����Ϊ�ⲿvector�ı���ı�

Statu *statu = Statu::getInstance();


SECTION("������һ�ű�"){
statu->

clear();

string name = "���Ա�1";
vector<string> col_name(3, "�ֶ�");
col_name[0] += "1";
col_name[1] += "2";
col_name[2] += "3";
REQUIRE(col_name[2]
== "�ֶ�3");
vector<ll> size(3, 100);
vector<char> isHash(3, 'F');
vector<char> isUnique(3, 'F');

REQUIRE(statu
->
createTable(name, col_name, size, isHash, isUnique
) == 0);
string str = "���Ա�1";
REQUIRE(statu
->table_name[0] == str);
REQUIRE(statu
->table_col_num[0] == 3);

REQUIRE(statu
->table_col_name[0][0] == "�ֶ�1");
REQUIRE(statu
->table_col_name[0][1] == "�ֶ�2");
REQUIRE(statu
->table_col_name[0][2] == "�ֶ�3");


}

SECTION("�ڶ��δ�"){
string str = "���Ա�1";
REQUIRE(statu
->table_name[0] == str);
REQUIRE(statu
->table_col_num[0] == 3);

REQUIRE(statu
->table_col_name[0][0] == "�ֶ�1");
REQUIRE(statu
->table_col_name[0][1] == "�ֶ�2");
REQUIRE(statu
->table_col_name[0][2] == "�ֶ�3");
}


SECTION("���˸�vector���Ƿ�Ӱ�죬�������ڶ��ű�"){
string name = "���Ա�2";
vector<string> col_name(4, "�ֶ�");
col_name[0] += "4";
col_name[1] += "5";
col_name[2] += "6";
col_name[3] += "7";
vector<ll> size(4, 100);
vector<char> isHash(4, 'F');
vector<char> isUnique(4, 'F');

REQUIRE(statu
->
createTable(name, col_name, size, isHash, isUnique
) == 0);

col_name[0] = "�ֶ�10";
REQUIRE(statu
->table_name[1] == "���Ա�2");
REQUIRE(statu
->table_col_num[1] == 4);

REQUIRE(statu
->table_col_name[1][0] == "�ֶ�4");
REQUIRE(statu
->table_col_name[1][1] == "�ֶ�5");
REQUIRE(statu
->table_col_name[1][2] == "�ֶ�6");
REQUIRE(statu
->table_col_name[1][3] == "�ֶ�7");
}

SECTION("���Ի�ȡλ��"){
REQUIRE(statu
->getIdx("���Ա�1") == 0);
REQUIRE(statu
->getIdx("���Ա�2") == 1);
}

SECTION("�����������"){
statu->

clear();

REQUIRE(statu
->table_number == 0);
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


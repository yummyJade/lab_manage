#pragma once

#include <string>
#include "../core/Time.h"
#include <iostream>
using namespace std;

class Book
{

private:
	char type;		//书所属类别A--V，期刊为0
	bool isLend = true;		//是否可借
	int price;		//书本价格
	int id;
	int sum;        //书本数量
	string name;		//书名
	string author;		//作者
	string isbn;		//ISBN



							

public:
	Book();
	~Book();
	//打印书本类型
	string printType();
	string printIsLend();

	void setBookInfo(string namet,string authort,string isbnt,char typet,double pricet);
	//打印书本基本信息
	void printBookInfo() ;
	
};


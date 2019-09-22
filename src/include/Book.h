#pragma once

#include <string>
#include "SimpleTime.h"
#include <iostream>
using namespace std;


class Book
{

private:
	char type;		//���������A--V���ڿ�Ϊ0
	bool isLend = true;		//�Ƿ�ɽ�
	int price;		//�鱾�۸�
	int id;
	string name;		//����
	string author;		//����
	string isbn;		//ISBN


public:
	Book();
	~Book();
	//��ӡ�鱾����
	string printType();
	string printIsLend();

	void setBookInfo(string namet,string authort,string isbnt,char typet,double pricet);
	//��ӡ�鱾������Ϣ
	void printBookInfo() ;
	
};


#pragma once

#include <string>
#include "Time.h"
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
	string printType() {
		string temp;
		switch (type)
		{
		case 'A':temp = "���˼���塢�������塢ë��˼�롢��Сƽ����"; break;
		case 'B':temp = "��ѧ���ڽ�"; break;
		case 'C':temp = "����ѧ����"; break;
		case 'D':temp = "���Ρ�����"; break;
		case 'E':temp = "����"; break;
		case 'F':temp = "����"; break;
		case 'G':temp = "�Ļ�����ѧ������������"; break;
		case 'H':temp = "���ԡ�����"; break;
		case 'I':temp = "��ѧ"; break;
		case 'J':temp = "����"; break;
		case 'K':temp = "��ʷ������"; break;
		case 'N':temp = "��Ȼ��ѧ����"; break;
		case 'O':temp = "�����ѧ�ͻ�ѧ"; break;
		case 'P':temp = "����ѧ�������ѧ"; break;
		case 'Q':temp = "�����ѧ"; break;
		case 'R':temp = "ҽҩ������"; break;
		case 'S':temp = "ũҵ��ѧ"; break;
		case 'T':temp = "��ҵ����"; break;
		case 'U':temp = "��ͨ����"; break;
		case 'V':temp = "���ա�����"; break;
		case 'X':temp = "������ѧ����ȫ��ѧ"; break;
		case 'Z':temp = "�ۺ���ͼ��"; break;

		default:temp = "�ڿ���־";
			break;
		}
		return temp;
	}
	string printIsLend() {
		if (isLend) {
			return "�ɽ�";
		}
		else if (!isLend) {
			return "���ɽ�";
		}
	}

	void setBookInfo(string namet,string authort,string isbnt,char typet,double pricet) {
		name = namet;
		author = authort;
		isbn = isbnt;
		type = typet;
		price = pricet;
	}
	//��ӡ�鱾������Ϣ
	void printBookInfo() {
		cout << id << name << author << isbn << type;
	}

	void setId(int value) {
		id = value;
	}
	int getId() {
		return id;
	}
	
	int printSize() {
		return sizeof(Book);
	}
	
};


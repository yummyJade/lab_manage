#include <string>
#pragma once
using namespace std;
enum status { Teacher, Graduate,Undergraduate, Senior, Middle, Primary, Kindergarten};		//��Ȩ˼��ö��

class User
{
private:
	int id;
	int jobNum;		//����
	int maxLend;		//�ɽ��������
	int maxTime;		//�ɽ���ʱ��
	status type;
	string pwd;		//password


public:
	User();
	~User();
	User(int idt, int jobNumt, int maxLendt, int maxTimet, string pwdt, status typet) {
		id = idt;
		jobNum = jobNumt;
		maxLend = maxLendt;
		maxTime = maxTimet;
		pwd = pwdt;

		switch (typet)
		{
		default:
			break;
		}
	}

	//��ӡ�û���Ϣ
	void printUserInfo() {

	}
	
	bool canLendBook() {

	}
	void setId(int value) {
		id = value;
	}
	int getId() {
		return id;
	}
	void setJobNum(int jobNum) {

	}
};


#include <string>
#pragma once
using namespace std;
enum status { Teacher, Graduate,Undergraduate, Senior, Middle, Primary, Kindergarten};		//特权思想枚举

class User
{
private:
	int id;
	int jobNum;		//工号
	int maxLend;		//可借书的数量
	int maxTime;		//可借阅时间
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

	//打印用户信息
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


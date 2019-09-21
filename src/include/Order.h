#pragma once

#include "Time.h"
#include <iostream>
#include <string>
using namespace std;

class Order
{
private:
	bool IsRenew = true;		//�Ƿ������
	int debt = 0;		//Ƿ��(cent)
	int id;		//�������
	Time borrowTime;		//����ʱ��
	Time returnTime;		//����ʱ��
	

	

public:
	Order();
	~Order();

	void printSize() {
		cout << sizeof(Order);
	}
	Time getBorrowTime() {
		return borrowTime;
	}
	void setBorrowTime(Time value) {
		borrowTime = value;
	}
	Time getReturnTime() {
		return returnTime;
	}
	void setReturnTime(Time value) {
		returnTime = value;
	}
	double getDebt() {
		return debt;
	}
	void setDebt(double value) {
		debt = value;
	}
};


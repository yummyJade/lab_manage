#pragma once

#include "Time.h"
#include <iostream>
#include <string>
using namespace std;

class Order
{
private:
	bool IsRenew = true;		//是否可续借
	int debt = 0;		//欠款(cent)
	int id;		//订单编号
	Time borrowTime;		//借书时间
	Time returnTime;		//还书时间
	

	

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


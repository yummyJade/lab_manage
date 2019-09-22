#pragma once
#include "SimpleTime.h"
#include <string>

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

};


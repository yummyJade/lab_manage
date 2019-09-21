#pragma once
#include<string>
#include "Date.h"
using namespace std;


class Time :
	public Date
{
private:
	int hour;
	int min;
	int sec;

public:
	Time();
	~Time();
};


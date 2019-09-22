#include "../../include/core/Time.h"
#include <iostream>

using namespace std;



Time::Time() {}

Time::~Time()
{
}

void Time::print()
{
    cout << year << "年" << month << "月" << day << "日" <<hour << ":" << minute << ":" << second << endl;
}

Time::Time(int y, int m, int d, short hour, short minute, short second) : Date(y, m, d), hour(hour), minute(minute),
                                                                          second(second) {}




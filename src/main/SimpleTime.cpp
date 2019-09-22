#include "SimpleTime.h"
#include <iostream>

using namespace std;



SimpleTime::SimpleTime() {}

SimpleTime::~SimpleTime()
{
}

void SimpleTime::print()
{
    cout << year << "Äê" << month << "ÔÂ" << day << "ÈÕ" <<hour << ":" << minute << ":" << second << endl;
}

SimpleTime::SimpleTime(int y, int m, int d, short hour, short minute, short second) : Date(y, m, d), hour(hour), minute(minute),
                                                                                      second(second) {}




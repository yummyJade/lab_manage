#pragma once
#include "Date.h"
class Time :
        public Date
{
public:
    short hour;
    short minute;
    short second;

    Time();
    Time::~Time();
    Time(int y, int m, int d, short hour, short minute, short second);

    void print();
};


#pragma once
#include "Date.h"
class SimpleTime :
        public Date
{
public:
    short hour;
    short minute;
    short second;

    SimpleTime();
    ~SimpleTime();
    SimpleTime(int y, int m, int d, short hour, short minute, short second);

    void print();
};


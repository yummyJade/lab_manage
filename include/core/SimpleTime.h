#pragma once

#include "Date.h"

class SimpleTime {
public:
    short hour;
    short minute;
    short second;
    Date date;

    SimpleTime();

    ~SimpleTime();


    // 序列化函数
    std::string serialize();

    // 反序列化函数
    bool deSerialize(std::string info);

    void print();
};


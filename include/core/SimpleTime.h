#pragma once

#include "Date.h"

class SimpleTime {
public:
    short hour;
    short minute;
    short second;
    Date date;

    SimpleTime(short hour, short minute, short second, const Date &date);

    SimpleTime();

    ~SimpleTime();


    /**
     * 比较两个时间谁更靠后
     * @param another
     * @return 大于0,说明当前日期比another大,等于0时相等,小于0当前更小
     */
    int compare(SimpleTime another);

    /**
     * 和日期谁更靠后,即不考虑时分秒
     * @param another
     * @return 大于0,说明当前日期比another大,等于0时相等,小于0当前更小
     */
    int compare(Date another);

    // 序列化函数
    std::string serialize();

    // 反序列化函数
    bool deSerialize(std::string info);

    void print();

    // 返回当前时间
    static SimpleTime nowTime();

    // 转化为ll 类型
    long long toLLTime();

    // ll 转化为 SimpleTime
    static SimpleTime llTimeToSimpleTime(long long time);

};


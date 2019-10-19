#pragma once

#include <string>

class Date {
public:
    int year;
    int month;
    int day;

    Date();

    Date(int y, int m, int d);

    /**
     * 判断是否为闰年
     * @param year 年份
     * @return 该年是否为闰年
     */
    bool isLeapYear(int year);

    /**
     * 给当前对象增加指定天数
     * @param num
     */
    void addDay(int num);

    /**
     * 比较两个日期谁更靠后
     * @param another
     * @return 大于0,说明当前日期比another大,等于0时相等,小于0当前更小
     */
    int compare(Date another);

    /**
     * 返回 今天日期
     * @return
     */
    static Date today();

    // 序列化函数
    std::string serialize();

    // 反序列化函数
    bool deSerialize(std::string info);

    void print();

    int toInt();

    static Date intDate2Date(int date);
};


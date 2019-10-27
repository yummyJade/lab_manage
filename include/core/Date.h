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
     * @return 该年是否为闰年(true or false)
     */
    bool isLeapYear(int year);

    /**
     * 给当前对象增加指定天数
     * @param num
     */
    Date addDay(int num);

    /**
     * 比较两个日期的大小
     * @param another
     * @return 大于0,说明当前日期比another大,等于0时相等,小于0当前更小. (返回值为日期相差天数)
     */
    int compare(Date another);

    /**
     * 获得今天日期
     * @return 今天日期的Date对象
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


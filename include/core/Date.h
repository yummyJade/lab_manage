#pragma once

class Date {
public:
    int year;
    int month;
    int day;

    Date();

    Date(int y, int m, int d);

    bool isLeapYear(int year);

    void print();

    void addDay(int num);
};


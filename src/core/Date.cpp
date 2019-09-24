#include "../../include/core/Date.h"
#include <iostream>
#include <string>
using namespace std;

Date::Date() {
    year = 0;
    month = 0;
    day = 0;
}

Date::Date(int y, int m, int d) {
    year = y;
    month = m;
    day = d;
}

bool Date::isLeapYear(int year) {
    return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

void Date::print() {
    cout << this->serialize();
}

void Date::addDay(int num) {
    int numOfYear = isLeapYear(year) ? 366 : 365;
    while (num > numOfYear) {
        year++;
        num -= numOfYear;
        numOfYear = isLeapYear(year) ? 366 : 365;
    }

    int dayOfMonth[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    dayOfMonth[2] = isLeapYear(year) ? 29 : 28;

    while (num > dayOfMonth[month]) {
        num -= dayOfMonth[month];
        month++;
        if (month == 13) {
            year++;
            month = 1;
            dayOfMonth[2] = isLeapYear(year) ? 29 : 28;
        }
    }

    while (num > 0) {
        num--;
        day++;
        if (day > dayOfMonth[month]) {
            month++;
            if (month == 13) {
                year++;
                month = 1;
                dayOfMonth[2] = isLeapYear(year) ? 29 : 28;
            }

            day = 1;
        }
    }
}

int Date::compare(Date another) {
    if (this->year != another.year) {
        return this->year - another.year;
    } else if (this->month != another.month) {
        return this->month - another.month;
    } else
        return this->day - another.day;
}

string Date::serialize() {
    string result = "";
    result += to_string(this->year) + "-";
    result += to_string(this->month) + "-";
    result += to_string(this->day);

    return result;
}

bool Date::deSerialize(std::string info) {
    // todo:这个反序列化写的很丑,谁有时间来改了它吧
    int num = 0;
    int i = 0;
    while (i < info.size()) {
        if (info[i] != '-') {
            num *= 10;
            num += info[i] - '0';
            i++;
        } else {
            this->year = num;
            num = 0;
            i++;
            break;
        }
    }
    while (i < info.size()) {
        if (info[i] != '-') {
            num *= 10;
            num += info[i] - '0';
            i++;
        } else {
            this->month = num;
            num = 0;
            i++;
            break;
        }
    }
    while (i < info.size()) {
        if (info[i] != '-') {
            num *= 10;
            num += info[i] - '0';
            i++;
        } else {
            this->day = num;
            i++;
            break;
        }
    }
    this->day = num;

    return true;
}



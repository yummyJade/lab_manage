#include "../../include/core/Date.h"
#include <iostream>
#include <string>
#include <ctime>
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

Date Date::addDay(int num) {
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
    return *this;
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
    // todo:��������л�д�ĺܳ�,˭��ʱ������������
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

Date Date::today() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return Date(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
}


int Date::toInt() {
    string time = "";
    time += this->year;
    if (this->month < 10) {
        time += "0";
    }
    time += this->month;
    if (this->day < 10) {
        time += "0";
    }
    time += this->day;
    return atoi(time.c_str());
}


Date Date::intDate2Date(int date) {
    string time_str = to_string(date);
    if (time_str.length() < 7) {
        return Date();
    }
//    cout<<"str is "<<time_str<<endl;
    return Date(atoi(time_str.substr(0, 3).c_str()), atoi(time_str.substr(4, 5).c_str()),
                atoi(time_str.substr(6, 7).c_str()));
}



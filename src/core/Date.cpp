#include "../../include/core/Date.h"
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
    cout << year << "Äê" << month << "ÔÂ" << day << "ÈÕ" << endl;
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

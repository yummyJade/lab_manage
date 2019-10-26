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
	struct tm t1 = { 0 };
	struct tm t2 = { 0 };
	double seconds;

	t1.tm_year = this->year - 1900; t1.tm_mon = this->month-1; t1.tm_mday = this->day;//现在时间2016,11,21  
	t2.tm_year = another.year - 1900; t2.tm_mon = another.month-1; t2.tm_mday = another.day;//高考时间2017,6,7  

	seconds = difftime(mktime(&t1), mktime(&t2));//转换结构体为time_t,利用difftime,计算时间差  
	int compareResult = seconds / 86400;
	return compareResult;

    /*if (this->year != another.year) {
        return this->year - another.year;
    } else if (this->month != another.month) {
        return this->month - another.month;
    } else
        return this->day - another.day;*/
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

Date Date::today() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return Date(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
}


int Date::toInt() {
    string time = "";
    time += to_string(this->year);
    if (this->month < 10) {
        time += "0";
    }
    time += to_string(this->month);
    if (this->day < 10) {
        time += "0";
    }
    time += to_string(this->day);
    return atoi(time.c_str());
}


Date Date::intDate2Date(int date) {
    string time_str = to_string(date);
    if (time_str.length() < 7) {
        return Date();
    }
//    cout<<"str is "<<time_str<<endl;
    return Date(atoi(time_str.substr(0, 4).c_str()), atoi(time_str.substr(4, 2).c_str()),
                atoi(time_str.substr(6, 2).c_str()));
}



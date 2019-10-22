#include "core/SimpleTime.h"
#include <iostream>
#include <string>
#include <ctime>
using namespace std;



SimpleTime::SimpleTime() {}

SimpleTime::~SimpleTime()
{
}

void SimpleTime::print()
{
    cout << this->serialize();
}


std::string SimpleTime::serialize() {
    string info = this->date.serialize() + " ";
    info += to_string(hour) + ":";
    info += to_string(minute) + ":";
    info += to_string(second);
    return info;
}

bool SimpleTime::deSerialize(std::string info) {
    // todo:这里的下标可能有一点点小问题,我没测
    string dateInfo = info.substr(0, info.find(" "));
    this->date.deSerialize(dateInfo);
    info = info.substr(info.find(" ") + 1, info.size());
    int num = 0;
    int i = 0;
    while (i < info.size()) {
        if (info[i] != ':') {
            num *= 10;
            num += info[i] - '0';
            i++;
        } else {
            this->hour = num;
            num = 0;
            i++;
            break;
        }
    }
    while (i < info.size()) {
        if (info[i] != ':') {
            num *= 10;
            num += info[i] - '0';
            i++;
        } else {
            this->minute = num;
            num = 0;
            i++;
            break;
        }
    }
    while (i <= info.size()) {
        if (info[i] != '-') {
            num *= 10;
            num += info[i] - '0';
            i++;
        } else {
            this->second = num;
            i++;
            break;
        }
    }
    this->second = num;

    return true;
}

SimpleTime::SimpleTime(short hour, short minute, short second, const Date &date) : hour(hour), minute(minute),
                                                                                   second(second), date(date) {}

SimpleTime SimpleTime::nowTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return SimpleTime(ltm->tm_hour, ltm->tm_min, ltm->tm_sec, Date::today());
}

int SimpleTime::compare(SimpleTime another) {
    int result = this->date.compare(another.date) != 0;
    if (result == 0) {
        if (this->hour != another.hour) {
            return this->hour - another.hour;
        } else if (this->minute != another.minute) {
            return this->minute - another.minute;
        } else
            return this->second - another.second;
    }
    return result;
}

int SimpleTime::compare(Date another) {
    return this->date.compare(another);
}

long long SimpleTime::toLLTime() {
    // todo : 这里用cin或者别的重构一下吧,太丑了,不忍直视
    string time = "";
    time += this->date.year;
    if (this->date.month < 10) {
        time += "0";
    }
    time += this->date.month;
    if (this->date.day < 10) {
        time += "0";
    }
    time += this->date.day;
    if (this->hour < 10) {
        time += "0";
    }
    time += this->hour;
    if (this->minute < 10) {
        time += "0";
    }
    time += this->minute;
    if (this->second < 10) {
        time += "0";
    }
    time += this->second;

    return atoll(time.c_str());
}

SimpleTime SimpleTime::llTimeToSimpleTime(long long time) {
    string time_str = to_string(time);
    if (time_str.length() < 12) {
        return SimpleTime();
    }
    Date date(atoi(time_str.substr(0, 3).c_str()), atoi(time_str.substr(4, 5).c_str()),
              atoi(time_str.substr(6, 7).c_str()));
    return SimpleTime(atoi(time_str.substr(8, 9).c_str()), atoi(time_str.substr(10, 11).c_str()),
                      atoi(time_str.substr(12, 13).c_str()), date);
}

SimpleTime SimpleTime::addDay(int num) {
    this->date.addDay(num);
    return *this;
}






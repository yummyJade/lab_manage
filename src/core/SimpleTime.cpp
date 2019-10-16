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






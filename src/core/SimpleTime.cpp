#include "core/SimpleTime.h"
#include <iostream>
#include <string>
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






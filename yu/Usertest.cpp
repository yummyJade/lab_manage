//
// Created by THINKPAD on 2019/9/24.
//
#include <iostream>
#include <stdlib.h>

#define enum_to_string(x) #x
using namespace std;

enum status {
    Admin, Teacher, Graduate, Undergraduate
};        //特权思想枚举
const string STATUS[4] = {
        "ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"
};

string serialize(status temp) {
//    temp = Graduate;
    int idx = 0;
    if (temp > Undergraduate) {
        return "error";
    }
    idx = static_cast<status>(temp);
    return STATUS[idx];
}

bool deSerialize(string temp, status &tempS) {
    for (int i = 0; i < 4; i++) {
        if (temp == STATUS[i]) {
            tempS = status(i);
            return true;
        }
    }
    return false;
}

int main() {
    string s = "UNDERGRADUATE";
    status ss;
    bool f;

    f = deSerialize(s, ss);
    cout << ss << endl;
//    cout<<enum_to_string(ss1)<<endl;
    cout << serialize(ss) << endl;



}
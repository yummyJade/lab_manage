#ifndef MYC1_INPUT_Cpp
#define MYC1_INPUT_Cpp

#include "../../include/core/Input.h"
#include <iostream>
#include <string>
using namespace std;

int Input::getInt() {
    int result;
    cin>>result;
    while(cin.fail()){
        cin.clear();
        cin.ignore(10000,'\n');
        cout<<"������һ������:";
        cin>>result;
    }
    return result;
}

long long Input::getLongLong() {
    long long result;
    cin>>result;
    while(cin.fail()){
        cin.clear();
        cin.ignore(10000,'\n');
        cout<<"������һ��������:";
        cin>>result;
    }
    return result;
}

std::string Input::getAssignMaxLengthStr(int maxLength=20) {
    string result;
    cin>>result;
    while (1){
        while(cin.fail()){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"���벻�Ϸ�,����������:";
            cin>>result;
        }

        if(result.length()<=maxLength){
            break;
        }
    }
    return result;
}

char Input::getChar() {
    char result;
    cin>>result;
    while(cin.fail()){
        cin.clear();
        cin.ignore(10000,'\n');
        cout<<"������һ���ַ�:";
        cin>>result;
    }
    return result;
}
#endif //MYC1_INPUT_Cpp
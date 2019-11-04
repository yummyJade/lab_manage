#ifndef MYC1_INPUT_Cpp
#define MYC1_INPUT_Cpp

#include "../../include/core/Input.h"
#include <iostream>
#include <string>
using namespace std;

int Input::getInt(bool needPositive) {
    int result;
    cin>>result;

    while (1){
        while(cin.fail()){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"������һ������:";
            cin>>result;
        }
        if(needPositive && result<=0){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"������һ��������:";
            cin>>result;
        }else{
            break;
        }
    }

	cin.clear();
	cin.ignore(10000, '\n');
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
	cin.clear();
	cin.ignore(10000, '\n');
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
		else {
            cin.clear();
            cin.ignore(10000,'\n');
			cout << "���볤�Ȳ��ܳ���"<< maxLength <<",����������:";
            cin>>result;
		}
    }
	cin.clear();
	cin.ignore(10000, '\n');
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
	cin.clear();
	cin.ignore(10000, '\n');
    return result;
}
#endif //MYC1_INPUT_Cpp
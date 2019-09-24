//
// Created by THINKPAD on 2019/9/24.
//
#include <iostream>
#include <stdlib.h>
using namespace std;

enum status {
    Admin,Teacher, Graduate, Undergraduate
};        //特权思想枚举
const string STATUS[4] = {
        "ADMIN","TEACHER","GRADUATE","UNDERGRADUATE"
};

string serialize(status temp){
    temp = Graduate;
    int idx = 0;
    if(temp > Undergraduate)
    {
        return "error";
    }
    idx = static_cast<status>(temp);

    return STATUS[idx+1];
}

bool deSerialize(string temp,status& tempS){

    int len;
    bool flag = false;
    len = 4;
    for(int i = 0; i < len; i++ ){
        if(temp == STATUS[i]){
            tempS = status(i);
            flag = true;
            break;
        } else{
            continue;
        }

    }
    if(flag){

        return true;
    }else{
        return false;
    }

}

int main(){

string s = "GRADUATE";
status ss ;
bool f;

f = deSerialize(s,ss);




}
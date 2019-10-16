//
// Created by THINKPAD on 2019/10/14.
//


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <iostream>
#include "./Verify.h"

using namespace std;

//Verify(){
//
//}
Verify::Verify(){

}

int Verify::convertDigtal(char inputStr[]){
    //todo:string转char
    int inputInt;
    if(isdigit(inputStr[0])){
        inputInt = atoi(inputStr);
        return inputInt;
    }
    return -1;

}

bool Verify::optionExist(int choose, int sum){
    return (choose < sum && choose > -1);
}

bool Verify::optionExist(int choose, int optionArr[]) {
    int len = sizeof(optionArr)/ sizeof(optionArr[0]);
    for(int i = 0; i < len; i++){
        if(choose == optionArr[i]){
            return true;
        }
    }
    cout << "选项不存在" << endl;
    return false;
}
int main ()
{
    char str[]="dgd";

    Verify verify = Verify();
    int a = verify.convertDigtal(str);
    cout << a;
    return 0;
}

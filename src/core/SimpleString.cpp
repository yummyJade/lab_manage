#include "core/SimpleString.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;


bool SimpleString::isDigital(std::string str) {
    //todo:重要级别9,有时间测测哪个实现快
    stringstream sin(str);
    int num;
    char p;
    if(!(sin >> num))
        return false;
    return !(sin >> p);
}


string SimpleString::fixPath(std::string str){
    regex double_brace("[/\\\\]{1,2}");  // (/{1,2})|(\\\\{1,2})
    return regex_replace(str, double_brace, "\\");
}


bool SimpleString::isLegalPath(std::string str) {
    string suffix = str.substr(str.find_last_of("."), str.size()); // 获取后缀,//todo:这里后缀不存在估计会报错
    if (suffix==".txt"||suffix==".csv"){
        return true;
    }
    return false;
}
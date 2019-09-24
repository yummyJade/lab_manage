#include "core/SimpleString.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;


bool SimpleString::isDigital(std::string str) {
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
    return true;
}
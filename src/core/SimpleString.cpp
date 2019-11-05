#include "core/SimpleString.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "../../src/core/Input.cpp"

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
	regex double_brace("[/\\\\]{1,2}");
    return regex_replace(str, double_brace, "\\");
}

bool SimpleString::isLegalPath(std::string str) {
    if (str.find(".") == string::npos) {
        return false;
    }
    string suffix = str.substr(str.find_last_of("."), str.size()); // 获取后缀,
    if (suffix==".csv"){
        return true;
    }
    return false;
}

std::string SimpleString::readPathFromCmd() {
    string path;

    while (true) {
        printf("请输入文件路径(输入0返回):");
        path=Input::getAssignMaxLengthStr(200);
        if(path=="0")
            return "";
        // cin >> path;

        if (path != SimpleString::fixPath(path)) {
            cout << "文件路径修复为:" << SimpleString::fixPath(path) << endl;
            path = SimpleString::fixPath(path);
        }
        if (SimpleString::isLegalPath(path)) {
            break;
        }
        printf("文件路径不合法,必须输入一个csv文件的路径\n");
    }


    return path;
}

bool SimpleString::isLegalISBN(std::string str) {

    return false;
}

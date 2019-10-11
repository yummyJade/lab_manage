#include "core/SimpleString.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;


bool SimpleString::isDigital(std::string str) {
    //todo:��Ҫ����9,��ʱ�����ĸ�ʵ�ֿ�
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
    string suffix = str.substr(str.find_last_of("."), str.size()); // ��ȡ��׺,//todo:�����׺�����ڹ��ƻᱨ��
    if (suffix==".txt"||suffix==".csv"){
        return true;
    }
    return false;
}

std::string SimpleString::readPathFromCmd() {
    string path;

    while (true) {
        printf("�������ļ�·��:");
        cin >> path;
        if (path != SimpleString::fixPath(path)) {
            cout << "�ļ�·���޸�Ϊ:" << SimpleString::fixPath(path) << endl;
            path = SimpleString::fixPath(path);
        }
        if (SimpleString::isLegalPath(path)) {
            break;
        }
        printf("�ļ�·�����Ϸ�,��������һ��txt��csv�ļ���·��\n");
    }


    return path;
}

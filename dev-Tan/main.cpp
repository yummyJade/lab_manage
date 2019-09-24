//
// Created by Tjm on 2019/9/22.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../include/libm.h"
#include "../src/libcpp.h"
#include <regex>

using namespace std;

int regexTest();


int readBooksInfo() {

    string path = "";
    ifstream fin;
    while (true) {
        printf("请输入文件路径:");
        cin >> path;
        if (path != SimpleString::fixPath(path)) {
            cout << "文件路径修复为:" << SimpleString::fixPath(path) << endl;
            path = SimpleString::fixPath(path);
        }

        cout << "正在寻找文件" << path << endl;
        // "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
        fin = ifstream(path);//打开文件流操作
        if (fin.good()) {
            cout << "已找到文件,正在读取" << endl;
            break;
        } else {
            cout << "文件不存在,请检查路径后重新输入" << endl;
            continue;
        }

    }


    string line;
    int num = 0;
    while (getline(fin, line))   //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
    {
        if (num++ > 10) // 打印10行做测试
            break;
        cout << "原始字符串：" << line << endl;
        istringstream sin(line);

        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
        string name = fields[0];
        string press = fields[1];
        string author = fields[2];
        char type = fields[3][0];
        string isbn = fields[4];
//        int price=stof(fields[5])*100;
//        int num = stoi(fields[6]);
//        bool isLend = fields[7][0] == 'Y' || fields[7][0] == 'y';

        cout << "处理之后的字符串：" << name << "\t" << press << "\t" << author << endl;
    }
    return 0;
}

int regexTest() {

    std::string text = "{{ name }} for brown fox", replace_result;
    std::regex double_brace("(\\{\\{) (.*) (\\}\\})");

    //regex_replace返回值即为替换后的字符串
    replace_result = regex_replace(text, double_brace, "*");
    cout << replace_result << "\n";

    //构造存有结果的字符串，[$&]即为用[]将匹配成功部分括起来
    // $&	Inserts the matched substring.
    cout << regex_replace(text, double_brace, "[$&]") << '\n';

    // $i则输出double_brace中第i个括号匹配到的值
    cout << regex_replace(text, double_brace, "$1") << '\n';
    cout << regex_replace(text, double_brace, "[$2]") << '\n';
    cout << regex_replace(text, double_brace, "$3") << '\n';

    return 0;
}

int main() {
    readBooksInfo();


}


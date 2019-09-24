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
        printf("�������ļ�·��:");
        cin >> path;
        if (path != SimpleString::fixPath(path)) {
            cout << "�ļ�·���޸�Ϊ:" << SimpleString::fixPath(path) << endl;
            path = SimpleString::fixPath(path);
        }

        cout << "����Ѱ���ļ�" << path << endl;
        // "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
        fin = ifstream(path);//���ļ�������
        if (fin.good()) {
            cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
            break;
        } else {
            cout << "�ļ�������,����·������������" << endl;
            continue;
        }

    }


    string line;
    int num = 0;
    while (getline(fin, line))   //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
    {
        if (num++ > 10) // ��ӡ10��������
            break;
        cout << "ԭʼ�ַ�����" << line << endl;
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

        cout << "����֮����ַ�����" << name << "\t" << press << "\t" << author << endl;
    }
    return 0;
}

int regexTest() {

    std::string text = "{{ name }} for brown fox", replace_result;
    std::regex double_brace("(\\{\\{) (.*) (\\}\\})");

    //regex_replace����ֵ��Ϊ�滻����ַ���
    replace_result = regex_replace(text, double_brace, "*");
    cout << replace_result << "\n";

    //������н�����ַ�����[$&]��Ϊ��[]��ƥ��ɹ�����������
    // $&	Inserts the matched substring.
    cout << regex_replace(text, double_brace, "[$&]") << '\n';

    // $i�����double_brace�е�i������ƥ�䵽��ֵ
    cout << regex_replace(text, double_brace, "$1") << '\n';
    cout << regex_replace(text, double_brace, "[$2]") << '\n';
    cout << regex_replace(text, double_brace, "$3") << '\n';

    return 0;
}

int main() {
    readBooksInfo();


}


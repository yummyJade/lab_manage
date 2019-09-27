#include "../../include/model/User.h"
#include <iostream>
#include <vector>

using namespace std;

User::User()
{
}

User::User(int jobNum, int borrowNum, status type, const string &password) : jobNum(jobNum),
                                                                             borrowNum(borrowNum), type(type),
                                                                             password(password) {
    this->id = -1;

}

User::~User() {
}


void User::addBooksOperate() {
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
    vector<vector<string>> books;

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
        books.push_back(fields);

//        string name = fields[0];
//        string press = fields[1];
//        string author = fields[2];
//        char type = fields[3][0];
//        string isbn = fields[4];
//        int price=stof(fields[5])*100;
//        int num = stoi(fields[6]);
//        bool isLend = fields[7][0] == 'Y' || fields[7][0] == 'y';

//        vector<string> temp;
//        cout << "处理之后的字符串：" << name << "\t" << press << "\t" << author << endl;
    }

    // todo:对books进行操作,将它存到数据库里
    return;
}


vector<string> User::serialize() {
    vector<string> info;
//    info.push_back(to_string(this->id));
    info.push_back(to_string(this->jobNum));
    info.push_back(to_string(this->borrowNum));
    info.push_back(to_string(this->type));
    info.push_back(password);
    return info;
}

bool User::deSerialize(vector<string> info) {
    //todo: 这里要加上异常处理
    int id = stoi(info[0]);
    int jobNum = stoi(info[1]);
    int borrowNum = stoi(info[2]);
    status type;
    type = Teacher;// todo:这个枚举要怎么操作
    string password = info[4];
    User(id, jobNum, borrowNum, type, password);
    return true;
}

std::string User::statuEnumToString(status statu) {
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    return strs[statu];
}

status User::stringEnumToStatu(std::string str) {
    for (int i = 0; i < str.size(); ++i) { // 转化为大写
        str[i] = toupper(str[i]);
    }
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    for (int i = 0; i < sizeof(strs) / sizeof(strs[0]); i++) {
        if (str == strs[i]) {
            return status(i);
        }
    }
    return Undergraduate;//没找到默认返回的,//todo:或许不该这么写
}

User::User(int id, int jobNum, int borrowNum, status type, const string &password) : id(id), jobNum(jobNum),
                                                                                     borrowNum(borrowNum), type(type),
                                                                                     password(password) {}

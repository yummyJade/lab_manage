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
//#include "../thirdParty/sqlite3/sqlite3.h"

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
    getline(fin, line); // 吃掉首行
    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
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
        string author = fields[1];
        string press = fields[2];
        char type = fields[3][0];
        string isbn = fields[4];
        int price = stof(fields[5]) * 100;
        string position = fields[6];
        int num = stoi(fields[7]);


        cout << "处理之后的字符串：" << name << "\t" << press << "\t" << author << endl;
    }
    return 0;
}


int readUsersInfo() {
    string path;
    ifstream fin;
    while (true) {
        path = SimpleString::readPathFromCmd();
        // "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
        fin = ifstream(path);//打开文件流操作
        if (fin.good()) {
            cout << "已找到文件,正在读取" << endl;
            break;
        }

        cout << "文件不存在,请检查路径后重新输入" << endl;
    }

    string line;
    int num = 0;
    getline(fin, line); // 吃掉首行
    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
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

        long long workNum = stoll(fields[0]);
        string name = fields[1];
        string password = fields[2];
        int statu = stoi(fields[3]);

//        User();
//        cout << "处理之后的字符串：" << name << "\t" << press << "\t" << author << endl;
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


//void sqlTest();
//
//void sqlTest() {
//    sqlite3 *sql = NULL; // 一个打开的数据库实例
//    const char *path = "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\libManage.db";//某个sql文件的路径
//
//    // 根据文件路径打开数据库连接。如果数据库不存在，则创建。
//    // 数据库文件的路径必须以C字符串传入。
//    int result = sqlite3_open_v2(path, &sql, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX |
//                                             SQLITE_OPEN_SHAREDCACHE, NULL);
//
//    if (result == SQLITE_OK) {
//        std::clog << "打开数据库连接成功";
//    } else {
//        std::clog << "打开数据库连接失败";
//    }
//
//
//    const char *sqlSentence = "SELECT name, author FROM Books;";    //SQL语句
//    sqlite3_stmt *stmt = NULL;    // stmt语句句柄
//
//    //进行查询前的准备工作——检查语句合法性
//    //-1代表系统会自动计算SQL语句的长度
//    int result2 = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);
//
//    if (result2 == SQLITE_OK) {
//        std::clog << "查询语句OK";
//        // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
//        while (sqlite3_step(stmt) == SQLITE_ROW) {
//            // 取出第0列字段的值
//
//            // 取出第1列字段的值
//            const void *author = sqlite3_column_text16(stmt, 1);
////            int age = sqlite3_column_int(stmt, 1);
//            //输出相关查询的数据
//            std::clog << "name = " << ", author = " << (char *) author << endl;
//        }
//    } else {
//        std::clog << "查询语句有问题";
//    }
//    //清理语句句柄，准备执行下一个语句
//    sqlite3_finalize(stmt);
//}


int main() {

//    readBooksInfo();

    Book::printBookList(Book::searchAll());

//    DbAdapter *dbHelper = new DbAdapter("书");
//    vector<vector<string> > queryData = dbHelper->searchBySingleField("作者", "谢立");
//
//
//    for (ll i = 0; i < queryData.size(); i++) {
////        printf("2");
//        for (ll j = 0; j < queryData[0].size(); j++) {
//            printf("%s\t", queryData[i][j].data());
//        }
//        printf("\n");
//    }
//    return 1;



}


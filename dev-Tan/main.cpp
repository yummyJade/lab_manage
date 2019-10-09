//
// Created by Tjm on 2019/9/22.
//
#include "../include/libm.h"
#include "../src/libcpp.h"

#include <fstream>
#include <regex>

using namespace std;

int regexTest();


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
    vector<vector<string>> users;
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


        long long jobNum = stoll(fields[0]); // 工号
        string name = fields[1]; // 姓名
        string password = fields[2]; // 密码
        int statu = stoi(fields[3]); //状态


        User user(jobNum, static_cast<status>(statu), name, password);
        users.push_back(user.serialize());

    }
    vector<long long> ids;
    // todo: 导入的用户可能有重复,这里要处理
    User::addUsers(users, ids);


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
    Book::printBookList(Book::searchAll());
//    Book::updateBooks("isbn","978-5-664-71497-5","count","999");
    Book::importBooks();// 导入书籍的函数
    Book::printBookList(Book::searchAll());
//    DbAdapter db("Book");
//    vector<vector<string> > queryData=db.searchBySingleField("isbn","978-5-664-71497-5");



//    Book::printBookList(Book::searchAll());

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


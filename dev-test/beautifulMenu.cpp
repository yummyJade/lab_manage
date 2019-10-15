//
// Created by Tjm on 2019/10/15.
//

#include <iostream>
#include <iomanip>
#include<string>
#include<windows.h>
#include <vector>
#include "libm.h"
#include "../src/libcpp.h"
#include "../src/libcpp.h"
#include "../yu/Verify.h"
#include "../yu/Verify.cpp"

using namespace std;

bool printTree(int level, string str, int deepIndex = -1);

bool printMenu(string operaNum);

void printSearchMenu();

//搜索方式
void printSearchMenu() {
    system("cls");
    while (1) {
        system("cls");
        cout << "------------图书搜索--------------" << endl
             << "1.所有" << endl
             << "2.书名" << endl
             << "3.作者" << endl
             << "4.ISBN" << endl
             << "5.类型" << endl
             << "0.返回上一级" << endl
             << "---------------------------------" << endl
             << "请输入您的选择:";
        int choose;
        char input[50];
        cin.clear();
        cin >> input;
        // todo: input长度超出来了怎么办
        choose = Verify::convertDigtal(input);
        cout << "您的选择是:" << choose << endl;


        string fieds[] = {"", "all", "name", "author", "isbn", "type"};
        int options[] = {1, 2, 3, 4, 5, 0};
        // todo: 这里判断choose是否合法
        if (Verify::optionExist(choose, 6)) {
            cout << "请输入您的搜索词:";
            string keyWord;
            cin >> keyWord;
            vector<Book> books = Book::searchBooksBySingleField(fieds[choose], keyWord);
            Book::printBookList(books);
            if (choose == 0) {
                break;
            }
        }
    }
}


bool printMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.检索操作");
    if (userOpera[0] == '1') {
        printTree(2, "11.图书检索", deepIndex);

        printTree(2, "12.用户检索(未完成)", deepIndex);
    }

    printTree(1, "2.用户管理");
    if (userOpera[0] == '2') {
        printTree(2, "21.增加用户", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "211.批量导入用户", deepIndex);
            printTree(3, "212.增加单个用户(未完成)", deepIndex);
        }

        printTree(2, "22.删除用户(未完成)", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "221.批量删除用户", deepIndex);
            printTree(3, "222.删除单个用户", deepIndex);
        }
    }

    printTree(1, "3.图书管理");
    if (userOpera[0] == '3') {
        printTree(2, "31.增加图书", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "311.批量导入图书", deepIndex);
            printTree(3, "312.增加单种书籍", deepIndex);
            printTree(3, "313.增加单本书籍", deepIndex);
        }

        printTree(2, "32.删除图书(未完成)", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "321.批量删除图书", deepIndex);
            printTree(3, "322.删除单种书籍", deepIndex);
            printTree(3, "323.删除单本书籍", deepIndex);
        }

    }

    if (userOpera != "") {
        cout << endl << "请选择操作:" << endl;
        int operaNum = 0;
        cin >> operaNum;
        if (to_string(operaNum).length() <= deepIndex)
            printMenu(to_string(operaNum));
        switch (operaNum) {
            case 11:
                printSearchMenu();
                break;
            case 12:
                break;
            case 13:
                break;


            case 211:
                User::importUsers();
                break;
            case 212:
                break;
            case 23:
                break;


            case 311:
                Book::importBooksService();//批量导入图书
                break;
            case 312:
                Book::addOneBookService();//增加单种书籍
                break;
            case 313:
                BookInstance::addOneBookInstancesService();//增加单本书籍
                break;
            case 32:
                break;


        }
    }


    return false;
}


bool printTree(int level, string str, int deepIndex) {
    bool isHigh = (deepIndex > 1) && (deepIndex == level);
    while (level > 1) {
        level--;
        cout << setw(2) << "";
    }
    if (isHigh) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                BACKGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
    }
    cout << "--> " << str << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return true;
}


int main() {

    string operaNum = "";
    printMenu(operaNum);
    while (operaNum != "-1") {
        cout << "请输入操作数" << endl;
        cin >> operaNum;
        system("cls");
        printMenu(operaNum);
    }


    /*cout << "-------------------------------" << endl;
    cout << "-->" << 1 << " 图书管理" << endl;
    cout << "  -->" << 11 << " 增加图书" << endl;
    cout << "    -->" << 111 << " 增加单种图书" << endl;
    cout << "    -->" << 112 << " 增加单本图书" << endl;
    cout << "  -->" << 12 << " 删除图书" << endl;
    cout << "  -->" << 13 << " 导入图书" << endl;
    cout << "  -->" << 14 << " 导出图书" << endl;
    cout << "-------------------------------" << endl;
    cout << "-->" << 2 << " 用户管理" << endl;
    cout << "  -->" << 11 << " 增加用户" << endl;
    cout << "  -->" << 12 << " 删除用户" << endl;
    cout << "  -->" << 13 << " 导入用户" << endl;
    cout << "  -->" << 14 << " 导出用户" << endl;
    cout << "-------------------------------" << endl;
    cout << "-->" << 3 << " 信息检索" << endl;
    cout << "-------------------------------" << endl;*/



    /*vector<string> navs = { "操作码", "操作"};
    TableRenderer render(navs, 8);
    vector<vector<string>> lines = { { "1","图书管理" },{ "2","用户管理" },{ "3","信息检索" } };
    for (int i = 0; i < lines.size(); ++i) {
        render.addColume(lines[i]);
    }
    render.render();*/
}



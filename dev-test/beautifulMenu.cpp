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

bool printAdminMenu(string userOpera);

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
            cout << "请选择要操作的图书序号:";


            cin >> keyWord;
            if (choose == 0) {
                break;
            }
        }
    }
}

// 普通用户首页
bool printUserMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.检索操作");
    if (userOpera[0] == '1') {
        printTree(2, "11.图书检索", deepIndex);
    }

    printTree(1, "2.我的借阅");
    if (userOpera[0] == '2') {
        printTree(2, "21.我的在借图书", deepIndex);
        printTree(2, "22.我的预约图书", deepIndex);
        printTree(2, "21.我的借阅图书", deepIndex);
    }

    printTree(1, "3.账号相关");
    if (userOpera[0] == '3') {
        printTree(2, "31.修改密码", deepIndex);
    }

}

// 管理员操作用户的页面
bool printAdminDealUserMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.检索/借阅操作");
    if (userOpera[0] == '1') {
        printTree(2, "11.图书检索/借阅", deepIndex);
        printTree(2, "12.预约已到书籍", deepIndex);
    }

    printTree(1, "2.借阅记录");
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


}

// 管理员首页
bool printAdminMenu(string userOpera = "0") {
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
            printTree(3, "212.增加单个用户", deepIndex);
        }

        printTree(2, "22.冻结/解冻用户", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "221.冻结指定用户", deepIndex);
            printTree(3, "222.解冻指定用户", deepIndex);
        }

        printTree(2, "23.初始化用户密码", deepIndex);

    }

    printTree(1, "3.图书管理");
    if (userOpera[0] == '3') {
        printTree(2, "31.增加图书", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "311.批量导入图书", deepIndex);
            printTree(3, "312.增加单种书籍", deepIndex);
            printTree(3, "313.增加单本书籍", deepIndex);
        }

        printTree(2, "32.下架图书(未完成)", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "321.批量下架图书", deepIndex);
            printTree(3, "322.下架单种书籍", deepIndex);
            printTree(3, "323.下架单本书籍", deepIndex);
        }

        printTree(2, "33.修改图书", deepIndex);
        if (userOpera[1] == '3') {
            printTree(3, "331.修改指定种类图书信息", deepIndex);
            printTree(3, "332.修改指定图书实例信息", deepIndex);
        }
    }

    printTree(1, "400.处理用户操作");

    if (userOpera != "") {
        cout << endl << "请选择操作:" << endl;
        int operaNum = 0;
        cin >> operaNum;
//        if (to_string(operaNum).length() <= deepIndex-1){
//            printAdminMenu(to_string(operaNum));
//        }

        switch (operaNum) {
            case 11:
                printSearchMenu();
                break;
            case 12:
                break;
            case 13:
                break;


            case 211:// 批量导入用户
                User::importUsers();
                break;
            case 212:// 增加单个用户
                addSingleUserService();
                break;

            case 221:// 冻结指定用户
                freezeAssignUser();
                break;
            case 222:// 解冻指定用户
                unfreezeAssignUser();
                break;

            case 23:// 初始化用户密码
                resetAssignUserPassword();
                break;


            case 31:
                printAdminMenu(to_string(operaNum));
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
                printAdminMenu(to_string(operaNum));
            case 321:
                Book::importBooksService();//批量导入图书
                break;
            case 322:
                Book::addOneBookService();//增加单种书籍
                break;
            case 323:
                BookInstance::addOneBookInstancesService();//增加单本书籍
                break;

            case 33:
                printAdminMenu(to_string(operaNum));
            case 331:
                changeAssignBookInfo();
                break;
            case 332:
                changeAssignBookInstanceInfo();
                break;


            case 400:
                printAdminDealUserMenu();
                break;

            default:
                printAdminMenu(to_string(operaNum));
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
    printAdminMenu(operaNum);
    while (operaNum != "-1") {
        cout << "请输入操作数" << endl;
        cin >> operaNum;
        system("cls");
        printAdminMenu(operaNum);
    }


}



#include <iostream>
#include "../include/libm.h"
#include "../src/libcpp.h"

using namespace std;


//打印主菜单
void printMainMenu() {
    system("cls");
    while (1) {
        cout << "------------图书馆管理系统--------------" << endl
             << "1.用户登录" << endl
             << "2.管理员登录" << endl
             << "3.图书搜索" << endl
             << "0.退出系统" << endl
             << "--------------------------------------" << endl
             << "请输入您的选择:";
        int choose;
        cin >> choose;
        cout << "您的选择是:" << choose << endl;
        switch (choose) {
            case 1:
                printUserMenu();
                break;
            case 2:
                printAdminMenu();
                break;
            case 3:
                printSearchMenu();
                break;
            case 0:
                break;

            default:
                cout << "该选项不存在，请重新输入!" << endl;
                break;

        }
        if (choose == 0) {
            cout << "欢迎您的下次访问" << endl;
            break;

        }

    }
}


int main() {
    printMainMenu();

}
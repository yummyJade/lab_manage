#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include "libm.h"
#include "../src/util/DbAdapter.cpp"

using namespace std;

//函数声明
void printUserMenu();
void printMainMenu();
void printAdminMenu();
void printSearchMenu();
void printUserManageMenu();
void printSearchResult();

bool offFlag = false;
//一些需要先初始化的东西
void init(){
//    DbAdapter *dbHelper = new DbAdapter("书");

}
//打印主菜单
void printMainMenu() {

    while (1) {
        system("cls");
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
            case 2: //todo:还要增加登录的验证
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

//用户菜单
void printUserMenu(){
    while (1){


        cout << "------------用户操作--------------" << endl
             << "1.图书搜索" << endl
             << "2.个人订单" << endl
             << "3.修改密码" << endl
             << "0.返回上一级" << endl
             << "---------------------------------" << endl
             << "请输入您的选择:";
        int choose;
        cin >> choose;
        cout << "您的选择是:" << choose << endl;
        switch (choose){
            case 1:
                printSearchMenu();break;
            case 2:
                cout << "您的订单查询结果如下：" << endl;
                void printSearchResult();
                cout << std::left << setw(10) << "1" << setw(20) << "乌合之众" << setw(20) << "庞乐"
                     << setw(20) << "123456678" << setw(20) << "心理学" << endl;
                break;  //调用用户查询订单的函数
            case 3:
                break;
            case 0:
                break;
            default:
                cout << "该选项不存在，请重新输入!" << endl;
                break;
        }
        if (choose == 0) {

            break;

        }

    }


}

//管理员菜单
void printAdminMenu(){
    while (1){


        cout << "------------管理员操作--------------" << endl
             << "1.图书搜索" << endl
             << "2.添加书目" << endl
             << "3.减少书目" << endl
             << "4.用户管理" << endl
             << "0.返回上一级" << endl
             << "-----------------------------------" << endl
             << "请输入您的选择:";
        int choose;
        cin >> choose;
        cout << "您的选择是:" << choose << endl;
        switch (choose){
            case 1:
                printSearchMenu();break;
            case 2:

                cout << "增加书啦" << endl;
                break;
            case 3:
                cout << "减少书啦" << endl;
                break;
            case 4:

                while (1){
                    cout << "请输入用户工号:";
                    int userNum;
                    cin >> userNum;
                    if(userNum == 170200){
                        cout << "您已进入" << userNum << "用户管理中心！" << endl;
                        printUserManageMenu();
                        break;
                    } else{
                        cout << "用户不存在，请重新输入！" << endl;
                    }
                }


                break;
            case 5:

            case 0:
                break;
            default:
                cout << "该选项不存在，请重新输入!" << endl;
        }
        if(choose == 0){
            break;
        }
    }




}

//查询条目头
void printSearchResult(){
    cout
            << "----------------------------------------------------------------------------------------------------------------------------------" << endl
            << setiosflags(ios::left) << setw(10) << "序号" << setw(20) << "书名" << setw(20) << "出版社" << setw(10) << "作者"
            << setw(10) << "类型" << setw(20) << "ISBN" << setw(10) << "价格" << setw(10) << "数量" << setw(10) << "是否可借" << endl
            << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void printQueryData(vector<vector<string> > queryData){
    for (ll i = 0; i < queryData.size(); i++) {
        printf("%-10d",i+1);
        for (ll j = 0; j < queryData[0].size(); j++) {

            printf("%s\t", queryData[i][j].data());
        }
        printf("\n");
    }
};
//搜索方式
void printSearchMenu(){

    while (1) {
        DbAdapter *dbHelper = new DbAdapter("书");
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
        cin >> choose;
        cout << "您的选择是:" << choose << endl;
        string keyWord;
        if(choose != 0){
            cout << "请输入您的搜索词:";
            cin >> keyWord;
        }

        vector<vector<string> > queryData;
        printSearchResult();
        switch (choose){
            case 1:
                cout<< "xxxxxx" << endl;break;
            case 2:
                queryData = dbHelper->searchBySingleField("书名", keyWord);
                break;
            case 3:
                queryData = dbHelper->searchBySingleField("作者", keyWord);
                printQueryData(queryData);
                break;
            case 4:
                queryData = dbHelper->searchBySingleField("isbn", keyWord);
                printQueryData(queryData);
                break;
            case 5:
                queryData = dbHelper->searchBySingleField("类型",keyWord);
                printQueryData(queryData);
                break;
            case 0:
                break;
            default:
                cout << "该选项不存在，请重新输入!" << endl;
        }
        if(choose == 0){
            break;
        }
        getchar();
        getchar();

    }
}

//用户管理
void printUserManageMenu(){
    while (1) {
        if(offFlag){
            offFlag = false;
            break;
        }
        cout << "------------用户管理--------------" << endl
             << "1.个人订单" << endl
             << "2.借书" << endl
             << "3.还书" << endl
             << "4.密码重置" << endl
             << "5.注销" << endl
             << "0.返回上一级" << endl
             << "---------------------------------" << endl
             << "请输入您的选择:";
        int choose;
        cin >> choose;
        cout << "您的选择是:" << choose << endl;
        switch (choose){
            case 1:
                cout << "该用户的订单情况如下:" << endl;
                printSearchResult();
                cout<< "xxxxxx" << endl;break;
            case 2:
                printSearchMenu();
            case 3:
                break;
            case 4:
                cout << "密码重置成功，请尽快通知改用户修改密码!" << endl;
            case 5:
                while (1){
                    char yn;
                    cout << "您确定要注销么？(Y/N):";
                    cin >> yn;
                    if(yn == 'Y' || yn == 'y'){
                        offFlag = true;
                        cout << "注销成功！" << endl;
                        break;


                    } else{
                        break;
                    }
                }


            case 0:
                break;
            default:
                cout << "该选项不存在，请重新输入!" << endl;
        }
        if(choose == 0){
            break;
        }
    }
}

int main(){
    system("mode con cols=140 lines=40");
    init();
    printMainMenu();

    return 0;

}

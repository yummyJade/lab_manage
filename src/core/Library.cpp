//
// Created by Tjm on 2019/10/19.
//

#include "core/Library.h"
#include <iostream>
#include <string>

using namespace std;

User *Library::login(bool needSimpleUser = false) {
    long long jobNum;
    string password;
    User *user;
    int resultCode = 1;

    while (resultCode != 0) {
        cout << "请输入工号(输入0取消登陆)";
        cin >> jobNum;
        if (jobNum == 0) {
            return NULL;
        }
        cout << "请输入密码";
        cin >> password;
        resultCode = User::login(jobNum, password, user);
        switch (resultCode) {
            default:
            case 1:
                cout << "账号不存在" << endl;
            case 2:
                cout << "密码错误" << endl;
            case 0:
                if (needSimpleUser && user->getType() == 0) {
                    cout << "该账户是管理员账号,请登陆一个普通账户";
                    resultCode = 3;
                }
                break;
        }
    }

    if (needSimpleUser || user->getType() != 0) {
        Library::simpleUser = user;
    } else {
        Library::adminUser = user;
    }

    return user;
}

User *Library::loginAdminUser() {
    long long jobNum;
    string password;

    cout << "工号:";
    cin >> jobNum;
    cout << "密码:";
    cin >> password;

//    User::login(jobNum,password);
    return nullptr;
}

User *Library::getSimpleUserInstance() {
    vector<User> results = User::searchUsersBySingleField("jobNum", "10000");
    if (results.size() > 0) {
        User *user = new User(results[0]);
        return user;
    }
    return NULL;
}

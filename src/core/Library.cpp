//
// Created by Tjm on 2019/10/19.
//

#include "core/Library.h"
#include <iostream>
#include <string>

using namespace std;

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

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

User *Library::getSimpleUserInstance() {
    vector<User> results = User::searchUsersBySingleField("jobNum", "10000");
    if (results.size() > 0) {
        User *user = new User(results[0]);
        return user;
    }
    return NULL;
}

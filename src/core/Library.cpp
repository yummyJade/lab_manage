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
        cout << "�����빤��(����0ȡ����½)";
        cin >> jobNum;
        if (jobNum == 0) {
            return NULL;
        }
        cout << "����������";
        cin >> password;
        resultCode = User::login(jobNum, password, user);
        switch (resultCode) {
            default:
            case 1:
                cout << "�˺Ų�����" << endl;
            case 2:
                cout << "�������" << endl;
            case 0:
                if (needSimpleUser && user->getType() == 0) {
                    cout << "���˻��ǹ���Ա�˺�,���½һ����ͨ�˻�";
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

    cout << "����:";
    cin >> jobNum;
    cout << "����:";
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

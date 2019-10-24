#include "service/UserService.h"
#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"

// 根据输入选择一个user, 取消返回NULL
User *choseOneUser() {
    long long jobNum;
    User *user;
    while (true) {
        printf("请输入要操作的用户工号(输入0返回):");
        cin >> jobNum;
        if (jobNum == 0) {
            return NULL;
        }

        if (User::checkUserExist(jobNum, user)) { // 用户不存在
            return user;

        }
        printf("工号为%lld的用户不存在!", jobNum);
    }

}

bool printAssignInfo(User user) {
    vector<User> users;
    users.push_back(user);
    printf("检索到如下信息\n");
    User::printUserList(users);
	return true;
}

//===============================================================
//===============================================================

bool borrowAssignBookInstance(int jobNum, long long bookInstanceId) {
    // 判断用户能否借书(是否有权借阅,借书数量是多少)

    // 判断该书是否能被借阅

    // 插入一条借阅记录Order,需要能借多久,
    Order order();
    // 设置该Bookinstance不可借,并更新应还时间



    return false;
}


bool resetAssignUserPassword() {
    User *user = choseOneUser();
    if (user == NULL) {
        return false;
    }

    printAssignInfo(*user);

    user->changePwd(to_string(user->getJobNum()));
    printf("密码已重置为该用户的学号:%lld\n", user->getJobNum());
    return true;
}


// 冻结账号
bool freezeAssignUser() {
    User *user = choseOneUser();
    if (user == NULL) {
        return false;
    } else if (user->getType() == 0) {
        cout << "不能冻结管理员账号" << endl;
        return false;
    }
    printAssignInfo(*user);

    int newState = user->getType();
    if (newState < 0) {
        printf("操作失败,工号为:%lld的账号已经是冻结状态", user->getJobNum());
        return false;
    }
    newState = -newState;
    User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "state", to_string(newState));
    printf("已冻结学号为:%lld的用户\n", user->getJobNum());
}


// 解冻账号
bool unfreezeAssignUser() {
    User *user = choseOneUser();
    if (user == NULL) {
        return false;
    } else if (user->getType() == 0) {
        cout << "不能解冻管理员账号" << endl;
        return false;
    }
    printAssignInfo(*user);

    int newState = user->getType();
    if (newState > 0) {
        printf("操作失败,工号为:%lld的账号未被冻结,无需解冻", user->getJobNum());
        return false;
    }
    newState = -newState;
    User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "state", to_string(newState));
    printf("已冻结学号为:%lld的用户\n", user->getJobNum());
}


// 增加单个用户
bool addSingleUserService() {
    User *user;
    while (true) {
        printf("请依次输入 工号,姓名,用户类型(0管理员 1本科生 2研究生 3教师) 用空格隔开[工号输入0返回]\n");
        long long jobNum;
        string name;
        int type;
        cin >> jobNum >> name >> type;

        if (jobNum == 0) {
            return true;
        }

        // 判断用户是否存在
        if (!User::checkUserExist(jobNum, user)) {
            User newUser(jobNum, static_cast<status>(type), name);
            vector<vector<string>> users;
            users.push_back(newUser.serialize());
            vector<long long> ids;
            User::addUsers(users, ids);

            return true;
        }
        printf("工号为%lld的用户已经存在,添加失败", jobNum);
    }


    return true;
}
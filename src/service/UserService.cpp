#include "service/UserService.h"
#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"

// 根据输入选择一个user, 取消返回NULL
User *choseOneUser() {
    long long jobNum;
    User *user=new User();
    while (true) {
        printf("工号输入0返回\n");
        jobNum=user->readAndSetJobNum();
        if (jobNum == 0) {
            delete user;
            return NULL;
        }

        if (User::checkUserExist(jobNum, user)) { // 用户不存在
            return user;
        }
        printf("工号为%lld的用户不存在!\n", jobNum);
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

    cout << "输入Y确认重置该用户的密码,输入N取消:";
    char operate;
    operate=Input::getChar();
    if (operate == 'Y' || operate == 'y') {// 下架所有图书
        user->setPassword(to_string(user->getJobNum()));
        printf("密码已重置为该用户的学号:%lld\n", user->getJobNum());
        return true;
    }else{
        cout<<"取消操作"<<endl;
        return false;
    }

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

	char opera;
	cout << "输入Y确认冻结,N取消操作";
	opera=Input::getChar();
	if (opera == 'Y' || opera == 'y') {
		int newState = user->getType();
		if (newState < 0) {
			printf("操作失败,工号为:%lld的账号已经是冻结状态\n", user->getJobNum());
			return false;
		}
		newState = -newState;
		User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "type", to_string(newState));
		printf("已冻结学号为:%lld的用户\n", user->getJobNum());
	}
	else {
		cout << "取消操作"<<endl;
	}

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
	char opera;
	cout << "输入Y确认解冻,N取消操作";
    opera=Input::getChar();
	if (opera == 'Y' || opera == 'y') {
		int newState = user->getType();
		if (newState > 0) {
			printf("操作失败,工号为:%lld的账号未被冻结,无需解冻\n", user->getJobNum());
			return false;
		}
		newState = -newState;
		User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "type", to_string(newState));
		printf("已解冻学号为:%lld的用户\n", user->getJobNum());
	}
	else {
		cout << "取消操作" << endl;
	}
}


// 增加单个用户
bool addSingleUserService() {
    User *user=new User();
    while (true) {
        long long jobNum;
        string name;
        int type;
        printf("工号输入0返回\n");
        jobNum=user->readAndSetJobNum();
        if (jobNum == 0) {
            delete user;
            return true;
        }
        name=user->readAndSetName();
        type=user->readAndSetType();

        // 判断用户是否存在
        if (!User::checkUserExist(jobNum, user)) {
            User newUser(jobNum, static_cast<status>(type), name);
            vector<vector<string>> users;
            users.push_back(newUser.serialize());
            vector<long long> ids;
            User::addUsers(users, ids);

            delete user;
            return true;
        }
        printf("工号为%lld的用户已经存在,添加失败", jobNum);
    }
}

bool dealWithOverTimeOrder(User* user=NULL){
    if(user==NULL){//登陆用户
        while(1){
            user=choseOneUser();
            if (user == NULL) {
                return false;
            }else{
                if (Order::getAssignUserOweOrder(user->getFirstOrderId()).empty()){ // 没有逾期记录
                    cout<<"该用户当前没有逾期借阅,无需处理"<<endl;
                }else{
                    break;
                }
            }
        }
    }

    user->dealWithOverTimeOrder();
    return true;
}
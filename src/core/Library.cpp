#include "core/Library.h"
#include <iostream>
#include <string>
#include "util/DbAdapter.h"
//#include "../../src/util/DbAdapter.cpp"
#include "../../src/core/Input.cpp"

using namespace std;

User* Library::simpleUser =NULL;
User* Library::adminUser =NULL;

User *Library::login(bool needSimpleUser = false) {
    long long jobNum;
    string password;
    User *user=new User();
    int resultCode = 1;

    while (resultCode != 0) {
		printf("=========================================\n");
		printf("---------------用户登陆-----------------\n");
		printf("=========================================\n");
        cout << "工号(输入0返回):";
        jobNum = Input::getLongLong();
        if (jobNum == 0) {
            return NULL;
        }
        cout << "密码:";
        cin >> password;
        resultCode = User::login(jobNum, password, user);
		system("cls");
        switch (resultCode) {
            default:
            case 1:
                cout << "账号不存在" << endl;
				break;
            case 2:
                cout << "密码错误" << endl;
				break;
			case 3:
				cout << "该账号已被冻结,请联系管理员进行解冻操作" << endl;
				break;
            case 4:
                cout << "该账号有逾期记录未处理,禁止登陆" << endl;
                break;
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
	if (Library::adminUser == NULL) {
		cout << "未登陆,使用测试账号" << endl;
		vector<User> results = User::searchUsersBySingleField("jobNum", "10086");
		if (results.size() > 0) {
			User* user = new User(results[0]);

			return user;
		}
	}

	return Library::adminUser;
}

User *Library::getSimpleUserInstance() {
	if (Library::simpleUser == NULL) {
		cout << "未登陆,使用测试账号"<<endl;
		vector<User> results = User::searchUsersBySingleField("jobNum", "10000");
		if (results.size() > 0) {
			User* user = new User(results[0]);

			return user;
		}
	}
    
    return Library::simpleUser;
}

User* Library::getAdminUserInstance() {
	if (Library::adminUser == NULL) {
		cout << "未登陆,使用测试账号" << endl;
		vector<User> results = User::searchUsersBySingleField("jobNum", "10086");
		if (results.size() > 0) {
			User* user = new User(results[0]);
			return user;
		}
	}

	return Library::adminUser;
}

bool Library::checkDBLegal() {
    DbAdapter::init();
    return DbAdapter::checkTableExist("User") && DbAdapter::checkTableExist("Book");
}

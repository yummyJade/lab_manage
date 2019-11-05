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
		printf("---------------�û���½-----------------\n");
		printf("=========================================\n");
        cout << "����(����0����):";
        jobNum = Input::getLongLong();
        if (jobNum == 0) {
            return NULL;
        }
        cout << "����:";
        cin >> password;
        resultCode = User::login(jobNum, password, user);
		system("cls");
        switch (resultCode) {
            default:
            case 1:
                cout << "�˺Ų�����" << endl;
				break;
            case 2:
                cout << "�������" << endl;
				break;
			case 3:
				cout << "���˺��ѱ�����,����ϵ����Ա���нⶳ����" << endl;
				break;
            case 4:
                cout << "���˺������ڼ�¼δ����,��ֹ��½" << endl;
                break;
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
	if (Library::adminUser == NULL) {
		cout << "δ��½,ʹ�ò����˺�" << endl;
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
		cout << "δ��½,ʹ�ò����˺�"<<endl;
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
		cout << "δ��½,ʹ�ò����˺�" << endl;
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

#include "service/UserService.h"
#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"

// ��������ѡ��һ��user, ȡ������NULL
User *choseOneUser() {
    long long jobNum;
    User *user=new User();
    while (true) {
        printf("������Ҫ�������û�����(����0����):");
        cin >> jobNum;
        if (jobNum == 0) {
            return NULL;
        }

        if (User::checkUserExist(jobNum, user)) { // �û�������
            return user;

        }
        printf("����Ϊ%lld���û�������!", jobNum);
    }

}

bool printAssignInfo(User user) {
    vector<User> users;
    users.push_back(user);
    printf("������������Ϣ\n");
    User::printUserList(users);
	return true;
}

//===============================================================
//===============================================================

bool borrowAssignBookInstance(int jobNum, long long bookInstanceId) {
    // �ж��û��ܷ����(�Ƿ���Ȩ����,���������Ƕ���)

    // �жϸ����Ƿ��ܱ�����

    // ����һ�����ļ�¼Order,��Ҫ�ܽ���,
    Order order();
    // ���ø�Bookinstance���ɽ�,������Ӧ��ʱ��



    return false;
}


bool resetAssignUserPassword() {
    User *user = choseOneUser();
    if (user == NULL) {
        return false;
    }

    printAssignInfo(*user);

    user->setPassword(to_string(user->getJobNum()));
    printf("����������Ϊ���û���ѧ��:%lld\n", user->getJobNum());
    return true;
}


// �����˺�
bool freezeAssignUser() {
    User *user = choseOneUser();
    if (user == NULL) {
        return false;
    } else if (user->getType() == 0) {
        cout << "���ܶ������Ա�˺�" << endl;
        return false;
    }
    printAssignInfo(*user);

	char opera;
	cout << "����Yȷ�϶���,Nȡ������";
	cin >> opera;
	if (opera == 'Y' || opera == 'y') {
		int newState = user->getType();
		if (newState < 0) {
			printf("����ʧ��,����Ϊ:%lld���˺��Ѿ��Ƕ���״̬\n", user->getJobNum());
			return false;
		}
		newState = -newState;
		User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "type", to_string(newState));
		printf("�Ѷ���ѧ��Ϊ:%lld���û�\n", user->getJobNum());
	}
	else {
		cout << "ȡ������"<<endl;
	}
    
}


// �ⶳ�˺�
bool unfreezeAssignUser() {
    User *user = choseOneUser();
    if (user == NULL) {
        return false;
    } else if (user->getType() == 0) {
        cout << "���ܽⶳ����Ա�˺�" << endl;
        return false;
    }
    printAssignInfo(*user);
	char opera;
	cout << "����Yȷ�϶���,Nȡ������";
	cin >> opera;
	if (opera == 'Y' || opera == 'y') {
		int newState = user->getType();
		if (newState > 0) {
			printf("����ʧ��,����Ϊ:%lld���˺�δ������,����ⶳ\n", user->getJobNum());
			return false;
		}
		newState = -newState;
		User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "type", to_string(newState));
		printf("�ѽⶳѧ��Ϊ:%lld���û�\n", user->getJobNum());
	}
	else {
		cout << "ȡ������" << endl;
	}
}


// ���ӵ����û�
bool addSingleUserService() {
    User *user=new User();
    while (true) {
        printf("���������� ����,����,�û�����(0����Ա 1������ 2�о��� 3��ʦ) �ÿո����[��������0����]\n");
        long long jobNum;
        string name;
        int type;
        cin >> jobNum >> name >> type;

        if (jobNum == 0) {
            return true;
        }

        // �ж��û��Ƿ����
        if (!User::checkUserExist(jobNum, user)) {
            User newUser(jobNum, static_cast<status>(type), name);
            vector<vector<string>> users;
            users.push_back(newUser.serialize());
            vector<long long> ids;
            User::addUsers(users, ids);

            return true;
        }
        printf("����Ϊ%lld���û��Ѿ�����,���ʧ��", jobNum);
    }


    return true;
}
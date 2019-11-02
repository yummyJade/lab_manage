#include "service/UserService.h"
#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"

// ��������ѡ��һ��user, ȡ������NULL
User *choseOneUser() {
    long long jobNum;
    User *user=new User();
    while (true) {
        printf("��������0����\n");
        jobNum=user->readAndSetJobNum();
        if (jobNum == 0) {
            delete user;
            return NULL;
        }

        if (User::checkUserExist(jobNum, user)) { // �û�������
            return user;
        }
        printf("����Ϊ%lld���û�������!\n", jobNum);
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

    cout << "����Yȷ�����ø��û�������,����Nȡ��:";
    char operate;
    operate=Input::getChar();
    if (operate == 'Y' || operate == 'y') {// �¼�����ͼ��
        user->setPassword(to_string(user->getJobNum()));
        printf("����������Ϊ���û���ѧ��:%lld\n", user->getJobNum());
        return true;
    }else{
        cout<<"ȡ������"<<endl;
        return false;
    }

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
	opera=Input::getChar();
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
	cout << "����Yȷ�Ͻⶳ,Nȡ������";
    opera=Input::getChar();
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
        long long jobNum;
        string name;
        int type;
        printf("��������0����\n");
        jobNum=user->readAndSetJobNum();
        if (jobNum == 0) {
            delete user;
            return true;
        }
        name=user->readAndSetName();
        type=user->readAndSetType();

        // �ж��û��Ƿ����
        if (!User::checkUserExist(jobNum, user)) {
            User newUser(jobNum, static_cast<status>(type), name);
            vector<vector<string>> users;
            users.push_back(newUser.serialize());
            vector<long long> ids;
            User::addUsers(users, ids);

            delete user;
            return true;
        }
        printf("����Ϊ%lld���û��Ѿ�����,���ʧ��", jobNum);
    }
}

bool dealWithOverTimeOrder(User* user=NULL){
    if(user==NULL){//��½�û�
        while(1){
            user=choseOneUser();
            if (user == NULL) {
                return false;
            }else{
                if (Order::getAssignUserOweOrder(user->getFirstOrderId()).empty()){ // û�����ڼ�¼
                    cout<<"���û���ǰû�����ڽ���,���账��"<<endl;
                }else{
                    break;
                }
            }
        }
    }

    user->dealWithOverTimeOrder();
    return true;
}
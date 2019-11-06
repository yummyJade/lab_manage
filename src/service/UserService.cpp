#include "service/UserService.h"
#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"
#include <map>
#include<set>

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

    if(user->getType()==0){
        cout<<"�������ù���Ա������!"<<endl;
        return false;
    }

    printAssignInfo(*user);

    cout << "����Yȷ�����ø��û�������,����Nȡ��:";
    char operate;
    operate=Input::getChar();
    if (operate == 'Y' || operate == 'y') {
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


bool printSameBorrowBooksBetweenTwoUser(){
    cout<<"�������һ���û��Ĺ���:";
    User* oneUser=choseOneUser();
    if(oneUser==NULL){
        return false;
    }
    cout<<"������ڶ����û��Ĺ���:";
    User* anotherUser=choseOneUser();
    if(anotherUser==NULL){
        return false;
    }
    if(oneUser->getJobNum()==anotherUser->getJobNum()){
        cout<<"����ѡ��������ͬ���û�"<<endl;
        return false;
    }
    vector<Order>temp;

    // ��ȡ�û��Ľ���
    vector<Order> orders1= Order::getAssignUserBorrowingList(oneUser->getFirstOrderId());
    temp=Order::getAssignUserBorrowedHistory(oneUser->getFirstOrderId());
    orders1.insert(orders1.end(),temp.begin(),temp.end());

    vector<Order> orders2= Order::getAssignUserBorrowingList(anotherUser->getFirstOrderId());
    temp=Order::getAssignUserBorrowedHistory(anotherUser->getFirstOrderId());
    orders2.insert(orders2.end(),temp.begin(),temp.end());

    // ���û��Ľ��ļ�¼����û�������鼮
    set<int>instanceId1;
    set<int>instanceId2;

    for (int i = 0; i <orders1.size(); ++i) {
        instanceId1.insert(orders1[i].getBookId());
    }
    for (int i = 0; i <orders2.size(); ++i) {
        instanceId2.insert(orders2[i].getBookId());
    }

    set<string> isbns1;
    set<string> isbns2;
    for (set<int>::iterator it = instanceId1.begin(); it != instanceId1.end(); it++){
        isbns1.insert(BookInstance::getInstanceById(*it)->getIsbn());
    }

    for (set<int>::iterator it = instanceId2.begin(); it != instanceId2.end(); it++){
        isbns2.insert(BookInstance::getInstanceById(*it)->getIsbn());
    }

    vector<Book> Books;
    for (set<string>::iterator it = isbns1.begin(); it != isbns1.end(); it++){
        if(isbns2.count(*it)!=0){
            Books.push_back(Book::searchBooksBySingleField("isbn",*it)[0]);
        }
    }

    if(!Books.empty()){
        cout<<"����"<<oneUser->getJobNum()<<" �빤��"<<anotherUser->getJobNum()<<" ��ͬ�������±�"<<endl;
        Book::printBookList(Books);
    }else{
        cout<<"����"<<oneUser->getJobNum()<<" �빤��"<<anotherUser->getJobNum()<<" ���û�û�й�ͬ����"<<endl;
    }
    return true;
}
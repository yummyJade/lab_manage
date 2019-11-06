#include "service/UserService.h"
#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"
#include <map>
#include<set>

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

    if(user->getType()==0){
        cout<<"不能重置管理员的密码!"<<endl;
        return false;
    }

    printAssignInfo(*user);

    cout << "输入Y确认重置该用户的密码,输入N取消:";
    char operate;
    operate=Input::getChar();
    if (operate == 'Y' || operate == 'y') {
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


bool printSameBorrowBooksBetweenTwoUser(){
    cout<<"请输入第一个用户的工号:";
    User* oneUser=choseOneUser();
    if(oneUser==NULL){
        return false;
    }
    cout<<"请输入第二个用户的工号:";
    User* anotherUser=choseOneUser();
    if(anotherUser==NULL){
        return false;
    }
    if(oneUser->getJobNum()==anotherUser->getJobNum()){
        cout<<"不能选择两个相同的用户"<<endl;
        return false;
    }
    vector<Order>temp;

    // 获取用户的借阅
    vector<Order> orders1= Order::getAssignUserBorrowingList(oneUser->getFirstOrderId());
    temp=Order::getAssignUserBorrowedHistory(oneUser->getFirstOrderId());
    orders1.insert(orders1.end(),temp.begin(),temp.end());

    vector<Order> orders2= Order::getAssignUserBorrowingList(anotherUser->getFirstOrderId());
    temp=Order::getAssignUserBorrowedHistory(anotherUser->getFirstOrderId());
    orders2.insert(orders2.end(),temp.begin(),temp.end());

    // 把用户的借阅记录变成用户借过的书籍
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
        cout<<"工号"<<oneUser->getJobNum()<<" 与工号"<<anotherUser->getJobNum()<<" 共同借阅如下表"<<endl;
        Book::printBookList(Books);
    }else{
        cout<<"工号"<<oneUser->getJobNum()<<" 与工号"<<anotherUser->getJobNum()<<" 的用户没有共同借阅"<<endl;
    }
    return true;
}
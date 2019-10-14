#include "../../include/model/User.h"
#include "../../include/model/Book.h"
#include "../../include/model/BookInstance.h"
#include "../../include/core/SimpleString.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>

using namespace std;

const int User::lendDays[3] = {30, 60, 90};
const int User::lendNums[3] = {30, 60, 90};

User::User()
{
}


User::~User() {
}


vector<string> User::serialize() {
    // todo: 这里等他们的函数声明给我们
    vector<string> info;
////    info.push_back(to_string(this->id));
//    info.push_back(to_string(this->jobNum));
//
//    info.push_back(to_string(this->type));
//    info.push_back(password);
    return info;
}

bool User::deSerialize(vector<string> info) {
    //todo: 这里等他们的函数声明给我们
//    int id = stoi(info[0]);
//    int jobNum = stoi(info[1]);
//    int borrowNum = stoi(info[2]);
//    status type;
//    type = Teacher;// todo:这个枚举要怎么操作
//    string password = info[4];
//    User(id, jobNum, borrowNum, type, password);
    return true;
}

std::string User::statuEnumToString(status statu) {
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    return strs[statu];
}

status User::stringEnumToStatu(std::string str) {
    for (int i = 0; i < str.size(); ++i) { // 转化为大写
        str[i] = toupper(str[i]);
    }
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    for (int i = 0; i < sizeof(strs) / sizeof(strs[0]); i++) {
        if (str == strs[i]) {
            return status(i);
        }
    }
    return Undergraduate;//没找到默认返回的,//todo:或许不该这么写
}

User::User(int jobNum, status type, const string &name, const string &password) : jobNum(jobNum), type(type),
                                                                                  name(name), password(password) {}

bool User::setPassword(const string &password) {
    // todo:待实现加密函数
    this->password = password;
    return true;
}

bool User::isLegalPassword(const std::string &password) {
    // todo:待实现验证加密函数
    return (password == this->password);
}

bool User::changePwd(const std::string &password) {
    // 输入原来的密码

    // 判断原来的密码是否正确，不正确则跳到上面重新输入

    // 若正确，则输入新的密码
    return this->setPassword(password);
}

User User::login(std::string name, std::string password) {

    return User();
}

User User::checkUserExist(int jobNum) {
    DbAdapter db("User");
//    vector<vector<string>> results= db.searchBySingleField("jobNum",to_string(jobNum));
//    if (results.size()>0){
//        return
//    }
    return User();
}

std::vector<Order> User::getBorrowedHistory() {
    return Order::getAssignUserBorrowedHistory(this->jobNum);
}

std::vector<Order> User::getBorrowingList() {
    return Order::getAssignUserBorrowingList(this->jobNum);
}

bool User::addUsers(std::vector<std::vector<std::string>> queryData, std::vector<long long> &ids) {
    DbAdapter dbAdapter("User");
    dbAdapter.insert(queryData, ids);
    return true;
}

int User::isAllowedLogin() {
    // 判断用户状态,是否被禁止登陆
    if (this->type == status::Ban)
        return 1;
    // 判断用户所有订单的状态,是否有超时订单
    if (!Order::getAssignUserOweOrder(this->firstOrderId).empty())
        return 2;
    return 0;
}

bool User::borrowAssignBookInstance(long long bookInstanceId) {
    // 判断用户能否借书(是否有权借阅,借书数量是多少)
    if (Order::getAssignUserBorrowingList(this->firstOrderId).size() >= User::lendNums[this->type]) {
        return 1; // 返回1,借书数量已达上线
    }
    // 判断该书是否能被借阅
    BookInstance instance = BookInstance::getInstanceById(bookInstanceId);

//    if(instance == NULL){
//        return 5; // 返回5,图书不存在
//    }
//    BookInstance instance=instances[0];

//    if(instances[0].status==)

    // 插入一条借阅记录Order,需要能借多久,
    Order order();
    // 设置该Bookinstance不可借,并更新应还时间
    return false;
}




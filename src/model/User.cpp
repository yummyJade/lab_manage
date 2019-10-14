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
    // todo: ��������ǵĺ�������������
    vector<string> info;
////    info.push_back(to_string(this->id));
//    info.push_back(to_string(this->jobNum));
//
//    info.push_back(to_string(this->type));
//    info.push_back(password);
    return info;
}

bool User::deSerialize(vector<string> info) {
    //todo: ��������ǵĺ�������������
//    int id = stoi(info[0]);
//    int jobNum = stoi(info[1]);
//    int borrowNum = stoi(info[2]);
//    status type;
//    type = Teacher;// todo:���ö��Ҫ��ô����
//    string password = info[4];
//    User(id, jobNum, borrowNum, type, password);
    return true;
}

std::string User::statuEnumToString(status statu) {
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    return strs[statu];
}

status User::stringEnumToStatu(std::string str) {
    for (int i = 0; i < str.size(); ++i) { // ת��Ϊ��д
        str[i] = toupper(str[i]);
    }
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    for (int i = 0; i < sizeof(strs) / sizeof(strs[0]); i++) {
        if (str == strs[i]) {
            return status(i);
        }
    }
    return Undergraduate;//û�ҵ�Ĭ�Ϸ��ص�,//todo:��������ôд
}

User::User(int jobNum, status type, const string &name, const string &password) : jobNum(jobNum), type(type),
                                                                                  name(name), password(password) {}

bool User::setPassword(const string &password) {
    // todo:��ʵ�ּ��ܺ���
    this->password = password;
    return true;
}

bool User::isLegalPassword(const std::string &password) {
    // todo:��ʵ����֤���ܺ���
    return (password == this->password);
}

bool User::changePwd(const std::string &password) {
    // ����ԭ��������

    // �ж�ԭ���������Ƿ���ȷ������ȷ������������������

    // ����ȷ���������µ�����
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
    // �ж��û�״̬,�Ƿ񱻽�ֹ��½
    if (this->type == status::Ban)
        return 1;
    // �ж��û����ж�����״̬,�Ƿ��г�ʱ����
    if (!Order::getAssignUserOweOrder(this->firstOrderId).empty())
        return 2;
    return 0;
}

bool User::borrowAssignBookInstance(long long bookInstanceId) {
    // �ж��û��ܷ����(�Ƿ���Ȩ����,���������Ƕ���)
    if (Order::getAssignUserBorrowingList(this->firstOrderId).size() >= User::lendNums[this->type]) {
        return 1; // ����1,���������Ѵ�����
    }
    // �жϸ����Ƿ��ܱ�����
    BookInstance instance = BookInstance::getInstanceById(bookInstanceId);

//    if(instance == NULL){
//        return 5; // ����5,ͼ�鲻����
//    }
//    BookInstance instance=instances[0];

//    if(instances[0].status==)

    // ����һ�����ļ�¼Order,��Ҫ�ܽ���,
    Order order();
    // ���ø�Bookinstance���ɽ�,������Ӧ��ʱ��
    return false;
}




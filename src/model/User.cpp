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
    vector<string> info;
    info.push_back(to_string(this->jobNum));
    info.push_back(to_string(this->type));
    info.push_back(this->name);
    info.push_back(this->password);
    info.push_back(to_string(this->firstOrderId));
    return info;
}

bool User::deSerialize(vector<string> info) {
    long long jobNum = stoll(info[0]);
    status type = static_cast<status>(stoi(info[1]));

    string name = info[2];
    string password = info[3];
    int firstOrderId = stoi(info[4]);
    new(this) User(jobNum, type, name, password, firstOrderId);
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

User::User(long long jobNum, status type, const string &name, const string &password) : jobNum(jobNum), type(type),
                                                                                        name(name), password(password) {
    this->firstOrderId = -1;
}

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

bool User::checkUserExist(long long jobNum, User *user = NULL) {
    DbAdapter db("User");
    vector<vector<string>> results = db.searchBySingleField("jobNum", to_string(jobNum));

    if (results.size() > 0) {
        if (user != NULL) {
            User temp;
            temp.deSerialize(results[0]);
            *user = temp;
        }
        return true;
    }
    return false;
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
    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);

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

bool User::importUsers() {
    /* 读取一个有效路径,并打开其对应的文件*/
    string path;
    ifstream fin;
    string line;
    while (true) {
        path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
        fin = ifstream(path);//打开文件流操作
        if (fin.good()) {
            cout << "已找到文件,正在读取" << endl;
            break;
        }
        cout << "文件不存在,请检查路径后重新输入" << endl;
    }

    int index = 0;//要操作的行下标
    fin.clear();
    fin.seekg(0, ios::beg); // 重新跳转到文件头部
    getline(fin, line); // 吃掉首行
    vector<vector<string>> users; // 要insert到User表的数据
    vector<long long> existUsers; //已经存在的用户工号


    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
    {
        istringstream sin(line);

        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
        long long jobNum = stoll(fields[0]);
        string name = fields[1];
        string pwd = fields[2];
        int state = stoi(fields[3]);

        if (User::checkUserExist(jobNum)) {
            existUsers.push_back(jobNum);
        } else {
            pwd = User::encryPassword(pwd);
            User user(jobNum, static_cast<status>(state), name, pwd);
            users.push_back(user.serialize());
        }
    }
    vector<long long> ids;
    User::addUsers(users, ids);
    cout << "插入成功,以下工号的用户已经存在" << endl;
    for (int i = 0; i < existUsers.size(); ++i) {
        cout << existUsers[i] << endl;
    }
    return true;
}


std::string User::encryPassword(std::string pwd) {
    return "123456";
}

User::User(long long int jobNum, status type, const string &name, const string &password, long long int firstOrderId)
        : jobNum(jobNum), type(type), name(name), password(password), firstOrderId(firstOrderId) {}




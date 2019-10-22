#include "../../include/model/User.h"
#include "../../include/model/Book.h"
#include "../../include/model/BookInstance.h"
#include "../../include/core/SimpleString.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>
#include <util/TableRenderer.h>

using namespace std;

const int User::lendDays[] = {0, 90, 60, 30, 0};
const int User::lendNums[] = {0, 90, 60, 30, 0};

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

User User::login(long long jobNum, std::string password) {

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
    if (this->type < 0)
        return 1;
    // 判断用户所有订单的状态,是否有超时订单
    if (!Order::getAssignUserOweOrder(this->firstOrderId).empty())
        return 2;
    return 0;
}

int User::borrowAssignBookInstance(int bookInstanceId) {
    // 判断用户能否借书(是否有权借阅,借书数量是多少)
    cout << "firstOrder is " << this->getFirstOrderId();
    cout << "用户借阅量" << Order::getAssignUserBorrowingList(this->getFirstOrderId()).size() << endl;

    cout << "用户可借阅量" << this->getCanLendNums() << endl;
    if (Order::getAssignUserBorrowingList(this->firstOrderId).size() >= this->getCanLendNums()) {
        return 1; // 返回1,借书数量已达上线
    }

    // 判断该书是否能被借阅,
    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);
    if (instance == NULL) {
        return 5; // 返回5,图书不存在
    } else if (instance->status != 1) {
        return 2; // 返回2,该书不是可借
    }

    // 插入一条借阅记录Order,需要(用户工号,书实例id,借书时间,预计归还时间,订单状态)
    Order order(this->getJobNum(), bookInstanceId, SimpleTime::nowTime(),
                SimpleTime::nowTime().addDay(this->getCanLendDays()),
                static_cast<Status>(1));

    int orderId = Order::addSingleOrder(this->getFirstOrderId(), order);

    // 设置该Bookinstance不可借,并更新应还时间
    instance->setStatus(2);
    instance->setPlanReturnDate(Date::today().addDay(this->getCanLendDays()));
    BookInstance::updateStateAndReturnTimeById(*instance);

    // 判断是否首次借阅,是的话更新借阅链表头的字段
    if (this->getFirstOrderId() == -1) {
        User::updateUsersAssignField("jobNum", to_string(this->jobNum), "firstOrderId", to_string(orderId));
    }
    return 0;
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

User::User(long long int jobNum, status type, const string &name, const string &password, int firstOrderId)
        : jobNum(jobNum), type(type), name(name), password(password), firstOrderId(firstOrderId) {}


std::string User::getUserMeaasge() {
    // 检测是否有正在预约书籍


    // 检测是否有预约已到书籍

    // 检测是否有即将逾期的借阅
    vector<Order> boringOrders = Order::getAssignUserBorrowingList(this->firstOrderId);
    for (int i = 0; i < boringOrders.size(); ++i) {

    }


    return std::__cxx11::string();
}

bool User::appointmentAssignBook(int jobNum, int bookId) {
    // 判断用户是否超过最大预约数

    // 判断该书是否可被预约(是否没有状态为可借阅的instance)

    // 创建预约Order

    // 更新Book表中预约数量

    return false;
}

std::vector<std::string> User::getPrintLineStr() {
    vector<string> info;
    info.push_back(to_string(this->jobNum));
    info.push_back(this->name);
    info.push_back(User::statuEnumToString(this->type));
    return info;
}

void User::printUserList(std::vector<User> users) {
    vector<string> navs = {"工号", "姓名", "账号类型"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < users.size(); ++i) {
        render.addColume(users[i].getPrintLineStr());
    }
    render.render();
}

User User::getUserByJobNum(long long jobNum) {
    DbAdapter db("User");
    vector<vector<string>> results = db.searchBySingleField("jobNum", to_string(jobNum));
    User user;
    if (results.size() == 0) {
        cout << "错误,该用户不存在,该接口调用需保证用户存在";
    } else {
        user.deSerialize(results[0]);
    }

    return user;
}

const int *User::getLendDays() {
    return lendDays;
}

const int *User::getLendNums() {
    return lendNums;
}

long long int User::getJobNum() const {
    return jobNum;
}

status User::getType() const {
    return type;
}

const string &User::getName() const {
    return name;
}

const string &User::getPassword() const {
    return password;
}

int User::getFirstOrderId() const {
    return firstOrderId;
}

bool User::updateUsersAssignField(std::string assignField, std::string assignValue, std::string changeField,
                                  std::string changeValue) {
    DbAdapter dbAdapter("User");
    dbAdapter.updateBySingleField(assignField, assignValue, changeField, changeValue);
    return true;
}

void User::setJobNum1(long long int jobNum) {
    User::jobNum = jobNum;
}

void User::setType(status type) {
    User::type = type;
}

void User::setName(const string &name) {
    User::name = name;
}

void User::setFirstOrderId(long long int firstOrderId) {
    User::firstOrderId = firstOrderId;
}

User::User(long long int jobNum, status type, const string &name) : jobNum(jobNum), type(type), name(name) {
    this->firstOrderId = -1;
    this->setPassword(to_string(this->jobNum));
}

vector<User> User::searchUsersBySingleField(std::string field, std::string value) {
    DbAdapter dbAdapter("User");
    vector<vector<string> > queryData = dbAdapter.searchBySingleField(field, value);
    cout << "检索到" << queryData.size() << endl;
    return User::stringsToUsers(queryData);
}

std::vector<User> User::stringsToUsers(std::vector<std::vector<std::string>> users) {
    vector<User> results;
    for (int i = 0; i < users.size(); ++i) {
        User user; // todo: 这里好像要用new
        user.deSerialize(users[i]);
        results.push_back(user);
    }
    return results;
}

vector<User> User::searchAll() {
    vector<User> result;
    DbAdapter dbAdapter("User");
    vector<vector<string>> infos = dbAdapter.searchAll();
    for (int i = 0; i < infos.size(); ++i) {
        User user;
        user.deSerialize(infos[i]);
        result.push_back(user);
    }
    return result;
}

int User::getCanLendDays() {
    return lendDays[this->getType()];
}

int User::getCanLendNums() {
    return lendNums[this->getType()];
}

int User::returnAssignOrder(Order order) {
    // 修改订单状态
    order.setStatu(static_cast<Status>(2));
    order.setReturnTime(SimpleTime::nowTime());
    Order::updateStateAndReturnTimeById(order);

    // 获取图书实例
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    // 判断图书是否被预约了
    Book book = Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
    if (book.getAppointmentNum() > 0) {//被预约了,特殊处理
        //todo: 处理预约操作,找到那个预约单子,给他改一下

    } else {// 没被预约,归还图书馆
        // 修改书的实例的状态(设为可借)
        instance->setStatus(1);
        BookInstance::updateStateAndReturnTimeById(*instance);
        cout << "还书成功!" << endl;
    }
    return 0;
}

int User::renewAssignOrder(Order order) {
    // 判断是否可续借
    if (order.getStatu() != 1) {
        cout << "该借阅已不可再续借" << endl;
        return 1;//该借阅不可续借
    }

    // 获取图书实例
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    // 判断图书是否被预约了
    Book book = Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
    if (book.getAppointmentNum() > 0) {//被预约了,不允许预约
        cout << "该书已经被预约,不允许续借" << endl;
        return 2;
    } else {// 没被预约,可以续借
        // 修改订单状态
        order.setStatu(static_cast<Status>(4));// 修改状态为4
        SimpleTime old = (SimpleTime &&) order.getReturnTime();//获取旧的应还日期
        order.setReturnTime(old.addDay(this->getCanLendDays()));
        Order::updateStateAndReturnTimeById(order);
        cout << "续借成功" << endl;
    }


    return 0;
}






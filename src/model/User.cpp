#include "../../include/model/User.h"
#include "../../include/model/Book.h"
#include "../../include/model/BookInstance.h"
#include "../../include/core/SimpleString.h"
#include "../../include/core/MD5.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>
#include <util/TableRenderer.h>

using namespace std;

const int User::lendDays[] = {0, 90, 60, 30, 0};
const int User::lendNums[] = {0, 90, 60, 30, 0};
const int User::appointNums[] = {0, 9, 6, 3, 0};

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
    this->password = calcMD5(password);
    User::updateUsersAssignField("jobNum", to_string(this->getJobNum()), "password", this->getPassword());
	return true;
}

bool User::isLegalPassword(const std::string &password) {
    return (calcMD5(password) == this->password);
}

bool User::changePwdService() {
    string pwd;
    // 输入原来的密码
    do {
        cout << "请输入原密码(输入0取消修改):";
        cin >> pwd;
        if (pwd == "0") {
            return false;
        }
        if (this->isLegalPassword(pwd)) {
            break;
        } else {
            cout << "原密码错误" << endl;
        }
    } while (true);

    // 若正确，则输入新的密码  todo: 做一个密码长度限制
    cout << "请输入新密码:";
    cin >> pwd;
    this->setPassword(pwd);

    cout << "修改成功";

    return true;
}

int User::login(long long jobNum, std::string password, User *user = NULL) {
	
    if (checkUserExist(jobNum, user)) {
        if (user->isLegalPassword(password)) {// 判断用户名是否正确
            if (user->getType() < 0)// 判断用户状态,是否被禁止登陆
                return 3;// 返回3,用户被禁止登陆(挂失\冻结)

            // 判断用户所有订单的状态,是否有超时订单
            if (!Order::getAssignUserOweOrder(user->getFirstOrderId()).empty()) {
                cout << "该用户有逾期借阅,需要处理后才能登陆" << endl;
                cout << "输入Y处理逾期,N取消登录:";
                char operate;
                cin >> operate;
                if (operate == 'Y' || operate == 'y') {
                    // 处理逾期情况
                    if (user->dealWithOverTimeOrder() == 0) {
                        return 0;// 登陆成功
                    }
                }
                return 4;// 返回4,有逾期借阅,禁止登陆
            }
            return 0; // 返回0,登陆成功
        } else {
            return 2;//返回2,密码错误
        }
    } else {
        return 1;// 返回1,用户不存在
    }

}

bool User::checkUserExist(long long jobNum, User *user = NULL) {
    DbAdapter db("User");
    vector<vector<string>> results = db.searchBySingleField("jobNum", to_string(jobNum));

    if (results.size() > 0) {
		if (user != NULL) {
			*user = User(User::stringsToUsers(results)[0]);
		}
		
		//cout << "password is" << user->getPassword()<<endl;
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
    /*cout << "firstOrder is " << this->getFirstOrderId();
    cout << "用户借阅量" << Order::getAssignUserBorrowingList(this->getFirstOrderId()).size() << endl;
    cout << "用户可借阅量" << this->getCanLendNums() << endl;*/

    if (Order::getAssignUserBorrowingList(this->firstOrderId).size() >= this->getCanLendNums()) {
		cout << "借书失败,借书数量已达上限" << endl;
        return 1; // 返回1,借书数量已达上线
    }

    // 判断该书是否能被借阅,
    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);
    if (instance == NULL) {
		cout << "借书失败,该图书不存在"<<endl;
        return 5; // 返回5,图书不存在
    } else if (instance->status != 1) {
		cout << "借书失败,该图书当前不可被借" << endl;
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
		this->setFirstOrderId(orderId);
        User::updateUsersAssignField("jobNum", to_string(this->jobNum), "firstOrderId", to_string(orderId));
    }
	cout << "借书成功" << endl;
    return 0;
}

bool User::importUsers(string incomingPath="") {
	string path;
	ifstream fin;
	string line;
	if (incomingPath != "") {
		fin = ifstream(path);//打开文件流操作
	}
	else {
		/* 读取一个有效路径,并打开其对应的文件*/
		while (true) {
			path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
			fin = ifstream(path);//打开文件流操作
			if (fin.good()) {
				cout << "已找到文件,正在读取" << endl;
				break;
			}
			cout << "文件不存在,请检查路径后重新输入" << endl;
		}
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
    return calcMD5(pwd);
}

User::User(long long int jobNum, status type, const string &name, const string &password, int firstOrderId)
        : jobNum(jobNum), type(type), name(name), password(password), firstOrderId(firstOrderId) {}


bool User::getUserMessage() {
    printf("--------------------------------------------------------\n");
    // 检测是否有预约已到的书籍
    vector<Order> AppointingOrders = Order::getAssignUserArrivedAppointmentList(this->getFirstOrderId());
    if(!AppointingOrders.empty()){//有预约已到内容
        cout<<"您有"<<AppointingOrders.size()<<"本预约的书籍已到,请及时到图书馆领取"<<endl;
        Order::printOrderList(AppointingOrders);
    }

    // 检测是否有即将逾期的借阅
    vector<Order> borrowingOrders = Order::getAssignUserBorrowingList(this->getFirstOrderId());
    vector<Order> soonTimeOutOrders;
    for (int i = 0; i < borrowingOrders.size(); ++i) {
        SimpleTime returnTime = borrowingOrders[i].getReturnTime();
        if (returnTime.compare(Date::today().addDay(3)) <= 0) {// 3天内到期的书
            soonTimeOutOrders.push_back(borrowingOrders[i]);
        }
    }
    if(!soonTimeOutOrders.empty()){//有预约已到内容
        cout<<"您有"<<soonTimeOutOrders.size()<<"本借阅的书籍将于3天内到期,请注意及时归还"<<endl;
        Order::printOrderList(soonTimeOutOrders);
    }
    printf("--------------------------------------------------------\n");
    return true;
}

// todo:返回类型改一下
bool User::appointmentAssignBook(int bookId, std::string isbn) {
    // 判断用户是否超过最大预约数
    if(Order::getAssignUserAppointmentList(this->firstOrderId).size() >= this->getCanAppointNums()){
        cout << "超过最大预约数" << endl;
        return 1;
    }
    //判断是否超过可借书的数量（可借数量+已预约的数量）
    if((Order::getAssignUserBorrowingList(this->firstOrderId).size() + Order::getAssignUserAppointmentList(this->firstOrderId).size()) >= this->getCanLendNums()){
        cout << "超过最大可借数" << endl;
        return 2;
    }
    // todo:判断该用户是否已经借阅了或者预约了这本书

    // 判断该书是否可被预约(是否没有状态为可借的且没有下架) 用bookid查bookinstance
    if(!BookInstance::checkAssignBookCanAppointmentInstanceExist(isbn)){
        cout << "不可逾越" << endl;
        return 3;
    }
    // 创建预约Order（用户号，书本唯一标识isbn(xbookId)，当前预约时间，状态为借阅）
    Order order(this->getJobNum(), bookId, SimpleTime::nowTime(), SimpleTime::nowTime(), static_cast<Status>(3));
    //持久化

    int orderId = Order::addSingleOrder(this->getFirstOrderId(), order);
    cout << "在这里" << endl;
    // 更新Book表中预约数量，为增加，其实这个函数很没用，不过我不知道怎么样把book持久化到数据库，update么

    Book::updateBooksAppointmentNum(isbn,1);
    cout << "在这里" << endl;
    cout << "预约成功，请等待到书通知" << endl;

    return true;
}

//bool User::getArrivedAppointment(){
//    //todo:用户领取预约已到的书籍
//
//}
std::vector<std::string> User::getPrintLineStr() {
    vector<string> info;
    info.push_back(to_string(this->jobNum));
    info.push_back(this->name);
	info.push_back(this->getTypeStr());
    //info.push_back(User::statuEnumToString(this->type));
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

const int *User::getAppointNums(){
    return appointNums;
}

long long int User::getJobNum() const {
    return jobNum;
}

status User::getType() const {
    return type;
}

std::string User::getTypeStr() {
	string strs[] = { "管理员","教师","研究生","本科生" };
	string result = "";
	int type = this->getType();
	if (type < 0) {
		type = -type;
		result = "冻结的";
	}
	return result+strs[type];
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
        User user; 
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

int User::getCanAppointNums() {
    return appointNums[this->getType()];
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
        //处理预约操作,找到那个预约单子,给他改一下
        //找到所有满足bookid==isbn && status = 3的单子
        //todo:注意一下这个string到int转换合不合法,不合法
        vector<Order> orders = Order::getAssignBookAppointingList(book.getId());
        int earliestIndex = 0;       //借阅时间最早的下标
        SimpleTime earliestDate = orders[0].getBorrowTime();         //记录时间最早那一天，这里的初值付给order里面第一个也行，
        for (int i = 1; i < orders.size(); ++i) {
            //接下来比较时间前后，找到最小？大的那个，
            if(earliestDate.compare(orders[i].getBorrowTime()) > 0) {    //大于0说明当前时间比compare里的要大，也就是要靠后
                earliestIndex = i;
                earliestDate = orders[i].getBorrowTime();
            }
        }
        //找到最大的以后更改对应订单的信息，将bookId修改为当前instanceId，将status改为5
        orders[earliestIndex].setStatu(static_cast<Status>(5));
        //存的实例id就是这个么？
        orders[earliestIndex].setBookId(instance->getId());
        Order::updateStateAndBookIdById(orders[earliestIndex]);

        //修改BookInstance的状态status为5,这里只修改了一项函数会不会报错
        instance->setStatus(5);
        //todo:记得新增一个修改单项status的函数
        BookInstance::updateStateAndReturnTimeById(*instance);

        //修改Book预约人数减一
        Book::updateBooksAppointmentNum(instance->getIsbn(),-1);


    } else {// 没被预约,归还图书馆
        // 修改书的实例的状态(设为可借)
		int oldState = instance->getStatus();
		if (oldState != 3) { // 状态为3 表示已经下架,还书后不修改状态
			instance->setStatus(1);
			BookInstance::updateStateAndReturnTimeById(*instance);

		}else{
            cout<<"该书已被标志为下架,由管理员回收,不流回图书馆"<<endl;
		}
        
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

bool User::getArrivedAppointment(Order order) {
    //现在这本书真正归这个人所有了，所以首先要修改订单信息
    order.setStatu(static_cast<Status>(1));
    order.setBorrowTime(SimpleTime::nowTime());
    //其实设置这个时间，我个人认为没有意义
    order.setReturnTime(SimpleTime::nowTime().addDay(this->getCanLendDays()));
    Order::updateStateAndReturnTimeAndLendTimeById(order);
    //修改bookInstance的应还时间以及status状态
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    instance->setStatus(2);
    instance->setPlanReturnDate(Date::today().addDay(this->getCanLendDays()));
    BookInstance::updateStateAndReturnTimeById(*instance);
    cout << "您的图书已领取！" << endl;
	return true;
}


int User::dealWithOverTimeOrder() {
    // 打印逾期的借阅
    vector<Order> OweOrders = Order::getAssignUserOweOrder(this->getFirstOrderId());
    Order::printOrderList(OweOrders);

    int arrears = 0;//要还的钱,单位分
    int allOverDays = 0;//总的逾期天数
    for (int i = 0; i < OweOrders.size(); ++i) {
        Date planreturnDate = OweOrders[i].getReturnTime().date;
        allOverDays += Date::today().compare(planreturnDate);
    }

    printf("总共逾期%d本书,共计逾期时间%d天,应缴%.2f元罚款\n", OweOrders.size(), allOverDays, allOverDays * 0.03);
    printf("输入Y缴纳罚款并归还书籍,N取消操作");
    char operate;
    cin >> operate;
    if (operate == 'Y' || operate == 'y') {
        for (int i = 0; i < OweOrders.size(); ++i) {
            // 还书
            this->returnAssignOrder(OweOrders[i]);
        }
        printf("缴纳罚款成功,逾期书籍已归还\n");
        return 0; // 已缴纳罚款
    }
    return 1;// 未缴纳罚款


}






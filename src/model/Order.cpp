#include "../../include/model/Order.h"
#include "../../linkDatebase/include/Record.h"
#include "../../linkDatebase/include/TableRecord.h"
#include <vector>
#include <iostream>
#include <string>
#include <util/TableRenderer.h>
#include <model/BookInstance.h>
#include <model/Book.h>
#include <model/User.h>


using namespace std;
Order::Order()
{
}


Order::~Order()
{
}

std::vector<std::string> Order::serialize() {
    std::vector<std::string> info;
//    info.push_back(to_string(this->id));
    info.push_back(std::to_string(this->bookId));
    info.push_back(this->borrowTime.serialize());
    info.push_back(this->returnTime.serialize());

    return info;
}



std::string Order::statuEnumToString(Status statu) {
    std::string strs[] = {"BORROWING", "RETURNED"};
    return strs[statu];
}

Status Order::stringEnumToStatu(std::string str) {
    for (int i = 0; i < str.size(); ++i) { // 转化为大写
        str[i] = toupper(str[i]);
    }
    std::string strs[] = {"BORROWING", "RETURNED"};
    for (int i = 0; i < sizeof(strs) / sizeof(strs[0]); i++) {
        if (str == strs[i]) {
            return Status(i);
        }
    }
    return BORROWING;//没找到默认返回的,//todo:或许不该这么写
}

Order::Order(long long userId, int bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime,
             Status statu) : userId(
        userId), bookId(bookId), borrowTime(borrowTime), returnTime(returnTime), statu(statu) {}




std::vector<Order> Order::getAllBorrowedHistory() {
    TableRecord *table = TableRecord::getInstance();
    int borrowIndex = 1;        //在借状态为3
    vector<Record> copys = table->queryByStatus(borrowIndex);
    vector<Order> result;
    for(int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}

std::vector<Order> Order::getAllAppointArrivedHistory() {
    TableRecord *table = TableRecord::getInstance();
    int appointArrivedIndex = 5;
    vector<Record> copys = table->queryByStatus(appointArrivedIndex);
    vector<Order> result;
    for(int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
	return result;
}
std::vector<Order>  Order::getAllBorrowedOweList() {
    //先获取所有的借阅记录
    vector<Order> orders = Order::getAllBorrowedHistory();
    vector<Order> result;
    //挑选出returnTime和今天相比小的
    SimpleTime nowTime = SimpleTime::nowTime();
    SimpleTime returnTime;
    for(int i = 0; i < orders.size(); ++i) {
        returnTime = orders[i].getReturnTime();
        if(returnTime.compare(Date::today()) < 0) {
            result.push_back(orders[i]);
        }
    }
    return result;
}

std::vector<Order> Order::getAllAppointArrivedOweList() {
    //先获取所有的预约已到记录
    vector<Order> orders = Order::getAllAppointArrivedHistory();
    vector<Order> result;
    //挑选出lendTime比今天小的
    SimpleTime getBookTime;
    for(int i = 0; i < orders.size(); ++i) {
        getBookTime = orders[i].getBorrowTime();
        if(getBookTime.compare(Date::today()) < 0) {
            result.push_back( orders[i]);
        }
    }
    return result;
}

std::vector<Order> Order::getAssignBookBorrowedHistory(int bookId) {
    TableRecord *table = TableRecord::getInstance();
    vector<Record> copys = table->queryByBookId(bookId);
    vector<Order> result;
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}


std::vector<Order> Order::getAssignBookAppointingList(int bookId) {
    // 先获取所有的借阅记录
    vector<Order> orders = Order::getAssignBookBorrowedHistory(bookId);
//    cout << "已获取到所有记录:" << orders.size() << endl;
    vector<Order> result;
    int appointmentIndex = 3;       //表示所有的在预约状态
    for (int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentIndex){
            result.push_back(orders[i]);
        }
    }
    return result;
}

std::vector<Order> Order::getAssignBookOweAppointing(int bookId) {
    //先获取所有的在预约记录
    vector<Order> orders = Order::getAssignBookBorrowedHistory(bookId);
    vector<Order> result;
    int appointmentArrivedIndex = 5;       //表示所有的预约到了
    SimpleTime appointArrivedInValidTime;
    for (int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentArrivedIndex) {
            //接下来找出所有预约超期的书籍
            //判断该订单的到期时间与当前时间的关系，若到期时间小于当前时间，代表逾期，返回结果
            appointArrivedInValidTime = orders[i].getBorrowTime();       //预约失效时间
            if(appointArrivedInValidTime.compare(SimpleTime::nowTime()) < 0) {
                result.push_back(orders[i]);
            }
        }
    }
	return result;
}

//1代表在借，2代表已还，3代表预约，4代表已续借的在借，5代表预约已到
std::vector<Order> Order::getAssignUserBorrowedHistory(int firstOrderId) {
    TableRecord *table = TableRecord::getInstance();
	//cout << "firstID is" << firstOrderId << endcoul;
    vector<Record> copys = table->queryByPerson(firstOrderId);
    vector<Order> result ;
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}


std::vector<Order> Order::getAssignUserCompleteOrders(int firstOrderId) {
	// 获取所有借阅记录
	vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
	vector<Order> result;
	int borringIndexs[] = { 2 }; // 已经还
	for (int i = 0; i < orders.size(); ++i) {
		for (int j = 0; j < sizeof(borringIndexs) / sizeof(int); ++j) {
			if (orders[i].statu == borringIndexs[j]) {
				result.push_back(orders[i]);
				break;
			}
		}
	}
	//cout << "result size " << result.size() << endl;
	return result;
}

std::vector<Order> Order::getAssignUserBorrowingList(int firstOrderId) {
    // 获取所有借阅记录
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    //cout << "已获取到所有记录:" << orders.size() << endl;
    vector<Order> result;
    int borringIndexs[] = {1, 4}; // 在借状态statu的值集合
    for (int i = 0; i < orders.size(); ++i) {
        for (int j = 0; j < sizeof(borringIndexs) / sizeof(int); ++j) {
            if (orders[i].statu == borringIndexs[j]) {
                result.push_back(orders[i]);
                break;
            }
        }
    }
    return result;
}


std::vector<Order> Order::getAssignUserAppointmentList(int firstOrderId){
//    //获取所有的在预约状态
//    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
//    vector<Order> result ;
////    cout << orders.size() << endl;
//    int appointmentIndexs[] = {3, 5};      //表示的是在预约的状态，包括在预约以及预约已到未取
//
//    for(int i = 0; i < orders.size(); ++i) {
//        for(int j = 0; j < sizeof(appointmentIndexs) / sizeof(int); ++j) {
//            if(orders[i].statu == appointmentIndexs[j]) {
//                result.push_back(orders[i]);
//                break;
//            }
//        }
//    }
//    //如果为空的处理？
////    cout << result.size() << endl;
//    return result;


    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    vector<Order> result ;
//    cout << orders.size() << endl;
    int appointmentIndexs[] = {3, 5};      //表示的是在预约的状态，包括在预约以及预约已到未取
    int appointmentArrivedIndex = 5;
    int appointmentIndex = 3;
    SimpleTime appointArrivedInValidTime;
    for(int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentIndex) {
            result.push_back(orders[i]);
        } else if(orders[i].statu == appointmentArrivedIndex) {
            //判断该订单的到期时间与当前时间的关系，若到期时间大于等于当前时间，代表还未过期，返回结果
            appointArrivedInValidTime = orders[i].getBorrowTime();       //预约失效时间
            if (appointArrivedInValidTime.compare(SimpleTime::nowTime()) >= 0) {
                result.push_back(orders[i]);
            }
        }
    }

    return result;
}


std::vector<Order> Order::getAssignUserArrivedAppointmentList(int firstOrderId) {
    //先获取所有的在预约状态
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    vector<Order> result;
    int appointmentIndex = 5;      //表示的是预约已到未取
    SimpleTime appointArrivedInValidTime;
    for(int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentIndex) {
//            result.push_back(orders[i]);
            //判断该订单的到期时间与当前时间的关系，若到期时间大于等于当前时间，代表还未过期，返回结果
            appointArrivedInValidTime = orders[i].getBorrowTime();       //预约失效时间
            if(appointArrivedInValidTime.compare(SimpleTime::nowTime()) >= 0){
                result.push_back(orders[i]);
            }else{
//                orders[i].setStatu(static_cast<Status>(6));     //将订单修改成预约超期的状态
//                orders[i].updateStateAndReturnTimeById(orders[i]);      //持久化
//                BookInstance *instance = BookInstance::getInstanceById(orders[i].getBookId());
//                //修改书籍实例的状态为可借
//                instance->setStatus(1);
//                //更新函数爱改不改
//                BookInstance::updateStateAndReturnTimeById(*instance);
            }

        }

    }
    //cout << "result size " << result.size() << endl;
    return result;
}

bool Order::judgeAssignUserAppointmentList(int firstOrderId, int isbn) {
    //获取所有的在预约状态
	return true;
}
std::vector<Order> Order::getAssignUserOweOrder(int firstOrderId) {
    // 先获取所有在借记录
    vector<Order> orders = Order::getAssignUserBorrowingList(firstOrderId);
    vector<Order> result;
    for (int i = 0; i < orders.size(); ++i) {
        // 如果应还时间小于今天,则已逾期
        if (orders[i].returnTime.compare(Date::today()) <= -1) {
            result.push_back(orders[i]);
        }
    }
    return result;
}



int Order::addOneOrder(Order order, int firstId) {
//    TableRecord* table =TableRecord::getInstance();
//    Record record;
//    table->insertData(firstId,record);
    return 0;
}

Record Order::toRecordCopy() {
    Record record;
    record.setId(this->id);
    record.setBookId(this->bookId);
    record.setState(this->statu);
    record.setStId(this->userId);
    record.setBoTime(this->borrowTime.toLLTime());
	//record.setReTime(this->returnTime.toLLTime());
	record.setReTime(this->returnTime.date.toInt());
    return record;
}


Order Order::RecordCopyToOrder(Record record) {
    SimpleTime boTime = SimpleTime::llTimeToSimpleTime(record.getBoTime());
	Date reDate = Date::intDate2Date(record.getReTime());
	SimpleTime reTime = SimpleTime(0, 0, 0, reDate);

    //SimpleTime reTime = SimpleTime::llTimeToSimpleTime(record.getReTime());

    return Order(record.getId(), record.getStId(), record.getBookId(), boTime, reTime,
                 static_cast<Status>(record.getState()));
}

Order::Order(long long int id, long long int userId, int bookId, const SimpleTime &borrowTime,
             const SimpleTime &returnTime, Status statu) : id(id), userId(userId), bookId(bookId),
                                                           borrowTime(borrowTime), returnTime(returnTime),
                                                           statu(statu) {}

int Order::addSingleOrder(int firstId, Order order) {
    TableRecord *table = TableRecord::getInstance();
    return table->insertData(firstId, order.toRecordCopy());
}

void Order::printOrderList(std::vector<Order> orders) {
    vector<string> navs = {"编号","书名", "条码号", "借书时间", "还书/应还时间", "状态"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < orders.size(); ++i) {
		vector<string> line;
		vector<string> temp = orders[i].getPrintLineStr(1);
		line.push_back(to_string(i + 1));
		line.insert(line.end(), temp.begin(), temp.end());
        render.addColume(line);
    }
    render.render();
}

void Order::printOrderOweList(std::vector<Order> orders, int type) {

    vector<string> navs = {"编号", "用户工号", "用户姓名", "书名", "条码号", "借书时间", "还书/应还时间", "状态", "逾期天数"};
	if (type == 3) {
		navs = { "编号", "用户工号", "用户姓名", "书名", "条码号", "预约时间", "最晚预约应取时间", "状态", "逾期天数" };
	}
//    vector<string> navs = {"编号","书名", "条码号", "借书时间", "还书/应还时间", "状态"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < orders.size(); ++i) {
        vector<string> line;
        vector<string> temp = orders[i].getPrintLineStr(type);
        line.push_back(to_string(i + 1));
        line.insert(line.end(), temp.begin(), temp.end());
        render.addColume(line);
    }
    render.render();
}

std::vector<std::string> Order::getPrintLineStr(int type = 1) {
    vector<string> info;
    Book book;
    BookInstance *bookInstance = BookInstance::getInstanceById(this->getBookId());
	string code;
	string planReturnTime;
    SimpleTime date;
    long long userId;
    User user;
    if(this->getStatu()==3){// 如果是预约的话
        book = Book::searchBooksById(this->getBookId());
		code = "无";
        planReturnTime="该图书预约尚未到";
    }else{
            book = Book::searchBooksBySingleField("isbn", bookInstance->getIsbn())[0];
            code = to_string(bookInstance->getId());
            date = this->getReturnTime();
            planReturnTime=date.date.serialize();
    }

    if (this->getStatu()==5) {
        planReturnTime="预约待领取";
    }
    if(type == 2 || type==3) {
        userId = this->getUserId();
        user = User::getUserByJobNum(userId);
        info.push_back(to_string(userId));
        info.push_back(user.getName());
    }
    info.push_back(book.getName());// 书名
    info.push_back(code); //条码号
    date =  this->getBorrowTime();
    info.push_back(date.serialize());
    info.push_back(planReturnTime);
	info.push_back(this->getStatuStr());
	if(type == 2) {
        SimpleTime time = this->getReturnTime();
        int dueDay = time.compare(SimpleTime::nowTime());
        info.push_back(to_string(dueDay*(-1)));
	}else if(type == 3) {
        SimpleTime time = this->getBorrowTime();
        int dueDay = time.compare(SimpleTime::nowTime());
        info.push_back(to_string(dueDay*(-1)));
	}

    return info;
}


std::string Order::getStatuStr() {
	string strs[] = { "", "在借", "已还","预约中", "在借(已续借)","预约已到" };
	return strs[this->getStatu()];
}
long long int Order::getId() const {
    return id;
}

long long int Order::getUserId() const {
    return userId;
}

int Order::getBookId() const {
    return bookId;
}

const SimpleTime &Order::getBorrowTime() const {
    return borrowTime;
}

const SimpleTime &Order::getReturnTime() const {
    return returnTime;
}

Status Order::getStatu() const {
    return statu;
}

void Order::setId(long long int id) {
    Order::id = id;
}

void Order::setUserId(long long int userId) {
    Order::userId = userId;
}

void Order::setBookId(int bookId) {
    Order::bookId = bookId;
}

void Order::setBorrowTime(const SimpleTime &borrowTime) {
    Order::borrowTime = borrowTime;
}

void Order::setReturnTime(const SimpleTime &returnTime) {
    Order::returnTime = returnTime;
}

void Order::setStatu(Status statu) {
    Order::statu = statu;
}

bool Order::updateStateAndReturnTimeById(Order order) {
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {6, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}

bool Order::updateStateAndBookIdAndBorrowTimeById(Order order){
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {2, 5, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}

bool Order::updateStateAndReturnTimeAndLendTimeById(Order order) {
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {2, 6, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}

std::vector<Order> Order::getAllOrders() {
    TableRecord *table = TableRecord::getInstance();
    vector<Record> copys = table->queryAll();
    vector<Order> result ;
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}

std::vector<Order> Order::getAllBorrwedOrders() {
    TableRecord *table = TableRecord::getInstance();
    vector<Order> result ;
    vector<Record> copys = table->queryByStatus(1);//在借
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    copys = table->queryByStatus(2);//已还
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    copys = table->queryByStatus(4);//续借的在借
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}


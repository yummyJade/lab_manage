#include "../../include/model/Order.h"
#include "../../linkDatebase/include/Record.h"
#include "../../linkDatebase/include/TableRecord.h"
#include <vector>
#include <iostream>
#include <string>
#include <util/TableRenderer.h>
#include <model/BookInstance.h>
#include <model/Book.h>


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

//1代表在借，2代表已还，3代表预约，4代表已续借的在借，5代表预约已到
std::vector<Order> Order::getAssignUserBorrowedHistory(int firstOrderId) {
    TableRecord *table = TableRecord::getInstance();
	//cout << "firstID is" << firstOrderId << endl;
    vector<Record> copys = table->queryByPerson(firstOrderId);
    vector<Order> result;
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
    //获取所有的在预约状态
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    vector<Order> result;
    int appointmentIndexs[] = {3, 5};      //表示的是在预约的状态，包括在预约以及预约已到未取
    for(int i = 0; i < orders.size(); ++i) {
        for(int j = 0; j < sizeof(appointmentIndexs) / sizeof(int); ++j) {
            if(orders[i].statu == appointmentIndexs[j]) {
                result.push_back(orders[i]);
                break;
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
    for(int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentIndex) {
            result.push_back(orders[i]);
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
		vector<string> temp = orders[i].getPrintLineStr();
		line.push_back(to_string(i + 1));
		line.insert(line.end(), temp.begin(), temp.end());
        render.addColume(line);
    }
    render.render();
}

std::vector<std::string> Order::getPrintLineStr() {
    vector<string> info;
    Book book;
    BookInstance *bookInstance = BookInstance::getInstanceById(this->getBookId());
	string code;
    if(this->getStatu()==3){// 如果是预约的话
        book = Book::searchBooksById(this->getBookId());
		code = "无";
    }else{
        book = Book::searchBooksBySingleField("isbn", bookInstance->getIsbn())[0];
		code = to_string(bookInstance->getId());
    }
	

    info.push_back(book.getName());// 书名
    info.push_back(code); //条码号
    SimpleTime date =  this->getBorrowTime();
    info.push_back(date.serialize());
    date = this->getReturnTime();
    info.push_back(date.date.serialize());
	info.push_back(this->getStatuStr());

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

bool Order::updateStateAndBookIdById(Order order){
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {4,7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}

bool Order::updateStateAndReturnTimeAndLendTimeById(Order order) {
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {2, 6, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}


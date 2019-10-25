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

    //todo: 枚举的序列化和反序列化
//    info.push_back(this->statu);
    return info;
}

bool Order::deSerialize(std::vector<std::string> info) {
    //todo: 这里要加上异常处理

//    int IsRenew=stoi(info[0]);
//    int bookId=stoi(info[1]);
//    int borrowTime=stoi(info[2]);
//    int returnTime=stoi(info[3]);
//    status type;
//    type=Teacher;// todo:这个枚举要怎么操作
//    string password=info[4];
//
//    Order(id,jobNum,borrowNum,type,password);
    return true;
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


//1代表在借，2代表已还，3代表预约，4代表已续借的在借，5代表预约已到
std::vector<Order> Order::getAssignUserBorrowedHistory(int firstOrderId) {
    TableRecord *table = TableRecord::getInstance();
    vector<Record> copys = table->queryByPerson(firstOrderId);
    vector<Order> result;
//    cout<<"copys size "<<copys.size()<<endl;
    for (int i = 0; i < copys.size(); ++i) {
//        cout<<"开始转化"<<endl;
		Order order = Order::RecordCopyToOrder(copys[i]);
		// cout << "转化后的时间" << order.borrowTime.serialize()<<endl;
		result.push_back(order);
		//result.push_back(Order::RecordCopyToOrder(copys[i]));

//        cout<<"转化成功"<<endl;
    }
//    cout<<"copys end "<<endl;
    return result;
}


std::vector<Order> Order::getAssignUserBorrowingList(int firstOrderId) {
    // 获取所有借阅记录
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    cout << "已获取到所有记录:" << orders.size() << endl;
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
    cout << "result size " << result.size() << endl;
    return result;
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
	//cout << "借书时间是" << record.getBoTime() << endl;
	//cout << "借书时间2是" << boTime.serialize() << endl;
	SimpleTime reTime = SimpleTime(0,0,0,reDate);
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
    vector<string> navs = {"书名", "条码号", "借书时间", "还书/应还时间", "状态"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < orders.size(); ++i) {
        render.addColume(orders[i].getPrintLineStr());
    }
    render.render();
}

std::vector<std::string> Order::getPrintLineStr() {
    vector<string> info;
    BookInstance *bookInstance = BookInstance::getInstanceById(this->getBookId());
//    cout<<"isbn:"<<bookInstance->getIsbn()<<"|||";
    Book book = Book::searchBooksBySingleField("isbn", bookInstance->getIsbn())[0];
    info.push_back(book.getName());
    info.push_back(to_string(bookInstance->getId()));

    SimpleTime date = this->getBorrowTime();

    info.push_back(date.serialize());
    date = this->getReturnTime();
    info.push_back(date.date.serialize());
	info.push_back(this->getStatuStr());
    //info.push_back(to_string(this->getStatu()));//todo:状态改成对应中文
    return info;
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

std::string Order::getStatuStr() {
	string strs[] = { "", "在借", "已还","预约", "已续借","预约已到" };
	return strs[this->getStatu()];
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



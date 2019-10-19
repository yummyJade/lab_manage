#include "../../include/model/Order.h"
#include "../../include/util/Record.h"
#include "../../include/util/TableRecord.h"
#include <vector>
#include <iostream>
#include <string>


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

Order::Order(long long userId, long long bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime,
             Status statu) : userId(
        userId), bookId(bookId), borrowTime(borrowTime), returnTime(returnTime), statu(statu) {}


//1代表在借，2代表已还，3代表预约，4代表已续借的在借，5代表预约已到
std::vector<Order> Order::getAssignUserBorrowedHistory(int firstOrderId) {
    return std::vector<Order>();
}


std::vector<Order> Order::getAssignUserBorrowingList(int firstOrderId) {
    // 获取所有借阅记录
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
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
    record.setReTime(this->returnTime.toLLTime());
    return record;
}


Order Order::RecordCopyToOrder(Record record) {
    SimpleTime boTime = SimpleTime::llTimeToSimpleTime(record.getBoTime());
    SimpleTime reTime = SimpleTime::llTimeToSimpleTime(record.getReTime());
    return Order(record.getId(), record.getStId(), record.getBookId(), boTime, reTime,
                 static_cast<Status>(record.getState()));
}

Order::Order(long long int id, long long int userId, long long int bookId, const SimpleTime &borrowTime,
             const SimpleTime &returnTime, Status statu) : id(id), userId(userId), bookId(bookId),
                                                           borrowTime(borrowTime), returnTime(returnTime),
                                                           statu(statu) {}

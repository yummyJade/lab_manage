#include "../../include/model/Order.h"
#include <vector>
#include <iostream>
#include <string>

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

std::vector<Order> Order::getAssignUserBorrowedHistory(int jobNum) {
    return std::vector<Order>();
}

std::vector<Order> Order::getAssignUserBorrowingList(int jobNum) {
    return std::vector<Order>();
}

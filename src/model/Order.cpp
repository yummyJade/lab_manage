#include "../../include/model/Order.h"



Order::Order()
{
}


Order::~Order()
{
}

Order::Order(bool isRenew, int id, int bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime, Status statu)
        : IsRenew(isRenew), id(id), bookId(bookId), borrowTime(borrowTime), returnTime(returnTime), statu(statu) {}

vector<string> Order::serialize() {
    vector<string> info;
//    info.push_back(to_string(this->id));
    info.push_back(to_string(this->IsRenew));
    info.push_back(to_string(this->bookId));
    info.push_back(this->borrowTime.serialize());
    info.push_back(this->returnTime.serialize());

    //todo: 枚举的序列化和反序列化
//    info.push_back(this->statu);
    return info;
}

bool Order::deSerialize(vector<string> info) {
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
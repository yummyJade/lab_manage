#pragma once

#include "core/SimpleTime.h"


enum Status {
    Borrowing, Returned
};        //订单状态
class Order {
private:
    bool IsRenew = true;        // 是否可续借
    int id;        //订单编号
    int bookId;     // 借的书的id
    SimpleTime borrowTime;        // 借书时间
    SimpleTime returnTime;        // 还书时间
    Status statu;               // 订单状态

public:
    Order();

    Order(bool isRenew, int id, int bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime, Status statu);


    ~Order();

    // 计算欠款
    int calcDebt();

    // 序列化函数
    vector<string> serialize();

    // 反序列化函数
    bool deSerialize(vector<string>);
};


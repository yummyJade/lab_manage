#pragma once

#include "core/SimpleTime.h"


enum Status {
    BORROWING, RETURNED
};        //订单状态
class Order {
private:
    bool IsRenew = true;        // 是否可续借
    int id;        //订单编号
    int bookId;     // 借的书的id
    SimpleTime borrowTime;        // 借书时间
    SimpleTime returnTime;        // 还书时间
    Status statu;               // 订单状态

    /**
     * 将枚举类型的statu转化成对应的字符串
     * @param statu
     * @return
     */
    static std::string statuEnumToString(Status statu);

    /**
     * 将字符串转换为对应的枚举
     * @param str
     * @return
     */
    static Status stringEnumToStatu(std::string str);

public:
    Order();

    Order(bool isRenew, int id, int bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime, Status statu);


    ~Order();

    // 计算欠款
    int calcDebt();

    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);
};


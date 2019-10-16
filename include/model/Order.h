#pragma once

#include "core/SimpleTime.h"
#include <iostream>
#include <vector>

enum Status {
    BORROWING, RETURNED
};        //订单状态
class Order {
private:
    long long id;
    long long userId;        //借书的人的id
    long long bookId;     // 借的书的id
    SimpleTime borrowTime;        // 借书时间
    SimpleTime returnTime;        // 还书时间
    Status statu;               // 订单状态 1代表在借，2代表已还，3代表预约，4代表已续借的在借，5代表预约已到

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

    Order(long long userId, long long bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime, Status statu);

    ~Order();

    // 计算欠款
    int calcDebt();

    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);

public:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由 public 调用------------

    /**
     * 获取指定用户的历史借书记录
     * @param firstOrderId
     * @return
     */
    static std::vector<Order> getAssignUserBorrowedHistory(int firstOrderId);

    /**
     * 获取指定用户的正在借阅的记录
     * @param firstOrderId
     * @return
     */
    static std::vector<Order> getAssignUserBorrowingList(int firstOrderId);

    /**
     * 获取指定用户正在逾期的借书记录
     * @param firstOrderId
     * @return
     */
    static std::vector<Order> getAssignUserOweOrder(int firstOrderId);


    /**
     * 获得逾期未还的订单
     * @return
     */
    static std::vector<Order> getTimeOutOrders();

    /**
     * 检索field字段的值为value的内容
     * @param field
     * @param value
     * @return
     */
    static std::vector<Order> filter(std::string field, std::string value);
};


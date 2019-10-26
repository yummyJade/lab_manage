#pragma once

#include "core/SimpleTime.h"
#include <iostream>
#include <vector>
#include "../../linkDatebase/include/Record.h"

enum Status {
    BORROWING, RETURNED
};        //订单状态
class Order {
private:
    long long id;
    long long userId;        //借书的人的id
    int bookId;     // 借的书的id
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

    // 借书用的
    Order(long long userId, int bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime, Status statu);

    Order(long long int id, long long int userId, int bookId, const SimpleTime &borrowTime,
          const SimpleTime &returnTime, Status statu);

    ~Order();

    long long int getId() const;

    long long int getUserId() const;

    int getBookId() const;

    const SimpleTime &getBorrowTime() const;

    const SimpleTime &getReturnTime() const;

    Status getStatu() const;

	std::string getStatuStr();

    void setId(long long int id);

    void setUserId(long long int userId);

    void setBookId(int bookId);

    void setBorrowTime(const SimpleTime &borrowTime);

    void setReturnTime(const SimpleTime &returnTime);

    void setStatu(Status statu);

    // 计算欠款
    int calcDebt();

    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);

    // 序列化为Record
    Record toRecordCopy();

    // Record转化为Order
    static Order RecordCopyToOrder(Record record);

private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------
    // 添加一个实例到数据库,返回其id
    static int addOneOrder(Order order, int firstId = -1);

public:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由 public 调用------------
    /**
     * 获取指定书籍的历史借书记录
     * @param bookId
     * @return
     */
    static std::vector<Order> getAssignBookBorrowedHistory(int bookId);
    /**
     * 获取指定书籍的正在预约的记录
     *
     */
    static std::vector<Order> getAssignBookAppointingList(int bookId);
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
     * 获取指定用户的已预约未取的记录
     * @param firstOrderId
     * @return
     */
    static std::vector<Order> getAssignUserAppointmentList(int firstOrderId);
    /**
     * 获取指定用户的已到的预约（且没有超期
     * @param firstOrderId
     * @return
     */
     //todo:这个预约超期判断谁来做，此时要判断么
     static std::vector<Order> getAssignUserArrivedAppointmentList(int firstOrderId);
    /**
     * 判断指定用户的在预约记录是否有该本书（未实现）
     */
    static bool judgeAssignUserAppointmentList(int firstOrderId, int isbn);
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

    /**
     * 往数据库增一条记录
     * @param order
     * @return
     */
    static int addSingleOrder(int firstId, Order order);

    // 静态函数, 打印查询出来的结果集
    static void printOrderList(std::vector<Order> orders);

    // 获取用于打印列表的信息
    std::vector<std::string> getPrintLineStr();

    // 修改指定id的instance的状态和应还时间,用于借还书
    static bool updateStateAndReturnTimeById(Order order);

    //修改指定id的instance的状态和bookId,用于预约书
    static bool updateStateAndBookIdById(Order order);

    //修改指定id的instance的状态和借书时间还书时间，用于领取书
    static bool updateStateAndReturnTimeAndLendTimeById(Order order);
};


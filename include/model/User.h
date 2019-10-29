#pragma once
#include <vector>
#include "../model/Order.h"
enum status {
    Admin = 0, Teacher = 1, Graduate = 2, Undergraduate = 3, BanTeacher = -1, BanGraduate = -2, BanUndergraduate = -3
};        //特权思想枚举
const std::string STATUS[] = {
        "ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE", "BAN"
};

class User {
    static const int lendDays[];//= {30, 60, 90}; // 最多可同时借书的时长,单位天
    static const int lendNums[];// = {30, 60, 90}; // 最多可同时借书的本数
    static const int appointNums[];// = {3, 6, 9}; //最多可以同时预约的书本数量
private:
    long long jobNum;        //工号即id
    status type;
    std::string name;        //姓名
    std::string password;        //password
    int firstOrderId;  // 该学生借的第一本书的订单在Order表的id,-1表示未借

    /**
     * 将枚举类型的statu转化成对应的字符串
     * @param statu
     * @return
     */
    static std::string statuEnumToString(status statu);

public:
    User();
    ~User();

    // 默认用工号做密码
    User(long long int jobNum, status type, const std::string &name);

    User(long long jobNum, status type, const std::string &name, const std::string &password);

    User(long long int jobNum, status type, const std::string &name, const std::string &password,
         int firstOrderId);

    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);

    //打印用户信息
    void printUserInfo();

    bool canLendBook();


    void setJobNum(long long jobNum);

    void setJobNum1(long long int jobNum);

    void setType(status type);

    void setName(const std::string &name);

    void setFirstOrderId(long long int firstOrderId);

    long long readAndSetJobNum();

    int readAndSetType();

    std::string readAndSetName();

    std::string readAndSetPassword();

    static const int *getLendDays();

    static const int *getLendNums();

    static const int *getAppointNums();

    long long int getJobNum() const;

    status getType() const;

	std::string getTypeStr();

    const std::string &getName() const;

    const std::string &getPassword() const;

    int getFirstOrderId() const;

    // 计算该用户一次借阅能借多久
    int getCanLendDays();

    // 计算该用户同时能借多少本
    int getCanLendNums();

    //计算该用户同时能够预约多少本
    int getCanAppointNums();

    // 获取当前用户的登陆消息提示(预约已到和3天内到期的书籍)
    bool getUserMessage();


    /** 待完成
     * 判断该工号的用户是否存在,若存在则返回该用户
     * @param jobNum
     * @return
     */
    static bool checkUserExist(long long jobNum, User *user);

    /** 已完成
     * 修改当前登陆的用户的密码,并持久化到数据库
     * @param password
     * @return
     */
    bool changePwdService();

    static bool updateUsersAssignField(std::string assignField, std::string assignValue, std::string changeField,
                                       std::string changeValue);

/**
 * 将字符串转换为对应的枚举
 * @param str
 * @return
 */
    static status stringEnumToStatu(std::string str);

/**
 * 修改密码,并持久化到数据库
 * @param password
 * @return
 */
bool setPassword(const std::string &password);

private:

    /**
     * 判断密码是否正确
     * @param password
     * @return
     */
    bool isLegalPassword(const std::string &password);


    // 二维字符串数组转User对象数组
    static std::vector<User> stringsToUsers(std::vector<std::vector<std::string>>);


private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------




    //------------------------------------------------------
    //----下面这些是用户共有的操作(需要登陆后操作)---------------

    /** 未完成
     * 获取当前登陆用户的借阅历史记录,通过调用Order类的静态函数实现
     * @return
     */
    std::vector<Order> getBorrowedHistory();


    /** 未完成
     * 获取当前登陆用户正在借阅的书的记录,通过调用Order类的静态函数实现
     */
    std::vector<Order> getBorrowingList();

    /**
     * 判断是否允许该账号登陆,在账号密码验证成功后执行该函数
     * @return 0-允许登陆,1-账号被禁止,2-有欠费
     */
    int isAllowedLogin();


public:
    /**
     * 用户预约指定书种
     * @param jobNum
     * @param isbn
     * @return
     */
    bool appointmentAssignBook(int bookId, std::string isbn);


    //------------------------------------------------------------------------------
    //----下面这些是管理员的操作,且与图书相关-------------------------------------------
    //------------------------------------------------------------------------------



    // 待完成,打印某一种书的情况(由isbn?..指定),包括是否被借阅,啥时候还
    void printAssignBookInfo();

    //------------------------------------------------------------------------------
    //----下面这些是管理员用户的操作,且与User相关---------------------------------------
    //------------------------------------------------------------------------------

    // 待完成,重置用户密码
    bool resetUserPassword();

public:
    //------------------------------------------------------
    //----下面这些是静态操作---------------------------
    // 静态函数, 批量导入用户
    static bool addUsers(std::vector<std::vector<std::string>> queryData, std::vector<long long> &ids);

    // 批量导入用户，这个函数要搬出去
    static bool importUsers(std::string incomingPath);

    /**
     * 加密密码
     * @param pwd
     * @return
     */
    static std::string encryPassword(std::string pwd);

    // 获取用于打印列表的信息
    std::vector<std::string> getPrintLineStr();

    // 静态函数, 打印查询出来的结果集 todo: 完善打印效果
    static void printUserList(std::vector<User>);

    // 获取指定id的用户对象
    static User getUserByJobNum(long long jobNum);

    // 静态函数, 根据指定字段的值搜索内容,返回User对象数组
    static std::vector<User> searchUsersBySingleField(std::string field, std::string value);

    // 静态函数, 显示该表所有内容
    static std::vector<User> searchAll();


    /**
     * 用户借指定书实例
     * @param bookInstanceId
     * @return
     */
    int borrowAssignBookInstance(int bookInstanceId);


    /**
     * 用户归还, 指定订单
     * @param bookInstanceId
     * @return
     */
    static int returnAssignOrder(Order order);

    /**
     * 用户续借指定订单
     * @param order
     * @return
     */
    int renewAssignOrder(Order order);
    /**
    * 用户取回已到的预约
    *
    */
    bool getArrivedAppointment(Order order);


    /**
     * 用户登陆
     * @param name
     * @param password
     * @return
     */
    static int login(long long jobNum, std::string password, User *user);


    /**
     * 处理用户的逾期借阅
     * @return
     */
    int dealWithOverTimeOrder();

};

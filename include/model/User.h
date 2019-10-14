#pragma once

#include <vector>
#include "../model/Order.h"
enum status {
    Admin, Teacher, Graduate, Undergraduate, Ban
};        //特权思想枚举
const std::string STATUS[] = {
        "ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE", "BAN"
};

class User {
    static const int lendDays[3];//= {30, 60, 90}; // 最多可同时借书的时长,单位天
    static const int lendNums[3];// = {30, 60, 90}; // 最多可同时借书的本数
private:
    int jobNum;        //工号即id
    status type;
    std::string name;        //姓名
    std::string password;        //password
    long long firstOrderId;  // 该学生借的第一本书的订单在Order表的id,-1表示未借

    /**
     * 将枚举类型的statu转化成对应的字符串
     * @param statu
     * @return
     */
    static std::string statuEnumToString(status statu);

    /**
     * 将字符串转换为对应的枚举
     * @param str
     * @return
     */
    static status stringEnumToStatu(std::string str);

public:
    User();
    ~User();

    User(int jobNum, status type, const std::string &name, const std::string &password);

    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);

    //打印用户信息
    void printUserInfo();

    bool canLendBook();

    void setId(int value);

    int getId();

    void setJobNum(int jobNum);


private:
    /**
     * 修改密码
     * @param password
     * @return
     */
    bool setPassword(const std::string &password);

    /**
     * 判断密码是否正确
     * @param password
     * @return
     */
    bool isLegalPassword(const std::string &password);


private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------
    /** 待完成
     * 判断该工号的用户是否存在,若存在则返回该用户
     * @param jobNum
     * @return
     */
    static User checkUserExist(int jobNum);

    /** 待完成
     * 用户登陆
     * @param name
     * @param password
     * @return
     */
    static User login(std::string name, std::string password);




    //------------------------------------------------------
    //----下面这些是用户共有的操作(需要登陆后操作)---------------
    /** 已完成
     * 修改当前登陆的用户的密码
     * @param password
     * @return
     */
    bool changePwd(const std::string &password);

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

    /**
     * 用户借指定书
     * @param bookInstanceId
     * @return
     */
    bool borrowAssignBookInstance(long long bookInstanceId);
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

};

//const int User::lendDays[3]={30,60,90};
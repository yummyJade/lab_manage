#pragma once

#include <vector>
enum status {
    Admin,Teacher, Graduate, Undergraduate
};        //特权思想枚举
const std::string STATUS[4] = {
        "ADMIN","TEACHER","GRADUATE","UNDERGRADUATE"
};

class User {
    constexpr static const int lendDays[3] = {30, 60, 90}; // 最多可同时借书的时长,单位天
    constexpr static const int lendNums[3] = {30, 60, 90}; // 最多可同时借书的本数
private:
    int jobNum;        //工号即id
    status type;
    std::string name;        //姓名
    std::string password;        //password

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

    //------------------------------------------------------
    //----下面这些是静态操作---------------------------
    /**
     * 用户登陆-待完成
     * @param name
     * @param password
     * @return
     */
    User login(std::string name, std::string password);

    //------------------------------------------------------
    //----下面这些是用户共有的操作---------------------------



    /**
     * 打印当前用户借阅的书-待完成
     */
    void printBorrowBooks();

    //------------------------------------------------------
    //----下面这些是管理员用户的操作---------------------------

    // 待完成,书籍导入
    void addBooksOperate();

    // 待完成,打印逾期未还名单
    void printTimeOutOrders();

    // 待完成,打印某一种书的情况(由isbn?..指定),包括是否被借阅,啥时候还
    void printAssignBookInfo();
};

//const int User::lendDays[3]={30,60,90};
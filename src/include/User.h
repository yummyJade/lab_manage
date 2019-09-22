#pragma once

#include <string>

enum status {
    Teacher, Graduate, Undergraduate, Senior, Middle, Primary, Kindergarten
};        //特权思想枚举

class User {
    static const int[]
    lendDays = {30, 60, 90}; // 最多可同时借书的时长,单位天
    static const int[]
    lendNums = {30, 60, 90}; // 最多可同时借书的本数
private:
    int id;
    int jobNum;        //工号
    int borrowNum;  // 当前借阅的图书数量
//	int maxLend;		//可借书的数量  这个不要了,用上面的静态常量省空间
//	int maxTime;		//可借阅时间  这个不要了,用上面的静态常量省空间
    status type;
    string pwd;        //password


public:
    User();
    ~User();


    //打印用户信息
    void printUserInfo();

    bool canLendBook();

    void setId(int value);

    int getId();

    void setJobNum(int jobNum);
};


#ifndef MYC1_LIBRARY_H
#define MYC1_LIBRARY_H

#include "../model/User.h"

class Library {
private:
    static User *adminUser;
    static User *simpleUser;

    Library();

    ~Library();

public:
    // 程序启动时候运行,判断数据库是否存在
    static bool checkDBLegal();

    static User *login(bool needSimpleUser);

    // 登陆管理员账号
    static User *loginAdminUser();

    // 登陆普通用户账号
    static User *loginSimpleUser();

    // 单例,获取登陆的管理员账号
    static User *getAdminUserInstance();

    // 单例,获取登陆的普通用户账号
    static User *getSimpleUserInstance();

};


#endif //MYC1_LIBRARY_H

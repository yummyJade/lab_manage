#ifndef MYC1_LIBRARY_H
#define MYC1_LIBRARY_H

#include "../model/User.h"

class Library {
private:
    User adminUser;
    User simpleUser;

    Library();

    ~Library();

public:
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

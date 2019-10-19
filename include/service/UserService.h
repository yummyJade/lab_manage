#ifndef MYC1_USERSERVICE_H
#define MYC1_USERSERVICE_H

#include <iostream>
#include <vector>
#include "model/User.h"


/**
 * 用户借指定书?
 * @param jobNum
 * @param bookInstanceId
 * @return
 */
bool borrowAssignBookInstance(int jobNum, long long bookInstanceId);


bool appointmentAssignBook(int jobNum, int bookId);

// 增加单个用户
bool addSingleUserService();

// 初始化用户密码
bool resetAssignUserPassword();

// 冻结指定用户(禁止借书\预约)
bool freezeAssignUser();

// 解冻指定用户
bool unfreezeAssignUser();

#endif //MYC1_USERSERVICE_H

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


#endif //MYC1_USERSERVICE_H

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <iomanip>
#include<string>
#include<windows.h>
#include <vector>
#include "libm.h"
#include "../src/libcpp.h"
#include "../src/libcpp.h"
#include "../linkDatebase/datacpp.h"
#include "../yu/Verify.h"
#include "../yu/Verify.cpp"
#include "../src/core/Input.cpp"
using namespace std;


TEST_CASE("增加一个逾期未娶的预约记录", "[lendBook]") {
	// 增加一个逾期的
	User* user = Library::getSimpleUserInstance();
	BookInstance* instance = BookInstance::getInstanceById(1);
	// 插入一条借阅记录Order,需要(用户工号,书实例id,借书时间,预计归还时间,订单状态)
	Date lendDate = Date(2019, 7, 20);
	SimpleTime lendTime = SimpleTime(0, 0, 0, lendDate);
	Order order(user->getJobNum(), instance->getId(), lendTime,
		lendTime.addDay(30),
		static_cast<Status>(5)); // 状态是5

	int orderId = Order::addSingleOrder(user->getFirstOrderId(), order);

	// 设置该Bookinstance已被预约,并更新应还时间
	instance->setStatus(5);
	instance->setPlanReturnDate(lendDate.addDay(30));
	BookInstance::updateStateAndReturnTimeById(*instance);

	// 判断是否首次借阅,是的话更新借阅链表头的字段
	if (user->getFirstOrderId() == -1) {
		user->setFirstOrderId(orderId);
		User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "firstOrderId", to_string(orderId));
	}

}


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


TEST_CASE("����һ������δȢ��ԤԼ��¼", "[lendBook]") {
	// ����һ�����ڵ�
	User* user = Library::getSimpleUserInstance();
	BookInstance* instance = BookInstance::getInstanceById(1);
	// ����һ�����ļ�¼Order,��Ҫ(�û�����,��ʵ��id,����ʱ��,Ԥ�ƹ黹ʱ��,����״̬)
	Date lendDate = Date(2019, 7, 20);
	SimpleTime lendTime = SimpleTime(0, 0, 0, lendDate);
	Order order(user->getJobNum(), instance->getId(), lendTime,
		lendTime.addDay(30),
		static_cast<Status>(5)); // ״̬��5

	int orderId = Order::addSingleOrder(user->getFirstOrderId(), order);

	// ���ø�Bookinstance�ѱ�ԤԼ,������Ӧ��ʱ��
	instance->setStatus(5);
	instance->setPlanReturnDate(lendDate.addDay(30));
	BookInstance::updateStateAndReturnTimeById(*instance);

	// �ж��Ƿ��״ν���,�ǵĻ����½�������ͷ���ֶ�
	if (user->getFirstOrderId() == -1) {
		user->setFirstOrderId(orderId);
		User::updateUsersAssignField("jobNum", to_string(user->getJobNum()), "firstOrderId", to_string(orderId));
	}

}


#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../include/libm.h"

TEST_CASE("导入数据", "[AbsoluteUserTests]") {
	// 导入用户

}

TEST_CASE("IncorrectDate", "[AbsoluteDateTests]") {  // 12/0/2020 -> 0
    Date *date = new Date(2019, 9, 26);
//GregorianDate gregDate;
//gregDate.SetMonth(12);
//gregDate.SetDay(0);
//gregDate.SetYear(2020);
//REQUIRE(gregDate.getAbsoluteDate() == 0);
}


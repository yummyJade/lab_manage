#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../include/libm.h"

TEST_CASE("增加天数", "[AbsoluteDateTests]") {  // 12/2/2020 -> 737761
    Date *date = new Date(2019, 9, 26);
    date->addDay(4);
    CHECK(date->day == 30);

	Date date2 = Date(2019, 12, 31);
	date2.addDay(1);
	CHECK(date2.year == 2020);
	CHECK(date2.month == 1);
	CHECK(date2.day == 1);

}

TEST_CASE("比较天数", "[AbsoluteDateTests]") {  // 12/0/2020 -> 0
    Date *date = new Date(2019, 9, 26);
	Date* date2 = new Date(2020, 9, 26);
	CHECK(date->compare(*date2) == -366);

	date->addDay(366);
	CHECK(date->compare(*date2) == 0);
}





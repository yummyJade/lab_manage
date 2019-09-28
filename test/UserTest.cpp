#define CATCH_CONFIG_MAIN

#include "catch.hpp"
//#include "../include/libm.h"
#include "../include/core/Date.h"

TEST_CASE("ExampleDate", "[AbsoluteDateTests]") {  // 12/2/2020 -> 737761
    Date *date = new Date(2019, 9, 26);
    date->addDay(4);
    CHECK(date->day == 30);
//    REQUIRE(date->day==29);
}

TEST_CASE("IncorrectDate", "[AbsoluteDateTests]") {  // 12/0/2020 -> 0
    Date *date = new Date(2019, 9, 26);
//GregorianDate gregDate;
//gregDate.SetMonth(12);
//gregDate.SetDay(0);
//gregDate.SetYear(2020);
//REQUIRE(gregDate.getAbsoluteDate() == 0);
}


#include <iostream>
#include "../include/core/Date.h"
#include "../src/core/Date.cpp"
#include "../include/core/Time.h"
#include "../src/core/Time.cpp"
#include "../include/model/Book.h"
#include "../src/model/Book.cpp"

int main() {
    Date *date=new Date(2019,9,22);
    date->print();
    date->addDay(100);
    date->print();
    return 0;
}
#include <iostream>
#include "Date.h"
#include "Date.cpp"
#include "SimpleTime.h"
#include "SimpleTime.cpp"
#include "Book.h"
#include "Book.cpp"

int main() {
    Date *date=new Date(2019,9,22);
    date->print();
    date->addDay(100);
    date->print();
    delete date;
    return 0;
}
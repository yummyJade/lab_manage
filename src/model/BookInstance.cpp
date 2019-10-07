#include "../../include/model/BookInstance.h"
#include <iostream>

using namespace std;


BookInstance::BookInstance(const string &isbn, int status, const string &position) : isbn(isbn), status(status),
                                                                                     position(position) {}

BookInstance::BookInstance(const string &isbn, int status, const string &position, const Date &planReturnDate) : isbn(
        isbn), status(status), position(position), planReturnDate(planReturnDate) {}

BookInstance::BookInstance(const string &isbn, const string &position) : isbn(isbn), position(position) {}


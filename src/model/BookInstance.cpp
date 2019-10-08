#include "../../include/model/BookInstance.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


BookInstance::BookInstance(const string &isbn, int status, const string &position) : isbn(isbn), status(status),
                                                                                     position(position) {}

BookInstance::BookInstance(const string &isbn, int status, const string &position, const Date &planReturnDate) : isbn(
        isbn), status(status), position(position), planReturnDate(planReturnDate) {}

BookInstance::BookInstance(const string &isbn, const string &position) : isbn(isbn), position(position) {}

int BookInstance::addBookInstance(BookInstance instance, int firstId) {
    // todo: 连接数据库
    return 66;
}

int BookInstance::importBookInstances(std::vector<BookInstance> instances, int firstId = -1) {
    int index = 0;
    if (firstId == -1) {//如果是首次插入,先插一本
        firstId = addBookInstance(instances[index++], firstId);
    }

    while (index < instances.size()) {// 将尚未导入的实例全部导入
        addBookInstance(instances[index++], firstId);
    }
    return firstId;
}




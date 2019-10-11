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
    // todo: �������ݿ�
    return 66;
}

int BookInstance::importBookInstances(std::vector<BookInstance> instances, int firstId = -1) {
    int index = 0;
    if (firstId == -1) {//������״β���,�Ȳ�һ��
        firstId = addBookInstance(instances[index++], firstId);
    }

    while (index < instances.size()) {// ����δ�����ʵ��ȫ������
        addBookInstance(instances[index++], firstId);
    }
    return firstId;
}




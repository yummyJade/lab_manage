#ifndef MYC1_BOOKSERVICE_H
#define MYC1_BOOKSERVICE_H

#include <iostream>
#include <vector>
#include "model/Book.h"
#include "model/BookInstance.h"


/**
 * 打印指定ISBN的书籍的详细信息
 * @param isbn
 * @return
 */
bool printBookDetailInfo(std::string isbn, vector<BookInstance>* saveInstances);


// 改变指定种类书籍信息
bool changeAssignBookInfo();

// 改变指定书实例信息
bool changeAssignBookInstanceInfo();


bool changeAssignBookInstanceState(long long id);


// 下架指定isbn的所有图书
bool deleteAssignIsbnBook(std::string isbn);

#endif //MYC1_BOOKSERVICE_H

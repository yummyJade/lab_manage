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
bool printBookDetailInfo(std::string isbn);


#endif //MYC1_BOOKSERVICE_H

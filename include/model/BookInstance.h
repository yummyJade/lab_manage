//
// Created by THINKPAD on 2019/9/28.
//

#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

class BookInstance {
private:
    long long id;
    long long bookId;     //图书所属种类的id
    int status;     //状态
    std::string position;       // 图书位置
    Date planReturnDate; // 预计归还日期

public:
    BookInstance(long long int bookId, const std::string &position) : bookId(bookId), position(position) {}

    BookInstance(long long bookId, int status, const std::string &position, const Date &planReturnDate) : bookId(
            bookId),
                                                                                                          status(status),
                                                                                                          position(
                                                                                                                  position),
                                                                                                          planReturnDate(
                                                                                                                  planReturnDate) {}
};

#endif //MYC1_BOOKINSTANCE_H


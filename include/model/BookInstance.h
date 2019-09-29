//
// Created by THINKPAD on 2019/9/28.
//

#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

class BookInstance : {
private:
    int bookId;     //图书的id
    int status;     //状态
    std::string position;       // 图书位置
    Date::Date planReturnDate; // 预计归还日期
};

#endif //MYC1_BOOKINSTANCE_H


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
    //    bool isLend = true;        //是否可借
//    bool isValid = true;       // 是否有效, 有被删除时置为false

};

#endif //MYC1_BOOKINSTANCE_H


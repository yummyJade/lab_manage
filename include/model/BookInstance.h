//
// Created by THINKPAD on 2019/9/28.
//

#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

#include <iostream>
#include "../core/Date.h"
class BookInstance {
private:
    long long id;
    std::string isbn;     //ͼ�����������isbn
    int status;     //״̬
    std::string position;       // ͼ��λ��
    Date planReturnDate; // Ԥ�ƹ黹����

public:

    BookInstance(const std::string &isbn, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position, const Date &planReturnDate);

private:
    //------------------------------------------------------
    //----������Щ�������ݿ⽻���Ľӿ�,��private����------------


};

#endif //MYC1_BOOKINSTANCE_H


#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

#include <iostream>
#include <vector>
#include "../core/Date.h"
class BookInstance {
public:
    long long id;
    std::string isbn;     //图书所属种类的isbn
    int status;     //状态
    std::string position;       // 图书位置
    Date planReturnDate; // 预计归还日期

public:

    BookInstance(const std::string &isbn, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position, const Date &planReturnDate);


private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------
    // 添加一个实例到数据库,返回其id
    static int addBookInstance(BookInstance instance, int firstId = -1);

public:
    /**
     * 批量导入同一本书的实例,返回其首个id
     * @param instances
     * @return
     */
    static int importBookInstances(std::vector<BookInstance> instances, int firstId);


//    /**
//     * 检索field字段的值为value的内容
//     * @param field
//     * @param value
//     * @return
//     */
//    static std::vector<BookInstance> filter(std::string field,std::string value);


    /**
     * 检索id字段值为参数id的内容
     * @param id
     * @return
     */
    static BookInstance getInstanceById(int id);
};

#endif //MYC1_BOOKINSTANCE_H


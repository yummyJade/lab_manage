#pragma once

#include "core/SimpleTime.h"
#include <iostream>
#include <vector>
class Book {

private:
    char type;        //书所属类别A--V，期刊为0
    int count;      //计数
    int price;        //书本价格,单位分
    int id;
    long long firstInstanceId;  // 第一本图书实例在Instance表的id
    std::string name;        //书名
    std::string author;        //作者
    std::string isbn;        //ISBN
    std::string press;       // 出版社

private:
    //打印书本类型
    std::string printType();

    std::string printIsLend();


public:
    Book();

    Book(char type, int count, int price, long long int firstInstanceId, const std::string &name,
         const std::string &author, const std::string &isbn, const std::string &press);

    Book(char type, int count, int price, const std::string &name, const std::string &author, const std::string &isbn,
         const std::string &press);

    ~Book();

    // 保存当前对象到数据库, 暂时写一下看看好不好使
    void save();

    //打印书本基本信息
    void printBookInfo();


    // ----------------------------------------------------------------
    // ------下面这些是静态函数------------------------------------------

    static bool importBooks();

    // 静态函数, 删除书籍
    static bool deleteBooksByBookIds(std::vector<std::string> bookIds);

    // 静态函数, 根据指定字段的值搜索内容
    static std::vector<std::vector<std::string>> searchBooksBySingleField(std::string field, std::string value);

    // 静态函数, 修改
    static bool
    updateBooks(std::string assignField, std::string assignValue, std::string changeField, std::string changeValue);

    // 静态函数, 显示该表所有内容
    static std::vector<std::vector<std::string>> searchAll();


    // 静态函数, 打印查询出来的结果集 todo: 完善打印效果
    static void printBookList(std::vector<std::vector<std::string>> queryData);

    // 静态函数, 借书 todo:原子操作,借书一瞬间炸了
    static void borrowAssignIdBook(long long bookId, long long userId);

    // 静态函数, 借书 todo:原子操作,借书一瞬间炸了
    static void returnAssignOrderId(long long orderId);


    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);

private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------
    /**
     * 判断给定的isbn号的书籍是否存在,若存在,则返回首本id,否则返回-1
     * @param isbn
     * @return
     */
    static int checkAssignISBNExist(std::string isbn);

    // 静态函数, 增加书籍,判断给定的isbn数组中,哪些是已经存在的,存在返回首本id,不存在返回-1
    static std::vector<int> checkISBNsExist(std::vector<std::string> isbns);

    // 静态函数, 增加书籍
    static bool addBooks(std::vector<std::vector<std::string>> queryData, std::vector<long long> &ids);

    // 静态函数, 将类中所有符合isbn条件的对象的馆藏量增加对应的addCount本
    static bool updateBooksCount(std::vector<std::string> isbns, std::vector<int> addCount);
};


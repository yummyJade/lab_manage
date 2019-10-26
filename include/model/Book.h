#pragma once
#include "core/SimpleTime.h"
#include <iostream>
#include <vector>
class Book {
private:
    char type;        //书所属类别A--V，期刊为0
    int count;      //计数
    int appointmentNum; // 预约人数
    int price;        //书本价格,单位分
    int id;
    int firstInstanceId;  // 第一本图书实例在Instance表的id
    std::string name;        //书名
    std::string author;        //作者
    std::string isbn;        //ISBN
    std::string press;       // 出版社

private:
    //打印书本类型
    std::string getTypeContent();

    std::string printIsLend();


public:
    Book();

	Book::Book(int id, char type, int count, int appointmentNum, int price, int firstInstanceId, const string& name,
		const string& author, const string& isbn, const string& press);

    Book(char type, int count, int price, int firstInstanceId, const std::string &name, const std::string &author,
         const std::string &isbn, const std::string &press);

    Book(char type, int count, int appointmentNum, int price, int firstInstanceId, const std::string &name,
         const std::string &author, const std::string &isbn, const std::string &press);

    Book(char type, int count, int price, const std::string &name, const std::string &author, const std::string &isbn,
         const std::string &press);


    ~Book();

    int getAppointmentNum() const;


    char getType() const;

    int getCount() const;

    int getPrice() const;

    int getId() const;

    const std::string &getName() const;

    const std::string &getAuthor() const;

    const std::string &getIsbn() const;

    const std::string &getPress() const;


    int getFirstInstanceId() const;

    void setType(char type);

    void setCount(int count);

    void setPrice(int price);

    void setId(int id);

    void setFirstInstanceId(long long int firstInstanceId);

    void setName(const std::string &name);

    void setAuthor(const std::string &author);

    void setIsbn(const std::string &isbn);

    void setPress(const std::string &press);

    void setAppointmentNum(int appointmentNum);


    //打印书本基本信息
    void printBookInfo();

    // 获取用于表格答案的字符数组;
    std::vector<std::string> getPrintLineStr();

    /**
     * 修改当前书的可修改信息(书名,出版社,作者,类型,价格)
     * @return
     */
    bool updateBookModifiableInfo();

    // ----------------------------------------------------------------
    // ------下面这些是静态函数------------------------------------------
    // 批量导入书籍，这个函数要搬出去
    static bool importBooksService();

    // 导出所有书籍，这个函数要搬出去
    static bool exportBooks(std::vector<Book> books);

    // 添加一本书籍
    static bool addOneBookService();

    // 批量删除书籍
    static bool batchDeleteAssignIsbnsBooks(std::vector<std::string> isbns);

    // 静态函数, 删除书籍
    static bool deleteBooksByBookIds(std::vector<std::string> bookIds);

    // 静态函数, 根据指定字段的值搜索内容,返回Book对象数组
    static std::vector<Book> searchBooksBySingleField(std::string field, std::string value);

    // 静态函数, 修改
    static bool
    updateBooks(std::string assignField, std::string assignValue, std::string changeField, std::string changeValue);


    // 静态函数, 显示该表所有内容
    static std::vector<Book> searchAll();


    // 静态函数, 打印查询出来的结果集
    static void printBookList(std::vector<Book>);

    // 静态函数, 借书 todo:原子操作,借书一瞬间炸了
    static void borrowAssignIdBook(long long bookId, long long userId);

    // 静态函数, 借书 todo:原子操作,借书一瞬间炸了
    static void returnAssignOrderId(long long orderId);


    // 序列化函数
    std::vector<std::string> serialize();

    // 反序列化函数
    bool deSerialize(std::vector<std::string>);

/**
 * 判断给定的isbn号的书籍是否存在,若存在,则返回首本id,否则返回-1
 * @param isbn
 * @return
 */
    static int checkAssignISBNExist(std::string isbn);


// 静态函数, 将类中所有符合isbn条件的对象的馆藏量增加对应的addCount本
    static bool updateBooksCount(std::vector<std::string> isbns, std::vector<int> addCount);

//静态函数，将对应isbn的书籍的对象的预约数量增加或减少 1.增加 -1.减少
    static bool updateBooksAppointmentNum(std::string isbn, int addAppointmentNum);

private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------

    // 静态函数, 增加书籍,判断给定的isbn数组中,哪些是已经存在的,存在返回首本id,不存在返回-1
    static std::vector<int> checkISBNsExist(std::vector<std::string> isbns);

    // 静态函数, 增加书籍
    static bool addBooks(std::vector<std::vector<std::string>> queryData, std::vector<long long> &ids);

    // 二维字符串数组转Book对象数组
    static std::vector<Book> stringsToBooks(std::vector<std::vector<std::string>>);
};


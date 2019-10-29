#ifndef MYC1_BOOKINSTANCE_H
#define MYC1_BOOKINSTANCE_H

#include <iostream>
#include <vector>
#include "../../linkDatebase/include/Bookcopy.h"
#include "../core/Date.h"
class BookInstance {
    static const std::string BOOK_INSTANCE_FIELDS[];
public:
    int id;
    std::string isbn;     //图书所属种类的isbn
    int status;     //状态 1 可借 2 已被借阅 3 已删除(丢失或下架) 4 已被借阅且已被预约 5 已被预约
    std::string position;       // 图书位置
    Date planReturnDate; // 预计归还日期

public:

    BookInstance(const std::string &isbn, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position);

    BookInstance(const std::string &isbn, int status, const std::string &position, const Date &planReturnDate);

    BookInstance(int id, const string &isbn, int status, const string &position, const Date &planReturnDate);

    int getId() const;

    const std::string &getIsbn() const;

    int getStatus() const;

    const std::string &getPosition() const;

    const Date &getPlanReturnDate() const;

    int readAndSetStatus();

    std::string readAndSetPosition();

    std::string readAndSetIsbn();


    void setIsbn(const std::string &isbn);

    void setStatus(int status);

    void setPosition(const std::string &position);


    void setPlanReturnDate(const Date &planReturnDate);

    // 改变state 并持久化到数据库
    bool changeStateAndPersistence(int newState);

    // 修改可被修改的实例信息(状态,位置)
    bool updateBookInstanceModifiableInfo();

    // 序列化为Record
    Bookcopy toBookCopy();

    // Record转化为Order
    static BookInstance BookCopyToBookInstance(Bookcopy bookcopy);

private:
    //------------------------------------------------------
    //----下面这些是与数据库交互的接口,由private调用------------
    // 添加一个实例到数据库,返回其id
    static int addBookInstance(BookInstance instance, int firstId = -1);

public:
    // 打印一行信息
    bool printLine();

    // 获取用于打印结果集列表的字符串
    std::vector<std::string> getPrintLineStr();

    // 获取图书状态
    std::string getStatusStr();
public:
    /**
     * 批量导入同一本书的实例,返回其首个id
     * @param instances
     * @return
     */
    static int importBookInstances(std::vector<BookInstance> instances, int firstId);


    // 手动添加一本书的实例
    static bool addOneBookInstancesService();

    // 检测指定id的实例是否存在,待完善
    static bool checkAssignBookInstanceIdExist(int id);


    // 检测指定isbn的实例能否预约（至少有一本在借状态或已被预约的书
    static bool checkAssignBookCanAppointmentInstanceExist(std::string isbn);


    // 静态函数, 打印查询出来的结果集
    static void printBookInstanceList(std::vector<BookInstance> instances);




    /**
     * 检索id字段值为参数id的内容
     * @param id
     * @return
     */
    static BookInstance *getInstanceById(int id);

    /**
     * 获取某本书的所有实例
     * @param firstId
     * @return
     */
    static std::vector<BookInstance> getInstancesByFirstId(int firstId);

    // 删除指定
    static bool deleteInstancesByAssignInstanceId(int id);

    // 修改指定id的instance的状态和应还时间,用于借还书
    static bool updateStateAndReturnTimeById(BookInstance book);


};

#endif //MYC1_BOOKINSTANCE_H


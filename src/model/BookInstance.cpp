#include "../../include/model/BookInstance.h"
#include <iostream>
#include <vector>
#include <string>
#include <model/Book.h>

using namespace std;


BookInstance::BookInstance(const string &isbn, int status, const string &position) : isbn(isbn), status(status),
                                                                                     position(position) {}

BookInstance::BookInstance(const string &isbn, int status, const string &position, const Date &planReturnDate) : isbn(
        isbn), status(status), position(position), planReturnDate(planReturnDate) {}

BookInstance::BookInstance(const string &isbn, const string &position) : isbn(isbn), position(position) {}

int BookInstance::addBookInstance(BookInstance instance, int firstId) {
    // todo: 连接数据库
    return 66;
}

int BookInstance::importBookInstances(std::vector<BookInstance> instances, int firstId = -1) {
    int index = 0;
    if (firstId == -1) {//如果是首次插入,先插一本
        firstId = addBookInstance(instances[index++], firstId);
    }

    while (index < instances.size()) {// 将尚未导入的实例全部导入
        addBookInstance(instances[index++], firstId);
    }
    return firstId;
}

BookInstance *BookInstance::getInstanceById(int id) {
    BookInstance *bookInstance = new BookInstance("ISBN", "position");
    return bookInstance;
}

std::vector<BookInstance> BookInstance::getInstancesByFirstId(int firstId) {
    vector<BookInstance> result;
    Date date(2019, 11, 10);
    BookInstance instance("const std::string &isbn", 1, "谢哲勇的床头柜", date);
    BookInstance instance2("const std::string &isbn", 2, "谢哲勇的书包", date);
    BookInstance instance3("const std::string &isbn", 3, "信北B505", date);
    result.push_back(instance);
    result.push_back(instance2);
    result.push_back(instance3);

    return result;
}

bool BookInstance::addOneBookInstancesService() {
    printf("请依次输入 isbn,位置,图书状态,有效数量,用空格隔开\n");
    vector<string> fields;
    int index = 0;
    string temp_info;
    while (index < 4) {
        cin >> temp_info;
        fields.push_back(temp_info);
        index++;
    }


    string isbn = fields[0];
    string position = fields[1];
    int state = stoi(fields[2]);
    int count = stoi(fields[3]);


    int firstId = Book::checkAssignISBNExist(isbn);// 判断图书是否存在
    if (firstId == -1) {// 如果该书不存在
        cout << "图书馆尚无ISBN为" << isbn << "的书籍,请先添加该书后再执行添加实例操作" << endl;
        return false;
    } else {

        vector<BookInstance> bookinstancesFirstAdd;
        for (int i = 0; i < count; ++i) {
            BookInstance bookInstance(isbn, state, position);
            bookinstancesFirstAdd.push_back(bookInstance);
        }
        long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                      firstId);//获取链表的第一个位置

        vector<int> counts;
        counts.push_back(count);
        vector<string> isbns;
        isbns.push_back(isbn);
        Book::updateBooksCount(isbns, counts);
    }
    return true;

}

bool BookInstance::deleteOneBookInstancesService() {
    printf("请依次输入要删除的图书条形码\n");
    int bookInstanceId;
    cin >> bookInstanceId;

    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);
    if (instance == NULL) {//如果图书不存在
        cout << "删除失败,系统内没有条形码为" << bookInstanceId << "的书籍" << endl;
        return false;
    }
    BookInstance::deleteInstancesByAssignInstanceId(bookInstanceId);
    return true;
}

bool BookInstance::deleteInstancesByAssignInstanceId(int id) {
    return false;
}




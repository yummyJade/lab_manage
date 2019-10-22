#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"

bool printBookDetailInfo(std::string isbn) {
    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    if (books.size() <= 0) {
        cout << "该图书不存在" << endl;
        return false;
    }
    Book book = books[0];

    book.printBookInfo();

    vector<string> navs = {"编号", "图书位置", "状态\\预计归还时间"};
    TableRenderer render(navs, 8);

    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());

    BookInstance::printBookInstanceList(instances);

    return true;
}


bool changeAssignBookInfo() {
    string isbn;
    while (true) {
        printf("请输入要修改的书籍的ISBN:");
        cin >> isbn;
        if (Book::checkAssignISBNExist(isbn) == -1) { // 图书不存在
            printf("isbn为%s的图书不存在!", isbn.c_str());
            return false;
        } else {
            break;
        }
    }


    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    Book::printBookList(books);

    Book book = books[0];
//    book.printBookInfo();
    printf("请输入修改后的书名、出版社、作者、类型、价格, 以空格隔开,");
    string name, press, author;
    char type;
    double price;
    cin >> name >> press >> author >> type >> price;
    // todo:判断输入的信息是否合法
    book.setName(name);
    book.setPress(press);
    book.setAuthor(author);
    book.setType(type);
    book.setPrice((int) (price * 100) / 1);

    book.updateBookModifiableInfo();
    return true;
}


bool changeAssignBookInstanceInfo() {
    long long id;
    while (true) {
        printf("请输入要修改的书籍的条码号:");
        cin >> id;
        if (!BookInstance::checkAssignBookInstanceIdExist(id)) { // 图书不存在
            printf("条码号为%lld的图书不存在!", id);
            return false;
        } else {
            break;
        }
    }

    BookInstance *book = BookInstance::getInstanceById(id);
    printf("检索到如下信息\n");
    book->printLine();

    printf("请输入修改后的isbn,馆藏位置,状态, 以空格隔开,");
    string isbn, positon;
    char state;
    cin >> isbn >> positon >> state;
    book->setIsbn(isbn);
    book->setPosition(positon);
    book->setStatus(state);
    book->updateBookInstanceModifiableInfo();
    return true;
}



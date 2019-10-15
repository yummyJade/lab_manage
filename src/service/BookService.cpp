#include "service/BookService.h"
#include <iostream>
#include <string>
#include "util/TableRenderer.h"

bool printBookDetailInfo(std::string isbn) {
    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    if (books.size() <= 0) {
        return false;
    }
    Book book = books[0];

    book.printBookInfo();

    vector<string> navs = {"编号", "图书位置", "状态\\预计归还时间"};
    TableRenderer render(navs, 8);

    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());
    int index = 1;
    for (int i = 0; i < instances.size(); ++i) {
        vector<string> line;
        if (instances[i].status == 1) {// 可借
            line = {to_string(index++), instances[i].position, "可借"};
        } else {
            line = {to_string(index++), instances[i].position, instances[i].planReturnDate.serialize()};
        }

        render.addColume(line);
    }

    render.render();
    return true;
}
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

    printf("书名:%s\n作者:%s\n出版社:%s\n类别:%s\nISBN:%s\n价格:%s\n");


}
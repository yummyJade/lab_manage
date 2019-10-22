#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"

bool printBookDetailInfo(std::string isbn) {
    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    if (books.size() <= 0) {
        cout << "��ͼ�鲻����" << endl;
        return false;
    }
    Book book = books[0];

    book.printBookInfo();

    vector<string> navs = {"���", "ͼ��λ��", "״̬\\Ԥ�ƹ黹ʱ��"};
    TableRenderer render(navs, 8);

    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());

    BookInstance::printBookInstanceList(instances);

    return true;
}


bool changeAssignBookInfo() {
    string isbn;
    while (true) {
        printf("������Ҫ�޸ĵ��鼮��ISBN:");
        cin >> isbn;
        if (Book::checkAssignISBNExist(isbn) == -1) { // ͼ�鲻����
            printf("isbnΪ%s��ͼ�鲻����!", isbn.c_str());
            return false;
        } else {
            break;
        }
    }


    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    Book::printBookList(books);

    Book book = books[0];
//    book.printBookInfo();
    printf("�������޸ĺ�������������硢���ߡ����͡��۸�, �Կո����,");
    string name, press, author;
    char type;
    double price;
    cin >> name >> press >> author >> type >> price;
    // todo:�ж��������Ϣ�Ƿ�Ϸ�
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
        printf("������Ҫ�޸ĵ��鼮�������:");
        cin >> id;
        if (!BookInstance::checkAssignBookInstanceIdExist(id)) { // ͼ�鲻����
            printf("�����Ϊ%lld��ͼ�鲻����!", id);
            return false;
        } else {
            break;
        }
    }

    BookInstance *book = BookInstance::getInstanceById(id);
    printf("������������Ϣ\n");
    book->printLine();

    printf("�������޸ĺ��isbn,�ݲ�λ��,״̬, �Կո����,");
    string isbn, positon;
    char state;
    cin >> isbn >> positon >> state;
    book->setIsbn(isbn);
    book->setPosition(positon);
    book->setStatus(state);
    book->updateBookInstanceModifiableInfo();
    return true;
}



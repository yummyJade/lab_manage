#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"

bool printBookDetailInfo(std::string isbn, vector<BookInstance>* saveInstances=NULL) {
	system("cls");
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
	if (saveInstances != NULL) {// 把结果集送出去
		*saveInstances = instances;
	}
    BookInstance::printBookInstanceList(instances);

    return true;
}


bool changeAssignBookInfo() {
    string isbn;
    Book tempBook;
    while (true) {

        printf("请输入要修改的书籍的ISBN(输入0返回):");
        isbn=tempBook.readAndSetIsbn();
		if (isbn == "0") return false;
        if (Book::checkAssignISBNExist(isbn) == -1) { // 图书不存在
            printf("isbn为%s的图书不存在!\n", isbn.c_str());
        } else {
            break;
        }
    }


    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    Book::printBookList(books);

    Book book = books[0];
    printf("请输入修改后的信息\n");

    book.readAndSetName();
    book.readAndSetPress();
    book.readAndSetAuthor();
    book.readAndSetType();
    book.readAndSetPrice();

    book.updateBookModifiableInfo();
    return true;
}


bool changeAssignBookInstanceInfo() {
    int id;

    while (true) {
        printf("请输入要修改的书籍的条码号(输入0返回):");
        id=Input::getInt();
		if (id == 0) return false;
        if (!BookInstance::checkAssignBookInstanceIdExist(id)) { // 图书不存在
            printf("条码号为%d的图书不存在!\n", id);
        } else {
            break;
        }
    }

    BookInstance *book = BookInstance::getInstanceById(id);
    printf("检索到如下信息\n");
    book->printLine();

    printf("请输入修改后的信息\n");

    book->readAndSetIsbn();
    book->readAndSetPosition();
    book->readAndSetStatus();
    book->updateBookInstanceModifiableInfo();
    return true;
}


bool deleteAssignIsbnBook(std::string isbn="") {
	// 输入isbn号
	if (isbn == "") {// 没有传isbn参数进来
		while (true) {
			printf("请输入要修改的书籍的ISBN(输入0返回):");
			isbn=Input::getAssignMaxLengthStr(20);
			if (isbn == "0") return false;
			if (Book::checkAssignISBNExist(isbn) == -1) { // 图书不存在
				printf("isbn为%s的图书不存在!\n", isbn.c_str());
			}
			else {
				break;
			}
		}
	}

	//检索并打印该isbn的图书信息 以及 所有图书实例的信息
	vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
	Book::printBookList(books); // 打印所有图书实例信息

	vector<BookInstance> instances = BookInstance::getInstancesByFirstId(books[0].getFirstInstanceId());
	BookInstance::printBookInstanceList(instances);// 打印所有图书实例信息

	//由用户选择是否要全部下架
	cout << "输入Y下架ISBN为" << isbn << "的所有图书,输入N取消:";
	char operate;
	operate=Input::getChar();
	if (operate == 'Y' || operate == 'y') {// 下架所有图书
		// 修改Book馆藏量为0
		Book::updateBooks("isbn",isbn,"count","0");

		// 修改instance的状态
		for (int i = 0; i < instances.size(); i++) {
			instances[i].setStatus(3);// 3为已下架
			BookInstance::updateStateAndReturnTimeById(instances[i]);
		}
		cout<<"已成功下架ISBN为" << isbn << "的所有图书"<<endl;
		return true;
	}
	cout << "已取消操作" << endl;
	return false;
}




#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"

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
    int id;
    while (true) {
        printf("请输入要修改的书籍的条码号:");
        cin >> id;
        if (!BookInstance::checkAssignBookInstanceIdExist(id)) { // 图书不存在
            printf("条码号为%d的图书不存在!", id);
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


bool deleteAssignIsbnBook(std::string isbn="") {
	// 输入isbn号
	if (isbn == "") {// 没有传isbn参数进来
		while (true) {
			printf("请输入要修改的书籍的ISBN(输入0返回):");
			cin >> isbn;
			if (isbn == "0") return false;
			if (Book::checkAssignISBNExist(isbn) == -1) { // 图书不存在
				printf("isbn为%s的图书不存在!", isbn.c_str());
				return false;
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
	cin >> operate;
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




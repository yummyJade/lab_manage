#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"

bool dealWithOverTimeAppoint(std::string isbn, vector<BookInstance>* saveInstances=NULL){

    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    if (books.size() <= 0) {
        cout << "该图书不存在" << endl;
        return false;
    }
    Book book = books[0];
    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());
    if (saveInstances != NULL) {
        *saveInstances = instances;
    }


    //先查一遍状态，如果有状态为5即该实例被预约了，再继续查下去，否则不管
    int appointmentIndex = 5;      //表示的是预约已到未取
    for(int i = 0; i < instances.size(); i++) {
        if(instances[i].getStatus() == appointmentIndex) {
            vector<Order> oweOrders = Order::getAssignBookOweAppointing(instances[i].getId());
            int i = 0;
            if(oweOrders.size() == 1){
                oweOrders[i].setStatu(static_cast<Status>(6));     //将订单修改成预约超期的状态
                oweOrders[i].updateStateAndReturnTimeById(oweOrders[i]);      //持久化
                BookInstance *instance = BookInstance::getInstanceById(oweOrders[i].getBookId());
                //修改书籍实例的状态为可借
                //todo:先判断有没有人预约，如果有人预约，将这本书给下一个预约的人，否则改为可借
                if(book.getAppointmentNum() == 0){

                    instance->setStatus(1);
                    //更新函数爱改不改
                    BookInstance::updateStateAndReturnTimeById(*instance);
                } else if(book.getAppointmentNum() > 0) {
                    //处理预约操作,找到那个预约单子,给他改一下
                    //找到所有满足bookid==isbn && status = 3的单子
                    vector<Order> orders = Order::getAssignBookAppointingList(book.getId());
                    int earliestIndex = 0;       //借阅时间最早的下标
                    SimpleTime earliestDate = orders[0].getBorrowTime();         //记录时间最早那一天，这里的初值付给order里面第一个也行，
                    for (int i = 1; i < orders.size(); ++i) {
                        //接下来比较时间前后
                        if(earliestDate.compare(orders[i].getBorrowTime()) > 0) {    //大于0说明当前时间比compare里的要大，也就是要靠后
                            earliestIndex = i;
                            earliestDate = orders[i].getBorrowTime();
                        }
                    }
                    //找到最大的以后更改对应订单的信息，将bookId修改为当前instanceId，将status改为5，将借阅时间保存为当前时间，用作判断预约过期
                    orders[earliestIndex].setStatu(static_cast<Status>(5));
                    //存的实例id就是这个么？
                    orders[earliestIndex].setBookId(instance->getId());
                    //新操作：对borrowTime字段进行更新，如果想保留预约时间的话，也可以用归还时间做个记录
//        orders[earliestIndex].setReturnTime(SimpleTime::nowTime().addDay(this->getCanLendDays()));
                    User earliestUser = User::getUserByJobNum(orders[earliestIndex].getUserId());       //获取该记录的用户
                    orders[earliestIndex].setBorrowTime(SimpleTime::nowTime().addDay(earliestUser.getCanAppointDays()));
                    Order::updateStateAndBookIdAndBorrowTimeById(orders[earliestIndex]);
//        cout << "order的bookInsId为" << instance->getId() << endl;
                    //修改BookInstance的状态status为5,这里只修改了一项函数会不会报错
                    instance->setStatus(5);
                    //todo:记得新增一个修改单项status的函数
                    BookInstance::updateStateAndReturnTimeById(*instance);
                    //修改Book预约人数减一
                    Book::updateBooksAppointmentNum(instance->getIsbn(),-1);
                    cout << "还书成功,该书已被预约,请归还到总台,不流入图书馆!" << endl;
                }
            }





        }
    }
    return true;

}

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

        printf("ISBN输入0取消操作\n");
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
    book->readAndSetPosition();
    book->readAndSetStatus();
    book->updateBookInstanceModifiableInfo();
	printf("修改成功\n");
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




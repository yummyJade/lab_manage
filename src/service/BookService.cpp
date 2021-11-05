#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"
#include "../../src/core/Input.cpp"

bool dealWithOverTimeAppoint(std::string isbn, vector<BookInstance>* saveInstances=NULL){

    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    if (books.size() <= 0) {
        cout << "��ͼ�鲻����" << endl;
        return false;
    }
    Book book = books[0];
    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());
    if (saveInstances != NULL) {
        *saveInstances = instances;
    }


    //�Ȳ�һ��״̬�������״̬Ϊ5����ʵ����ԤԼ�ˣ��ټ�������ȥ�����򲻹�
    int appointmentIndex = 5;      //��ʾ����ԤԼ�ѵ�δȡ
    for(int i = 0; i < instances.size(); i++) {
        if(instances[i].getStatus() == appointmentIndex) {
            vector<Order> oweOrders = Order::getAssignBookOweAppointing(instances[i].getId());
            int i = 0;
            if(oweOrders.size() == 1){
                oweOrders[i].setStatu(static_cast<Status>(6));     //�������޸ĳ�ԤԼ���ڵ�״̬
                oweOrders[i].updateStateAndReturnTimeById(oweOrders[i]);      //�־û�
                BookInstance *instance = BookInstance::getInstanceById(oweOrders[i].getBookId());
                //�޸��鼮ʵ����״̬Ϊ�ɽ�
                //todo:���ж���û����ԤԼ���������ԤԼ�����Ȿ�����һ��ԤԼ���ˣ������Ϊ�ɽ�
                if(book.getAppointmentNum() == 0){

                    instance->setStatus(1);
                    //���º������Ĳ���
                    BookInstance::updateStateAndReturnTimeById(*instance);
                } else if(book.getAppointmentNum() > 0) {
                    //����ԤԼ����,�ҵ��Ǹ�ԤԼ����,������һ��
                    //�ҵ���������bookid==isbn && status = 3�ĵ���
                    vector<Order> orders = Order::getAssignBookAppointingList(book.getId());
                    int earliestIndex = 0;       //����ʱ��������±�
                    SimpleTime earliestDate = orders[0].getBorrowTime();         //��¼ʱ��������һ�죬����ĳ�ֵ����order�����һ��Ҳ�У�
                    for (int i = 1; i < orders.size(); ++i) {
                        //�������Ƚ�ʱ��ǰ��
                        if(earliestDate.compare(orders[i].getBorrowTime()) > 0) {    //����0˵����ǰʱ���compare���Ҫ��Ҳ����Ҫ����
                            earliestIndex = i;
                            earliestDate = orders[i].getBorrowTime();
                        }
                    }
                    //�ҵ������Ժ���Ķ�Ӧ��������Ϣ����bookId�޸�Ϊ��ǰinstanceId����status��Ϊ5��������ʱ�䱣��Ϊ��ǰʱ�䣬�����ж�ԤԼ����
                    orders[earliestIndex].setStatu(static_cast<Status>(5));
                    //���ʵ��id�������ô��
                    orders[earliestIndex].setBookId(instance->getId());
                    //�²�������borrowTime�ֶν��и��£�����뱣��ԤԼʱ��Ļ���Ҳ�����ù黹ʱ��������¼
//        orders[earliestIndex].setReturnTime(SimpleTime::nowTime().addDay(this->getCanLendDays()));
                    User earliestUser = User::getUserByJobNum(orders[earliestIndex].getUserId());       //��ȡ�ü�¼���û�
                    orders[earliestIndex].setBorrowTime(SimpleTime::nowTime().addDay(earliestUser.getCanAppointDays()));
                    Order::updateStateAndBookIdAndBorrowTimeById(orders[earliestIndex]);
//        cout << "order��bookInsIdΪ" << instance->getId() << endl;
                    //�޸�BookInstance��״̬statusΪ5,����ֻ�޸���һ����᲻�ᱨ��
                    instance->setStatus(5);
                    //todo:�ǵ�����һ���޸ĵ���status�ĺ���
                    BookInstance::updateStateAndReturnTimeById(*instance);
                    //�޸�BookԤԼ������һ
                    Book::updateBooksAppointmentNum(instance->getIsbn(),-1);
                    cout << "����ɹ�,�����ѱ�ԤԼ,��黹����̨,������ͼ���!" << endl;
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
        cout << "��ͼ�鲻����" << endl;
        return false;
    }
    Book book = books[0];
    book.printBookInfo();
    vector<string> navs = {"���", "ͼ��λ��", "״̬\\Ԥ�ƹ黹ʱ��"};
    TableRenderer render(navs, 8);

    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());
	if (saveInstances != NULL) {// �ѽ�����ͳ�ȥ
		*saveInstances = instances;
	}
    BookInstance::printBookInstanceList(instances);

    return true;
}


bool changeAssignBookInfo() {
    string isbn;
    Book tempBook;
    while (true) {

        printf("ISBN����0ȡ������\n");
        isbn=tempBook.readAndSetIsbn();
		if (isbn == "0") return false;
        if (Book::checkAssignISBNExist(isbn) == -1) { // ͼ�鲻����
            printf("isbnΪ%s��ͼ�鲻����!\n", isbn.c_str());
        } else {
            break;
        }
    }


    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    Book::printBookList(books);

    Book book = books[0];
    printf("�������޸ĺ����Ϣ\n");

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
        printf("������Ҫ�޸ĵ��鼮�������(����0����):");
        id=Input::getInt();
		if (id == 0) return false;
        if (!BookInstance::checkAssignBookInstanceIdExist(id)) { // ͼ�鲻����
            printf("�����Ϊ%d��ͼ�鲻����!\n", id);
        } else {
            break;
        }
    }

    BookInstance *book = BookInstance::getInstanceById(id);
    printf("������������Ϣ\n");
    book->printLine();

    printf("�������޸ĺ����Ϣ\n");
    book->readAndSetPosition();
    book->readAndSetStatus();
    book->updateBookInstanceModifiableInfo();
	printf("�޸ĳɹ�\n");
    return true;
}


bool deleteAssignIsbnBook(std::string isbn="") {
	// ����isbn��
	if (isbn == "") {// û�д�isbn��������
		while (true) {
			printf("������Ҫ�޸ĵ��鼮��ISBN(����0����):");
			isbn=Input::getAssignMaxLengthStr(20);
			if (isbn == "0") return false;
			if (Book::checkAssignISBNExist(isbn) == -1) { // ͼ�鲻����
				printf("isbnΪ%s��ͼ�鲻����!\n", isbn.c_str());
			}
			else {
				break;
			}
		}
	}

	//��������ӡ��isbn��ͼ����Ϣ �Լ� ����ͼ��ʵ������Ϣ
	vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
	Book::printBookList(books); // ��ӡ����ͼ��ʵ����Ϣ

	vector<BookInstance> instances = BookInstance::getInstancesByFirstId(books[0].getFirstInstanceId());
	BookInstance::printBookInstanceList(instances);// ��ӡ����ͼ��ʵ����Ϣ

	//���û�ѡ���Ƿ�Ҫȫ���¼�
	cout << "����Y�¼�ISBNΪ" << isbn << "������ͼ��,����Nȡ��:";
	char operate;
	operate=Input::getChar();
	if (operate == 'Y' || operate == 'y') {// �¼�����ͼ��
		// �޸�Book�ݲ���Ϊ0
		Book::updateBooks("isbn",isbn,"count","0");

		// �޸�instance��״̬
		for (int i = 0; i < instances.size(); i++) {
			instances[i].setStatus(3);// 3Ϊ���¼�
			BookInstance::updateStateAndReturnTimeById(instances[i]);
		}
		cout<<"�ѳɹ��¼�ISBNΪ" << isbn << "������ͼ��"<<endl;
		return true;
	}
	cout << "��ȡ������" << endl;
	return false;
}




#include "service/BookService.h"
#include <iostream>
#include <string>
#include <model/User.h>
#include "util/TableRenderer.h"

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
    int id;
    while (true) {
        printf("������Ҫ�޸ĵ��鼮�������:");
        cin >> id;
        if (!BookInstance::checkAssignBookInstanceIdExist(id)) { // ͼ�鲻����
            printf("�����Ϊ%d��ͼ�鲻����!", id);
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


bool deleteAssignIsbnBook(std::string isbn="") {
	// ����isbn��
	if (isbn == "") {// û�д�isbn��������
		while (true) {
			printf("������Ҫ�޸ĵ��鼮��ISBN(����0����):");
			cin >> isbn;
			if (isbn == "0") return false;
			if (Book::checkAssignISBNExist(isbn) == -1) { // ͼ�鲻����
				printf("isbnΪ%s��ͼ�鲻����!", isbn.c_str());
				return false;
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
	cin >> operate;
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




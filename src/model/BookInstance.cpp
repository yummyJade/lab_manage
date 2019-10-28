#include "../../include/model/BookInstance.h"
//#include "../../src/model/BookInstance.cpp"
#include "../../linkDatebase/include/TableBookcopy.h"
#include <iostream>
#include <vector>
#include <string>
#include <model/Book.h>
#include <util/TableRenderer.h>

using namespace std;

const string BookInstance::BOOK_INSTANCE_FIELDS[] = {"", "ID", "ISBN", "STATE", "POSITION", "PLANRETURNDATE"};

BookInstance::BookInstance(const string &isbn, int status, const string &position) : isbn(isbn), status(status),
                                                                                     position(position) {}

BookInstance::BookInstance(const string &isbn, int status, const string &position, const Date &planReturnDate) : isbn(
        isbn), status(status), position(position), planReturnDate(planReturnDate) {}

BookInstance::BookInstance(const string &isbn, const string &position) : isbn(isbn), position(position) {
    this->status = 1;
}

int BookInstance::addBookInstance(BookInstance instance, int firstId) {
    instance.toBookCopy();
    TableBookcopy *tableBookcopy = TableBookcopy::getInstance();
    int result = tableBookcopy->insertData(firstId, instance.toBookCopy(), 1);
    return result;// ����id
}

int BookInstance::importBookInstances(std::vector<BookInstance> instances, int firstId = -1) {
    int index = 0;
    if (firstId == -1) {//������״β���,�Ȳ�һ��
        firstId = addBookInstance(instances[index++], firstId);
    }

    while (index < instances.size()) {// ����δ�����ʵ��ȫ������
        addBookInstance(instances[index++], firstId);
    }
    return firstId;
}

BookInstance *BookInstance::getInstanceById(int id) {
    BookInstance *bookInstance = new BookInstance("ISBN", "position");
    TableBookcopy *table = TableBookcopy::getInstance();
    vector<Bookcopy> results = table->query(id);
    if (results.size() > 0) {
        BookInstance result = BookInstance::BookCopyToBookInstance(results[0]);
        return new BookInstance(result);
    }
    return NULL;
}

std::vector<BookInstance> BookInstance::getInstancesByFirstId(int firstId) {
    vector<BookInstance> result;
    TableBookcopy *table = TableBookcopy::getInstance();
    vector<Bookcopy> copys = table->queryByBookId(firstId);
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(BookInstance::BookCopyToBookInstance(copys[i]));
    }

    // mock
//    Date date(2019, 11, 10);
//    BookInstance instance("const std::string &isbn", 1, "л���µĴ�ͷ��", date);
//    BookInstance instance2("const std::string &isbn", 2, "л���µ����", date);
//    BookInstance instance3("const std::string &isbn", 3, "�ű�B505", date);
//    result.push_back(instance);
//    result.push_back(instance2);
//    result.push_back(instance3);

    return result;
}

bool BookInstance::addOneBookInstancesService() {
    printf("���������� isbn,λ��,ͼ��״̬(����),��Ч����,�ÿո����\n");
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


    int firstId = Book::checkAssignISBNExist(isbn);// �ж�ͼ���Ƿ����
    if (firstId == -1) {// ������鲻����
        cout << "ͼ�������ISBNΪ" << isbn << "���鼮,������Ӹ������ִ�����ʵ������" << endl;
        return false;
    } else {

        vector<BookInstance> bookinstancesFirstAdd;
        for (int i = 0; i < count; ++i) {
            BookInstance bookInstance(isbn, state, position);
            bookinstancesFirstAdd.push_back(bookInstance);
        }
        long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                      firstId);//��ȡ����ĵ�һ��λ��

        vector<int> counts;
        counts.push_back(count);
        vector<string> isbns;
        isbns.push_back(isbn);
        Book::updateBooksCount(isbns, counts);
    }
    return true;

}

bool BookInstance::deleteInstancesByAssignInstanceId(int id) {
    return false;
}

bool
BookInstance::updateAssignFieldsById(long long id, std::vector<std::string> fieds, std::vector<std::string> values) {

    return false;
}

bool BookInstance::checkAssignBookInstanceIdExist(long long id) {
    return BookInstance::getInstanceById(id) != NULL;
}

bool BookInstance::checkAssignBookCanAppointmentInstanceExist(std::string isbn) {
    vector<Book> books = Book::searchBooksBySingleField("isbn", isbn);
    if (books.size() <= 0) {
        cout << "��ͼ�鲻����" << endl;
        return false;
    }
    Book book = books[0];

    vector<BookInstance> instances = BookInstance::getInstancesByFirstId(book.getFirstInstanceId());
    int canlendInstanceIndex = 1;        //�����ɽ��instance�����ܽ��ˣ������˼�ΪʲôҪԤԼ��
    int canAppointInstanceIndex[] = {2,5};        //������ԤԼ��instance��Ҳ���Ƿ��¼��Լ��Ѿ���ԤԼ���飬��֮��˼���ǣ��黹���еģ���������������
    for(int i = 0; i < instances.size(); ++i) {
        if(instances[i].getStatus() == canlendInstanceIndex) {
            cout << "�пɽ豾!" << endl;
            return false;
        }
        for(int j = 0; j < sizeof(canAppointInstanceIndex) / sizeof(int); ++j) {
            if(instances[i].getStatus() == canAppointInstanceIndex[j]) {
                cout << "������" << endl;
                return true;
            }
        }
    }
    return true;

}

void BookInstance::printBookInstanceList(std::vector<BookInstance> instances) {
    vector<string> navs = {"���", "�����", "ͼ��λ��", "״̬\\Ԥ�ƹ黹ʱ��"};
    TableRenderer render(navs, 8);
    int index = 1;

    for (int i = 0; i < instances.size(); ++i) {
        vector<string> line;
        if (instances[i].status == 1) {// �ɽ�
            line = {to_string(index++), to_string(instances[i].getId()), instances[i].position, "�ɽ�"};
        } else {
            line = {to_string(index++), to_string(instances[i].getId()), instances[i].position,
                    instances[i].planReturnDate.serialize()};
        }
        render.addColume(line);
    }
    render.render();
}

bool BookInstance::printLine() {
    printf("�����:%d,isbn:%s,�ݲ�λ��:%s,״̬:%s", this->id, this->isbn.c_str(), this->position.c_str(), this->status);
    return true;
}

int BookInstance::getId() const {
    return id;
}

const string &BookInstance::getIsbn() const {
    return isbn;
}

int BookInstance::getStatus() const {
    return status;
}

const string &BookInstance::getPosition() const {
    return position;
}

const Date &BookInstance::getPlanReturnDate() const {
    return planReturnDate;
}

void BookInstance::setIsbn(const string &isbn) {
    BookInstance::isbn = isbn;
}

void BookInstance::setStatus(int status) {
    BookInstance::status = status;
}

void BookInstance::setPosition(const string &position) {
    BookInstance::position = position;
}

void BookInstance::setPlanReturnDate(const Date &planReturnDate) {
    BookInstance::planReturnDate = planReturnDate;
}

bool BookInstance::updateBookInstanceModifiableInfo() {
//    BookInstance::updateAssignFieldsById("");
    return false;
}

std::vector<std::string> BookInstance::getPrintLineStr() {
    vector<string> info;
    info.push_back(to_string(this->id));
    info.push_back(this->position);
    info.push_back(this->getStatusStr());

    return info;
}

std::string BookInstance::getStatusStr() {
    // ״̬ 1 �ɽ� 2 �ѱ����� 3 ��ɾ��(��ʧ���¼�) 4 �ѱ��������ѱ�ԤԼ
    switch (this->status) {
        case 1:
            return "�ɽ�";
        case 2:
            return "�ѱ�����1";
        case 3:
            return "��ɾ��";
        case 4:
            return "�ѱ�����2";
    }
}


bool BookInstance::changeStateAndPersistence(int newState) {
    this->setStatus(newState);


    return false;
}

Bookcopy BookInstance::toBookCopy() {
    Bookcopy bookcopy;

    bookcopy.setId(this->getId());
//    char* isbn= const_cast<char *>(this->getIsbn().c_str());
    bookcopy.setIsbn(const_cast<char *>(this->getIsbn().c_str()));
    bookcopy.setState(this->getStatus());
    bookcopy.setPosition(const_cast<char *>(this->getPosition().c_str()));

    Date date = this->getPlanReturnDate();
    int time = date.toInt();
    bookcopy.setReTime(time);

    return bookcopy;
}


BookInstance BookInstance::BookCopyToBookInstance(Bookcopy bookcopy) {
    Date planReturnDate = Date::intDate2Date(bookcopy.getReTime());
    return BookInstance(bookcopy.getId(), bookcopy.getIsbn(), bookcopy.getState(), bookcopy.getPosition(),
                        planReturnDate);
}

BookInstance::BookInstance(int id, const string &isbn, int status, const string &position, const Date &planReturnDate)
        : id(id), isbn(isbn), status(status), position(position), planReturnDate(planReturnDate) {}

bool BookInstance::updateStateAndReturnTimeById(BookInstance book) {
    TableBookcopy *table = TableBookcopy::getInstance();
    vector<int> changeIndex = {2, 3};
    table->update(book.getId(), book.toBookCopy(), changeIndex);
    return false;
}





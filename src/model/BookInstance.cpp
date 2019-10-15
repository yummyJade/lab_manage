#include "../../include/model/BookInstance.h"
#include <iostream>
#include <vector>
#include <string>
#include <model/Book.h>

using namespace std;


BookInstance::BookInstance(const string &isbn, int status, const string &position) : isbn(isbn), status(status),
                                                                                     position(position) {}

BookInstance::BookInstance(const string &isbn, int status, const string &position, const Date &planReturnDate) : isbn(
        isbn), status(status), position(position), planReturnDate(planReturnDate) {}

BookInstance::BookInstance(const string &isbn, const string &position) : isbn(isbn), position(position) {}

int BookInstance::addBookInstance(BookInstance instance, int firstId) {
    // todo: �������ݿ�
    return 66;
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
    return bookInstance;
}

std::vector<BookInstance> BookInstance::getInstancesByFirstId(int firstId) {
    vector<BookInstance> result;
    Date date(2019, 11, 10);
    BookInstance instance("const std::string &isbn", 1, "л���µĴ�ͷ��", date);
    BookInstance instance2("const std::string &isbn", 2, "л���µ����", date);
    BookInstance instance3("const std::string &isbn", 3, "�ű�B505", date);
    result.push_back(instance);
    result.push_back(instance2);
    result.push_back(instance3);

    return result;
}

bool BookInstance::addOneBookInstancesService() {
    printf("���������� isbn,λ��,ͼ��״̬,��Ч����,�ÿո����\n");
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

bool BookInstance::deleteOneBookInstancesService() {
    printf("����������Ҫɾ����ͼ��������\n");
    int bookInstanceId;
    cin >> bookInstanceId;

    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);
    if (instance == NULL) {//���ͼ�鲻����
        cout << "ɾ��ʧ��,ϵͳ��û��������Ϊ" << bookInstanceId << "���鼮" << endl;
        return false;
    }
    BookInstance::deleteInstancesByAssignInstanceId(bookInstanceId);
    return true;
}

bool BookInstance::deleteInstancesByAssignInstanceId(int id) {
    return false;
}




#include "../../include/model/Book.h"
#include <iostream>
#include <core/SimpleString.h>
#include <fstream>
#include <sstream>
#include <model/BookInstance.h>
#include "../../include/util/DbAdapter.h"
using namespace std;

Book::Book() {
}


Book::~Book() {
}

string Book::printType() {
    string temp;
    switch (type) {
        case 'A':temp = "���˼���塢�������塢ë��˼�롢��Сƽ����";break;
        case 'B':temp = "��ѧ���ڽ�";break;
        case 'C':temp = "����ѧ����";break;
        case 'D':temp = "���Ρ�����";break;
        case 'E':temp = "����";break;
        case 'F':temp = "����";break;
        case 'G':temp = "�Ļ�����ѧ������������";break;
        case 'H':temp = "���ԡ�����";break;
        case 'I':temp = "��ѧ";break;
        case 'J':temp = "����";break;
        case 'K':temp = "��ʷ������";break;
        case 'N':temp = "��Ȼ��ѧ����";break;
        case 'O':temp = "�����ѧ�ͻ�ѧ";break;
        case 'P':temp = "����ѧ�������ѧ";break;
        case 'Q':temp = "�����ѧ";break;
        case 'R':temp = "ҽҩ������";break;
        case 'S':temp = "ũҵ��ѧ";break;
        case 'T':temp = "��ҵ����";break;
        case 'U':temp = "��ͨ����";break;
        case 'V':temp = "���ա�����";break;
        case 'X':temp = "������ѧ����ȫ��ѧ";break;
        case 'Z':temp = "�ۺ���ͼ��";break;
        default:temp = "�ڿ���־";break;
    }
    return temp;
}

string Book::printIsLend() {
//    return isLend ? "�ɽ�" : "���ɽ�";
//    if (isLend) {
//        return "�ɽ�";
//    } else if (!isLend) {
//        return "���ɽ�";
//    }
}

void Book::printBookInfo() {
    cout << id << name << author << isbn << type;
}




std::vector<std::string> Book::serialize() {
    vector<string> info;
//    info.push_back(to_string(this->id));
    info.push_back(to_string(this->type));
    info.push_back(to_string(this->count));
//    info.push_back(to_string(this->isValid));
    info.push_back(to_string(this->price));
    info.push_back(this->name);
    info.push_back(this->author);
    info.push_back(this->isbn);
    info.push_back(this->press);
//    info.push_back(this->position);
    return info;
}

bool Book::deSerialize(std::vector<std::string> info) {
    char type = info[0].data()[0];
//    bool isLend = info[1].data() == "true";
//    bool isValid = info[2].data() == "true";
    int price = (int) (info[3].data());
    string name = info[4].data();
    string author = info[5].data();
    string isbn = info[6].data();
    string press = info[7].data();
    string postion = info[8].data();
    long long id = (long long) info[8].data();

    Book(type,price,id,name,author,isbn,press);
    return true;
}

std::vector<std::vector<std::string>> Book::searchBooksBySingleField(std::string field, std::string value) {
    DbAdapter dbAdapter("Book");
    vector<vector<string> > queryData = dbAdapter.searchBySingleField(field, value);
    return queryData;
}


void Book::printBookList(std::vector<std::vector<std::string>> queryData) {
    for (ll i = 0; i < queryData.size(); i++) {
//        printf("2\n");
        for (ll j = 0; j < queryData[0].size(); j++) {
            printf("%s\t", queryData[i][j].data());
        }
        printf("\n");
    }
}

bool
Book::updateBooks(std::string assignField, std::string assignValue, std::string changeField, std::string changeValue) {
    DbAdapter dbAdapter("��");
    dbAdapter.updateBySingleField(assignField, assignValue, changeField, changeValue);
    return true;
}

bool Book::deleteBooksByBookIds(std::vector<std::string> bookIds) {
    for (int i = 0; i < bookIds.size(); ++i) {
        Book::updateBooks("Id", bookIds[i].data(), "isValid", "false");
    }
    return true;
}

std::vector<std::vector<std::string>> Book::searchAll() {
    DbAdapter dbAdapter("Book");
    return dbAdapter.searchAll();
}

Book::Book(char type, int count, int price, const string &name, const string &author, const string &isbn,
           const string &press) : type(type), count(count), price(price), name(name), author(author), isbn(isbn),
                                  press(press) {}

bool Book::addBooks(std::vector<std::vector<std::string>> queryData, vector<long long> &ids) {
    DbAdapter dbAdapter("Book");

    dbAdapter.insert(queryData, ids);
    return true;

}

int Book::checkAssignISBNExist(std::string isbn) {
    DbAdapter dbAdapter("Book");
    vector<vector<string>> result = dbAdapter.searchBySingleField("ISBN", isbn);
    if (result.size() > 0) {
//        cout << "ĩβԪ����" << result[0].back();
        return stoi(result[0].back());//todo :��������ħ��,����,Ҫ��֤���һ�������ǵ�һ��id
    }
    return -1;
}

bool Book::importBooks() {
    /* ��ȡһ����Ч·��,�������Ӧ���ļ�*/
    string path;
    ifstream fin;
    string line;
    while (true) {
        path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
        fin = ifstream(path);//���ļ�������
        if (fin.good()) {
            cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
            break;
        }

        cout << "�ļ�������,����·������������" << endl;
    }


    /* ���ж�һ�����Ҫ������鼮������Щ���Ѿ����ڵ�*/
    getline(fin, line); // �Ե�����
    vector<string> isbns;
    while (getline(fin, line)) //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
    {
        istringstream sin(line);
        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
//        string isbn = fields[4];
        isbns.push_back(fields[4]);
    }
    vector<int> isExists = Book::checkISBNsExist(isbns);


    /* �Բ�ͬ������鼮ִ�в�ͬ����*/
    int index = 0;//Ҫ���������±�
    fin = ifstream(path);//���ļ�������
    getline(fin, line); // �Ե�����
    vector<vector<string>> newBooks; // ��Щ����Ҫinsert��Book��
    vector<vector<string>> updateBooks; // ��Щ����Ҫ��update Book�����е�����

    while (getline(fin, line)) //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
    {
        istringstream sin(line);

        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
        string name = fields[0];
        string author = fields[1];
        string press = fields[2];
        char type = fields[3][0];
        string isbn = fields[4];
        int price = stof(fields[5]) * 100;
        string position = fields[6];
        int count = stoi(fields[7]);

        vector<BookInstance> bookinstancesFirstAdd;
        for (int i = 0; i < count; ++i) {
            BookInstance bookInstance(isbn, position);
            bookinstancesFirstAdd.push_back(bookInstance);
        }
        // ����bookʵ����instance��
        long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd);//��ȡ����ĵ�һ��λ��

        if (isExists[index] == -1) {//�����������״α���ӵ�
            Book book(type, count, price, firstInstanceId, name, author, isbn, press);
            newBooks.push_back(book.serialize());
        } else {//������������ʵ��
            // ����Book����ԭ���鼮�Ĺݲ�����
        }

        vector<BookInstance> instances;


    }

    // ����books
    vector<ll> ids;
    Book::addBooks(newBooks, ids);
    return true;
}

Book::Book(char type, int count, int price, long long int firstInstanceId, const string &name, const string &author,
           const string &isbn, const string &press) : type(type), count(count), price(price),
                                                      firstInstanceId(firstInstanceId), name(name), author(author),
                                                      isbn(isbn), press(press) {}

std::vector<int> Book::checkISBNsExist(std::vector<std::string> isbns) {
    vector<int> results;
    for (int i = 0; i < isbns.size(); ++i) {
        results.push_back(Book::checkAssignISBNExist(isbns[i]));
    }
    return results;
}



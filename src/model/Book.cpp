#include "../../include/model/Book.h"
#include <iostream>
#include <core/SimpleString.h>
#include <fstream>
#include <sstream>
#include <model/BookInstance.h>
#include "../../include/util/DbAdapter.h"
#include "../../include/util/TableRenderer.h"

using namespace std;

Book::Book() {
}


Book::~Book() {
}

string Book::getTypeContent() {
    string temp;
    switch (type) {
        case 'A':
            temp = "���˼���塢�������塢ë��˼�롢��Сƽ����";
            break;
        case 'B':
            temp = "��ѧ���ڽ�";
            break;
        case 'C':
            temp = "����ѧ����";
            break;
        case 'D':
            temp = "���Ρ�����";
            break;
        case 'E':
            temp = "����";
            break;
        case 'F':
            temp = "����";
            break;
        case 'G':
            temp = "�Ļ�����ѧ������������";
            break;
        case 'H':
            temp = "���ԡ�����";
            break;
        case 'I':
            temp = "��ѧ";
            break;
        case 'J':
            temp = "����";
            break;
        case 'K':
            temp = "��ʷ������";
            break;
        case 'N':
            temp = "��Ȼ��ѧ����";
            break;
        case 'O':
            temp = "�����ѧ�ͻ�ѧ";
            break;
        case 'P':
            temp = "����ѧ�������ѧ";
            break;
        case 'Q':
            temp = "�����ѧ";
            break;
        case 'R':
            temp = "ҽҩ������";
            break;
        case 'S':
            temp = "ũҵ��ѧ";
            break;
        case 'T':
            temp = "��ҵ����";
            break;
        case 'U':
            temp = "��ͨ����";
            break;
        case 'V':
            temp = "���ա�����";
            break;
        case 'X':
            temp = "������ѧ����ȫ��ѧ";
            break;
        case 'Z':
            temp = "�ۺ���ͼ��";
            break;
        default:
            temp = "�ڿ���־";
            break;
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
    this->getTypeContent();
    printf("---------------------------------------------------------\n");
    printf("����:%s\n����:%s\n������:%s\n���:%s\nISBN:%s\n�۸�:%.2f\n", name.c_str(), author.c_str(), press.c_str(),
           this->getTypeContent().c_str(), isbn.c_str(), price / 100);
    printf("---------------------------------------------------------\n");
//    cout << id << name << author << isbn << type;
}


std::vector<std::string> Book::serialize() {
    vector<string> info;
    info.push_back(to_string(this->type));
    info.push_back(to_string(this->count));
    info.push_back(to_string(this->price));
    info.push_back(to_string(this->appointmentNum));
    info.push_back(this->name);
    info.push_back(this->author);
    info.push_back(this->isbn);
    info.push_back(this->press);
    info.push_back(to_string(this->firstInstanceId));
    return info;
}

bool Book::deSerialize(std::vector<std::string> info) {
//    for (int i = 0; i < info.size(); ++i) {
//        cout << i << " " << info[i].data() << " ";
//    }
    char type = info[0].data()[0];
    int count = stoi(info[1].data());
    int price = stoi(info[2].data());
    int appointmentNum = stoi(info[3].data());
    string name = info[4].data();
    string author = info[5].data();
    string isbn = info[6].data();
    string press = info[7].data();
    int firstInstanceId = atoi(info[8].data());

    new(this) Book(type, count, appointmentNum, price, firstInstanceId, name, author, isbn, press);
//    this->Book::Book();
//    Book(type, count, price, firstInstanceId, name, author, isbn, press);
    return true;
}


std::vector<Book> Book::searchBooksBySingleField(std::string field, std::string value) {
    DbAdapter dbAdapter("Book");
    vector<vector<string> > queryData = dbAdapter.searchBySingleField(field, value);
//    cout<<"ִ�е�����"<<endl;
    return Book::stringsToBooks(queryData);
}


void Book::printBookList(std::vector<Book> books) {
    vector<string> navs = {"���", "����", "����", "������", "����", "�ݲ���", "ISBN"};
    TableRenderer render(navs, 8);

    for (int i = 0; i < books.size(); ++i) {
        vector<string> line;
        vector<string> temp = books[i].getPrintLineStr();
        line.push_back(to_string(i + 1));
        line.insert(line.end(), temp.begin(), temp.end());
        render.addColume(line);
//        render.addColume(books[i].getPrintLineStr());
    }
    render.render();
}

bool
Book::updateBooks(std::string assignField, std::string assignValue, std::string changeField, std::string changeValue) {
    DbAdapter dbAdapter("Book");
    dbAdapter.updateBySingleField(assignField, assignValue, changeField, changeValue);
    return true;
}

bool Book::deleteBooksByBookIds(std::vector<std::string> bookIds) {
    for (int i = 0; i < bookIds.size(); ++i) {
        Book::updateBooks("Id", bookIds[i].data(), "isValid", "false");
    }
    return true;
}

std::vector<Book> Book::searchAll() {
    vector<Book> result;
    DbAdapter dbAdapter("Book");
    vector<vector<string>> infos = dbAdapter.searchAll();
    for (int i = 0; i < infos.size(); ++i) {
        Book book;
        book.deSerialize(infos[i]);
        result.push_back(book);
    }
    return result;
}

Book::Book(char type, int count, int price, const string &name, const string &author, const string &isbn,
           const string &press) : type(type), count(count), price(price), name(name), author(author), isbn(isbn),
                                  press(press) {
    this->appointmentNum = 0;
}

bool Book::addBooks(std::vector<std::vector<std::string>> queryData, vector<long long> &ids) {
    DbAdapter dbAdapter("Book");
    dbAdapter.insert(queryData, ids);

    return true;

}

int Book::checkAssignISBNExist(std::string isbn) {
    DbAdapter dbAdapter("Book");
    vector<vector<string>> result = dbAdapter.searchBySingleField("isbn", isbn);
    if (result.size() > 0) {
//        cout<<"isbn:"<<isbn<<" ��"<<endl;
//        cout << "ĩβԪ����" << result[0].back();
        Book book;
        book.deSerialize(result[0]);
        return book.getFirstInstanceId();
    }
//    cout<<"isbn:"<<isbn<<" û��"<<endl;
    return -1;
}

bool Book::importBooksService() {
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
        isbns.push_back(fields[4]);
    }
    vector<int> isExists = Book::checkISBNsExist(isbns);

    /* �Բ�ͬ������鼮ִ�в�ͬ����*/
    int index = 0;//Ҫ���������±�
    fin.clear();
    fin.seekg(0, ios::beg); // ������ת���ļ�ͷ��
    getline(fin, line); // �Ե�����
    vector<vector<string>> newBooks; // Ҫinsert��Book�������
    vector<string> updateIsbns; // �Ѿ����ڵģ�ֻ��Ҫ���¹ݲ�����isbn��
    updateIsbns.empty();
    vector<int> addCounts; // �Ѿ����ڵģ�Ҫ��ӵĹݲ���

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
//        cout<<"first Id is"<<isExists[index]<<endl;
        long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                      isExists[index]);//��ȡ����ĵ�һ��λ��

        if (isExists[index] == -1) {//�����������״α���ӵ�
            Book book(type, count, price, firstInstanceId, name, author, isbn, press);
            newBooks.push_back(book.serialize());
        } else {//������������ʵ��
            // ��ӵ�������Ϣ����
//            cout<<"index is "<<index<<" isExi[index] is "<< firstInstanceId<<endl;
            updateIsbns.push_back(isbn);
            addCounts.push_back(count);
        }
        index++;
    }
    // ����books
//    cout << "�������,��������" <<newBooks.size()<< endl;
    vector<ll> ids;
    Book::addBooks(newBooks, ids);
    // ����ͼ��Ĺݲ���
//    cout << "update����,update����"<<updateIsbns.size() << endl;
    if (updateIsbns.size() > 0)
        Book::updateBooksCount(updateIsbns, addCounts);
    return true;
}



std::vector<int> Book::checkISBNsExist(std::vector<std::string> isbns) {
    vector<int> results;
    for (int i = 0; i < isbns.size(); ++i) {
        results.push_back(Book::checkAssignISBNExist(isbns[i].data()));
    }
    return results;
}


bool Book::updateBooksCount(std::vector<std::string> isbns, std::vector<int> addCount) {
    for (int i = 0; i < isbns.size(); ++i) {
        int oldCount = Book::searchBooksBySingleField("isbn", isbns[i].data())[0].count;
//        cout << "old count is " << oldCount << endl;
        Book::updateBooks("isbn", isbns[i].data(), "count", to_string(oldCount + addCount[i]));
    }
    return true;
}


bool Book::exportBooks(std::vector<Book> books) {
    string path = SimpleString::readPathFromCmd();
    std::ofstream saveFile(path);
    for (int j = 0; j < books.size(); ++j) {
        saveFile << books[j].name << books[j].author << books[j].press << books[j].getTypeContent() << books[j].isbn
                 << books[j].price << books[j].count << endl;
    }
    saveFile.close();
    return true;
}

std::vector<Book> Book::stringsToBooks(std::vector<std::vector<std::string>> books) {
    vector<Book> results;
//    cout<<"book ��size��"<<books.size()<<endl;
    for (int i = 0; i < books.size(); ++i) {
        Book book; // todo: �������Ҫ��new
        book.deSerialize(books[i]);
        results.push_back(book);
    }
    return results;
}

bool Book::addOneBookService() {
    printf("���������� ����,����,������,����,isbn,�۸�,λ��,��Ч����,�ÿո����\n");
    vector<string> fields;
    int index = 0;
    string temp_info;
    while (index < 8) {
        cin >> temp_info;
        fields.push_back(temp_info);
        index++;
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

    int firstId = Book::checkAssignISBNExist(isbn);
    long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                  firstId);//��ȡ����ĵ�һ��λ��

    if (firstId == -1) {// ������鲻����
        vector<vector<string>> books;
        Book book(type, count, price, firstInstanceId, name, author, isbn, press);
        books.push_back(book.serialize());
        vector<long long int> ids;
        Book::addBooks(books, ids);
    } else {
        vector<int> counts;
        counts.push_back(count);
        vector<string> isbns;
        isbns.push_back(isbn);
        Book::updateBooksCount(isbns, counts);
    }
    return true;
}


bool Book::batchDeleteAssignIsbnsBooks(std::vector<std::string> isbns) {
    for (int i = 0; i < isbns.size(); ++i) {
        Book::updateBooks("isbn", isbns[i].data(), "count", "0");
    }

    return true;
}

int Book::getFirstInstanceId() const {
    return firstInstanceId;
}

char Book::getType() const {
    return type;
}

int Book::getCount() const {
    return count;
}


int Book::getPrice() const {
    return price;
}

int Book::getId() const {
    return id;
}

const string &Book::getName() const {
    return name;
}

const string &Book::getAuthor() const {
    return author;
}

const string &Book::getIsbn() const {
    return isbn;
}

const string &Book::getPress() const {
    return press;
}

void Book::setType(char type) {
    Book::type = type;
}

void Book::setCount(int count) {
    Book::count = count;
}

void Book::setPrice(int price) {
    Book::price = price;
}

void Book::setId(int id) {
    Book::id = id;
}

void Book::setFirstInstanceId(long long int firstInstanceId) {
    Book::firstInstanceId = firstInstanceId;
}

void Book::setName(const string &name) {
    Book::name = name;
}

void Book::setAuthor(const string &author) {
    Book::author = author;
}

void Book::setIsbn(const string &isbn) {
    Book::isbn = isbn;
}

void Book::setPress(const string &press) {
    Book::press = press;
}

void Book::setAppointmentNum(int appointmentNum) {
    Book::appointmentNum = appointmentNum;
}

bool Book::updateBookModifiableInfo() {
    Book::updateBooks("isbn", this->isbn, "name", this->name);
    Book::updateBooks("isbn", this->isbn, "author", this->author);
    Book::updateBooks("isbn", this->isbn, "press", this->press);
    Book::updateBooks("isbn", this->isbn, "type", to_string(this->type));
    Book::updateBooks("isbn", this->isbn, "price", to_string(this->price));
    return true;
}

std::vector<std::string> Book::getPrintLineStr() {
    vector<string> info;
    info.push_back(this->name);
    info.push_back(this->author);
    info.push_back(this->press);

    info.push_back(this->getTypeContent());
    info.push_back(to_string(this->count));
    info.push_back(this->isbn);
    return info;
}

Book::Book(char type, int count, int appointmentNum, int price, int firstInstanceId, const string &name,
           const string &author, const string &isbn, const string &press) : type(type), count(count),
                                                                            appointmentNum(appointmentNum),
                                                                            price(price),
                                                                            firstInstanceId(firstInstanceId),
                                                                            name(name), author(author), isbn(isbn),
                                                                            press(press) {}

Book::Book(char type, int count, int price, int firstInstanceId, const string &name, const string &author,
           const string &isbn, const string &press) : type(type), count(count), price(price),
                                                      firstInstanceId(firstInstanceId), name(name), author(author),
                                                      isbn(isbn), press(press) {
    this->appointmentNum = 0;
}

int Book::getAppointmentNum() const {
    return appointmentNum;
}



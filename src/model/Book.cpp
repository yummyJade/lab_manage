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
        case 'A':temp = "马克思主义、列宁主义、毛泽东思想、邓小平理论";break;
        case 'B':temp = "哲学、宗教";break;
        case 'C':temp = "社会科学总论";break;
        case 'D':temp = "政治、法律";break;
        case 'E':temp = "军事";break;
        case 'F':temp = "经济";break;
        case 'G':temp = "文化、科学、教育、体育";break;
        case 'H':temp = "语言、文字";break;
        case 'I':temp = "文学";break;
        case 'J':temp = "艺术";break;
        case 'K':temp = "历史、地理";break;
        case 'N':temp = "自然科学总论";break;
        case 'O':temp = "数理科学和化学";break;
        case 'P':temp = "天文学、地球科学";break;
        case 'Q':temp = "生物科学";break;
        case 'R':temp = "医药、卫生";break;
        case 'S':temp = "农业科学";break;
        case 'T':temp = "工业技术";break;
        case 'U':temp = "交通运输";break;
        case 'V':temp = "航空、航天";break;
        case 'X':temp = "环境科学、安全科学";break;
        case 'Z':temp = "综合性图书";break;
        default:temp = "期刊杂志";break;
    }
    return temp;
}

string Book::printIsLend() {
//    return isLend ? "可借" : "不可借";
//    if (isLend) {
//        return "可借";
//    } else if (!isLend) {
//        return "不可借";
//    }
}

void Book::printBookInfo() {
    cout << id << name << author << isbn << type;
}




std::vector<std::string> Book::serialize() {
    vector<string> info;
    info.push_back(to_string(this->type));
    info.push_back(to_string(this->count));
    info.push_back(to_string(this->price));
    info.push_back(this->name);
    info.push_back(this->author);
    info.push_back(this->isbn);
    info.push_back(this->press);
//    info.push_back(this->position);
    return info;
}

bool Book::deSerialize(std::vector<std::string> info) {
    for (int i = 0; i < info.size(); ++i) {
        cout << i << " " << info[i].data() << " ";
    }
    cout << "序列化" << endl;
    char type = info[0].data()[0];
    int count = (int) (info[1].data());
    int price = (int) (info[2].data());
    string name = info[3].data();
    string author = info[4].data();
    cout << "序列化1" << endl;
    string isbn = info[5].data();
    string press = info[6].data();
//    string postion = info[8].data();
    cout << "序列化2" << endl;
    long long id = (long long) info[7].data();

    cout << "取出的count 是" << count << endl;
    Book(type, count, price, id, name, author, isbn, press);// todo:这个不是id

    cout << "序列化3" << endl;
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
    vector<vector<string>> result = dbAdapter.searchBySingleField("isbn", isbn);
    if (result.size() > 0) {
//        cout<<"isbn:"<<isbn<<" 有"<<endl;
//        cout << "末尾元素是" << result[0].back();
        return stoi(result[0].back());//todo :这里用了魔数,不对,要保证最后一个参数是第一本id
    }
//    cout<<"isbn:"<<isbn<<" 没有"<<endl;
    return -1;
}

bool Book::importBooks() {
    /* 读取一个有效路径,并打开其对应的文件*/
    string path;
    ifstream fin;
    string line;
    while (true) {
        path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
        fin = ifstream(path);//打开文件流操作
        if (fin.good()) {
            cout << "已找到文件,正在读取" << endl;
            break;
        }

        cout << "文件不存在,请检查路径后重新输入" << endl;
    }


    /* 先判断一下这次要导入的书籍种类哪些是已经存在的*/
    getline(fin, line); // 吃掉首行
    vector<string> isbns;
    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
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


    /* 对不同情况的书籍执行不同操作*/
    int index = 0;//要操作的行下标
    fin = ifstream(path);//打开文件流操作
    getline(fin, line); // 吃掉首行
    vector<vector<string>> newBooks; // 这些数据要insert到Book表
    vector<string> updateIsbns;
    vector<int> addCounts;

    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
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
        // 插入book实例到instance表
        long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                      isExists[index]);//获取链表的第一个位置

        if (isExists[index] == -1) {//若该种书是首次被添加的
            // 添加到插入信息数组
            Book book(type, count, price, firstInstanceId, name, author, isbn, press);
            newBooks.push_back(book.serialize());
        } else {//若该种书已有实例
            // 添加到更新信息数组
            updateIsbns.push_back(isbn);
            addCounts.push_back(count);
            cout << "count is " << count << endl;
        }
    }
    // 插入books
    vector<ll> ids;
    Book::addBooks(newBooks, ids);
    // 更新图书馆藏量
    Book::updateBooksCount(updateIsbns, addCounts);
    return true;
}

Book::Book(char type, int count, int price, long long int firstInstanceId, const string &name, const string &author,
           const string &isbn, const string &press) : type(type), count(count), price(price),
                                                      firstInstanceId(firstInstanceId), name(name), author(author),
                                                      isbn(isbn), press(press) {}

std::vector<int> Book::checkISBNsExist(std::vector<std::string> isbns) {
    vector<int> results;
    for (int i = 0; i < isbns.size(); ++i) {
        results.push_back(Book::checkAssignISBNExist(isbns[i].data()));
    }
    return results;
}


bool Book::updateBooksCount(std::vector<std::string> isbns, std::vector<int> addCount) {
    for (int i = 0; i < isbns.size(); ++i) {
        Book book;
        book.deSerialize(Book::searchBooksBySingleField("isbn", isbns[i].data())[0]);
        int oldCount = book.count;
        cout << "old count is" << oldCount << endl;
        Book::updateBooks("isbn", isbns[i].data(), "count", to_string(oldCount + addCount[i]));
    }
    return true;
}


void Book::save() {


}



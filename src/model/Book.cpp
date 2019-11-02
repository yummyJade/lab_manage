#include "../../include/model/Book.h"
#include <iostream>
#include <core/SimpleString.h>
#include <fstream>
#include <sstream>
#include <model/BookInstance.h>
#include "../../include/util/DbAdapter.h"
#include "../../include/util/TableRenderer.h"
#include "../../src/core/Input.cpp"
using namespace std;

Book::Book() {
	this->appointmentNum = 0;

}


Book::~Book() {
}

string Book::getTypeContent() {
    string temp;
	char this_type=this->getType() ;
	this_type=toupper(this_type);
    switch (this_type) {
        case 'A':
            temp = "马克思主义、列宁主义、毛泽东思想、邓小平理论";
            break;
        case 'B':
            temp = "哲学、宗教";
            break;
        case 'C':
            temp = "社会科学总论";
            break;
        case 'D':
            temp = "政治、法律";
            break;
        case 'E':
            temp = "军事";
            break;
        case 'F':
            temp = "经济";
            break;
        case 'G':
            temp = "文化、科学、教育、体育";
            break;
        case 'H':
            temp = "语言、文字";
            break;
        case 'I':
            temp = "文学";
            break;
        case 'J':
            temp = "艺术";
            break;
        case 'K':
            temp = "历史、地理";
            break;
        case 'N':
            temp = "自然科学总论";
            break;
        case 'O':
            temp = "数理科学和化学";
            break;
        case 'P':
            temp = "天文学、地球科学";
            break;
        case 'Q':
            temp = "生物科学";
            break;
        case 'R':
            temp = "医药、卫生";
            break;
        case 'S':
            temp = "农业科学";
            break;
        case 'T':
            temp = "工业技术";
            break;
        case 'U':
            temp = "交通运输";
            break;
        case 'V':
            temp = "航空、航天";
            break;
        case 'X':
            temp = "环境科学、安全科学";
            break;
        case 'Z':
            temp = "综合性图书";
            break;
        default:
            temp = "期刊杂志";
            break;
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
	return "";
}

void Book::printBookInfo() {
    this->getTypeContent();
    printf("---------------------------------------------------------\n");
    printf("书名:%s\n作者:%s\n出版社:%s\n类别:%s\nISBN:%s\n预约人数:%d人\n价格:%.2f元\n", name.c_str(), author.c_str(), press.c_str(),
           this->getTypeContent().c_str(), isbn.c_str(), this->getAppointmentNum(),(double)price / 100.0);
    printf("---------------------------------------------------------\n");
//    cout << id << name << author << isbn << type;
}


std::vector<std::string> Book::serialize() {
    vector<string> info;
	
    info.push_back(string(1, this->type));
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
	int id = atoi(info[info.size()-1].data());

    new(this) Book(id,type, count, appointmentNum, price, firstInstanceId, name, author, isbn, press);
//    this->Book::Book();
//    Book(type, count, price, firstInstanceId, name, author, isbn, press);
    return true;
}


std::vector<Book> Book::searchBooksBySingleField(std::string field, std::string value) {
    DbAdapter dbAdapter("Book");
    vector<vector<string> > queryData = dbAdapter.searchBySingleField(field, value);
//    cout<<"执行到这里"<<endl;
    return Book::stringsToBooks(queryData);
}


Book Book::searchBooksById(int id) {
    DbAdapter dbAdapter("Book");
    vector<vector<string> > queryData = dbAdapter.searchBettwenIdA2B(id, id);
    return Book::stringsToBooks(queryData)[0];
}

void Book::printBookList(std::vector<Book> books) {
    vector<string> navs = {"编号", "书名", "作者", "出版社", "类型", "馆藏量", "ISBN"};
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

bool Book::updateBooks(std::string assignField, std::string assignValue, std::string changeField, std::string changeValue) {
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
        Book book;
        book.deSerialize(result[0]);
        return book.getFirstInstanceId();
    }
    return -1;
}

bool Book::importBooksService(string incomingPath="") {
	string path;
	ifstream fin;
	string line;

	if (incomingPath != "") {
		path = SimpleString::fixPath(incomingPath);
		fin = ifstream(path);//打开文件流操作
	}
	else {
		/* 读取一个有效路径,并打开其对应的文件*/
		while (true) {
			path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
			if (path == "")
				return false;
			fin = ifstream(path);//打开文件流操作
			if (fin.good()) {
				cout << "已找到文件,正在读取" << endl;
				break;
			}
			cout << "文件不存在,请检查路径后重新输入" << endl;
		}
	}
    /* 先判断一下这次要导入的书籍种类哪些是已经存在的*/
    getline(fin, line); // 吃掉首行
    vector<string> isbns;
    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
    {
        vector<string> fields;
        try {
            istringstream sin(line);
            string field;
            while (getline(sin, field, ',')) {
                fields.push_back(field);
            }
            if(fields.size()==8){
                isbns.push_back(fields[4]);
            }else{
                isbns.push_back("-2");
            }

        }catch (...){
            isbns.push_back("-2");
            continue;
        }
        

    }
    vector<int> isExists = Book::checkISBNsExist(isbns);

    /* 对不同情况的书籍执行不同操作*/
    int index = 0;//要操作的行下标
    fin.clear();
    fin.seekg(0, ios::beg); // 重新跳转到文件头部
    getline(fin, line); // 吃掉首行
    vector<vector<string>> newBooks; // 要insert到Book表的数据
    vector<string> updateIsbns; // 已经存在的，只需要更新馆藏量的isbn号
    updateIsbns.empty();
    vector<int> addCounts; // 已经存在的，要添加的馆藏量

    while (getline(fin, line)) //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
    {
        try{
            if(isbns[index]=="-2"){ // 该行数据有误
                cout<<"第"<<index+1<<"个图书数据有误,导入失败"<<endl;
				index++;
                continue;
            }

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
            int price = stoi(fields[5]);
//            int price = stof(fields[5]) * 100;
            string position = fields[6];
            int count = stoi(fields[7]);

            vector<BookInstance> bookinstancesFirstAdd;

            for (int i = 0; i < count; ++i) {
                BookInstance bookInstance(isbn, position);
                bookinstancesFirstAdd.push_back(bookInstance);
            }
            // 插入book实例到instance表
//        cout<<"first Id is"<<isExists[index]<<endl;
            long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                          isExists[index]);//获取链表的第一个位置

            if (isExists[index] == -1) {//若该种书是首次被添加的
                Book book(type, count, price, firstInstanceId, name, author, isbn, press);
                if(!book.isLegalBookDate()){
                    cout<<"第"<<index+1<<"个图书数据有误,导入失败"<<endl;
                    index++;
                    continue;
                }
                newBooks.push_back(book.serialize());
            } else {//若该种书已有实例
                // 添加到更新信息数组
//            cout<<"index is "<<index<<" isExi[index] is "<< firstInstanceId<<endl;
                updateIsbns.push_back(isbn);
                addCounts.push_back(count);
            }
        }catch(...){
            cout<<"第"<<index+1<<"个图书数据有误,导入失败"<<endl;
        }
        index++;
    }
    // 插入books
//    cout << "插入操作,插入数量" <<newBooks.size()<< endl;
    vector<ll> ids;
    Book::addBooks(newBooks, ids);
    cout<<"成功导入了"<<newBooks.size()<<"个新的图书信息"<<endl;
    // 更新图书的馆藏量
//    cout << "update操作,update数量"<<updateIsbns.size() << endl;
	if (updateIsbns.size() > 0) {
		Book::updateBooksCount(updateIsbns, addCounts);
        cout<<""<<updateIsbns.size()<<"个的图书的ISBN已存在,已更新其数量"<<endl;
	}

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

//只有两种情况 +1 -1
bool Book::updateBooksAppointmentNum(std::string isbn, int addAppointmentNum){
    if(abs(addAppointmentNum) == 1) {
        int oldAppointmentNum = Book::searchBooksBySingleField("isbn",isbn.data())[0].getAppointmentNum();
        Book::updateBooks("isbn", isbn, "appointmentNum", to_string(oldAppointmentNum + addAppointmentNum));
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
//    cout<<"book 的size是"<<books.size()<<endl;
    for (int i = 0; i < books.size(); ++i) {
        Book book;
        book.deSerialize(books[i]);
        results.push_back(book);
    }
    return results;
}

bool Book::addOneBookService() {
	//预约测试用书1 谭坚铭 谭谭出版社 c 912-654-51-2 6021 南区宿舍 1
	system("cls");
	cout<<"--------------------添加书籍--------------------------"<<endl;
//    printf("请依次输入 书名,作者,出版社,类型(单个字符),isbn,价格(单位:元),位置,有效数量,用空格隔开\n");
//    vector<string> fields;
//    int index = 0;
//    string temp_info;
//    while (index < 8) {
//        cin >> temp_info;
//        fields.push_back(temp_info);
//        index++;
//    }
    Book tempBook;
    BookInstance tempInstance("","");
	cout << "书名输入0取消添加" << endl;
	string name = tempBook.readAndSetName();
	if (name == "0")
		return false;
    tempBook.readAndSetAuthor();
    tempBook.readAndSetPress();
    tempBook.readAndSetType();
    string isbn=tempBook.readAndSetIsbn();
    tempBook.readAndSetPrice();
    string position=tempInstance.readAndSetPosition();
    int count=tempBook.readAndSetCount();

//    string name = fields[0];
//    string author = fields[1];
//    string press = fields[2];
//    char type = fields[3][0];
//    string isbn = fields[4];
//    int price = stof(fields[5]) * 100;
//    string position = fields[6];
//    int count = stoi(fields[7]);


    vector<BookInstance> bookinstancesFirstAdd;
    for (int i = 0; i < count; ++i) {
        BookInstance bookInstance(isbn, position);
        bookinstancesFirstAdd.push_back(bookInstance);
    }

    // 插入book实例到instance表
    int firstId = Book::checkAssignISBNExist(isbn);
    long long firstInstanceId = BookInstance::importBookInstances(bookinstancesFirstAdd,
                                                                  firstId);//获取链表的第一个位置

    if (firstId == -1) {// 如果该书不存在
        vector<vector<string>> books;
        //Book book(type, count, price, firstInstanceId, name, author, isbn, press);
		tempBook.setFirstInstanceId(firstInstanceId);
		tempBook.setAppointmentNum(0);
        books.push_back(tempBook.serialize());
        vector<long long int> ids;
        Book::addBooks(books, ids);
		cout << "图书插入成功" << endl;
    } else {
        vector<int> counts;
        counts.push_back(count);
        vector<string> isbns;
        isbns.push_back(isbn);
        Book::updateBooksCount(isbns, counts);
		cout << "isbn为" << isbn << "的书籍已存在,成功增加了" << count << "本实例" << endl;
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

Book::Book(int id,char type, int count, int appointmentNum, int price, int firstInstanceId, const string& name,
	const string& author, const string& isbn, const string& press) :id(id), type(type), count(count),
	appointmentNum(appointmentNum),
	price(price),
	firstInstanceId(firstInstanceId),
	name(name), author(author), isbn(isbn),
	press(press) {
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

char Book::readAndSetType() {
    char result;
    while(true){
        cout<<"请输入类型(单个a-z之间的字符):";
        result=Input::getChar();
        this->setType(result);
        if ((result>='a' && result<='z')||(result>='A' && result<='Z')){
            return result;
        }
    }
}

int Book::readAndSetCount() {
    cout<<"请输入馆藏量:";
    int result=Input::getInt();
    this->setCount(result);
    return result;
}

int Book::readAndSetPrice() {
    cout<<"请输入价格(单位:分):";
    int result=Input::getInt();
    this->setPrice(result);
    return result;
}

std::string Book::readAndSetName() {
    cout<<"请输入书名:";
    string result=Input::getAssignMaxLengthStr(50);
    this->setName(result);
    return result;
}

std::string Book::readAndSetAuthor() {
    cout<<"请输入作者:";
    string result=Input::getAssignMaxLengthStr(20);
    this->setAuthor(result);
    return result;
}

std::string Book::readAndSetIsbn() {
    cout<<"请输入ISBN:";
    string result=Input::getAssignMaxLengthStr(20);
    this->setIsbn(result);
    return result;
}

std::string Book::readAndSetPress() {
    cout<<"请输入出版社:";
    string result=Input::getAssignMaxLengthStr(50);
    this->setPress(result);
    return result;
}

bool Book::isLegalBookDate() {
    if(this->getName().length()>50 || this->getAuthor().length()>20 || this->getIsbn().length()>20||this->getPress().length()>50){
        return false;
    }
    return true;
}



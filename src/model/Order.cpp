#include "../../include/model/Order.h"
#include "../../linkDatebase/include/Record.h"
#include "../../linkDatebase/include/TableRecord.h"
#include <vector>
#include <iostream>
#include <string>
#include <util/TableRenderer.h>
#include <model/BookInstance.h>
#include <model/Book.h>


using namespace std;
Order::Order()
{
}


Order::~Order()
{
}

std::vector<std::string> Order::serialize() {
    std::vector<std::string> info;
//    info.push_back(to_string(this->id));
    info.push_back(std::to_string(this->bookId));
    info.push_back(this->borrowTime.serialize());
    info.push_back(this->returnTime.serialize());

    //todo: ö�ٵ����л��ͷ����л�
//    info.push_back(this->statu);
    return info;
}

bool Order::deSerialize(std::vector<std::string> info) {
    //todo: ����Ҫ�����쳣����

//    int IsRenew=stoi(info[0]);
//    int bookId=stoi(info[1]);
//    int borrowTime=stoi(info[2]);
//    int returnTime=stoi(info[3]);
//    status type;
//    type=Teacher;// todo:���ö��Ҫ��ô����
//    string password=info[4];
//
//    Order(id,jobNum,borrowNum,type,password);
    return true;
}

std::string Order::statuEnumToString(Status statu) {
    std::string strs[] = {"BORROWING", "RETURNED"};
    return strs[statu];
}

Status Order::stringEnumToStatu(std::string str) {
    for (int i = 0; i < str.size(); ++i) { // ת��Ϊ��д
        str[i] = toupper(str[i]);
    }
    std::string strs[] = {"BORROWING", "RETURNED"};
    for (int i = 0; i < sizeof(strs) / sizeof(strs[0]); i++) {
        if (str == strs[i]) {
            return Status(i);
        }
    }
    return BORROWING;//û�ҵ�Ĭ�Ϸ��ص�,//todo:��������ôд
}

Order::Order(long long userId, int bookId, const SimpleTime &borrowTime, const SimpleTime &returnTime,
             Status statu) : userId(
        userId), bookId(bookId), borrowTime(borrowTime), returnTime(returnTime), statu(statu) {}


//1�����ڽ裬2�����ѻ���3����ԤԼ��4������������ڽ裬5����ԤԼ�ѵ�
std::vector<Order> Order::getAssignUserBorrowedHistory(int firstOrderId) {
    TableRecord *table = TableRecord::getInstance();
    vector<Record> copys = table->queryByPerson(firstOrderId);
    vector<Order> result;
//    cout<<"copys size "<<copys.size()<<endl;
    for (int i = 0; i < copys.size(); ++i) {
//        cout<<"��ʼת��"<<endl;
		Order order = Order::RecordCopyToOrder(copys[i]);
		// cout << "ת�����ʱ��" << order.borrowTime.serialize()<<endl;
		result.push_back(order);
		//result.push_back(Order::RecordCopyToOrder(copys[i]));

//        cout<<"ת���ɹ�"<<endl;
    }
//    cout<<"copys end "<<endl;
    return result;
}


std::vector<Order> Order::getAssignUserBorrowingList(int firstOrderId) {
    // ��ȡ���н��ļ�¼
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    cout << "�ѻ�ȡ�����м�¼:" << orders.size() << endl;
    vector<Order> result;
    int borringIndexs[] = {1, 4}; // �ڽ�״̬statu��ֵ����
    for (int i = 0; i < orders.size(); ++i) {
        for (int j = 0; j < sizeof(borringIndexs) / sizeof(int); ++j) {
            if (orders[i].statu == borringIndexs[j]) {
                result.push_back(orders[i]);
                break;
            }
        }
    }
    cout << "result size " << result.size() << endl;
    return result;
}


std::vector<Order> Order::getAssignUserOweOrder(int firstOrderId) {
    // �Ȼ�ȡ�����ڽ��¼
    vector<Order> orders = Order::getAssignUserBorrowingList(firstOrderId);
    vector<Order> result;
    for (int i = 0; i < orders.size(); ++i) {
        // ���Ӧ��ʱ��С�ڽ���,��������
        if (orders[i].returnTime.compare(Date::today()) <= -1) {
            result.push_back(orders[i]);
        }
    }
    return result;
}

int Order::addOneOrder(Order order, int firstId) {
//    TableRecord* table =TableRecord::getInstance();
//    Record record;
//    table->insertData(firstId,record);
    return 0;
}

Record Order::toRecordCopy() {
    Record record;
    record.setId(this->id);
    record.setBookId(this->bookId);
    record.setState(this->statu);
    record.setStId(this->userId);
    record.setBoTime(this->borrowTime.toLLTime());
    //record.setReTime(this->returnTime.toLLTime());
	record.setReTime(this->returnTime.date.toInt());
    return record;
}


Order Order::RecordCopyToOrder(Record record) {
    SimpleTime boTime = SimpleTime::llTimeToSimpleTime(record.getBoTime());
	Date reDate = Date::intDate2Date(record.getReTime());
	//cout << "����ʱ����" << record.getBoTime() << endl;
	//cout << "����ʱ��2��" << boTime.serialize() << endl;
	SimpleTime reTime = SimpleTime(0,0,0,reDate);
    //SimpleTime reTime = SimpleTime::llTimeToSimpleTime(record.getReTime());
    return Order(record.getId(), record.getStId(), record.getBookId(), boTime, reTime,
                 static_cast<Status>(record.getState()));
}

Order::Order(long long int id, long long int userId, int bookId, const SimpleTime &borrowTime,
             const SimpleTime &returnTime, Status statu) : id(id), userId(userId), bookId(bookId),
                                                           borrowTime(borrowTime), returnTime(returnTime),
                                                           statu(statu) {}

int Order::addSingleOrder(int firstId, Order order) {
    TableRecord *table = TableRecord::getInstance();
    return table->insertData(firstId, order.toRecordCopy());
}

void Order::printOrderList(std::vector<Order> orders) {
    vector<string> navs = {"����", "�����", "����ʱ��", "����/Ӧ��ʱ��", "״̬"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < orders.size(); ++i) {
        render.addColume(orders[i].getPrintLineStr());
    }
    render.render();
}

std::vector<std::string> Order::getPrintLineStr() {
    vector<string> info;
    BookInstance *bookInstance = BookInstance::getInstanceById(this->getBookId());
//    cout<<"isbn:"<<bookInstance->getIsbn()<<"|||";
    Book book = Book::searchBooksBySingleField("isbn", bookInstance->getIsbn())[0];
    info.push_back(book.getName());
    info.push_back(to_string(bookInstance->getId()));

    SimpleTime date = this->getBorrowTime();

    info.push_back(date.serialize());
    date = this->getReturnTime();
    info.push_back(date.date.serialize());
	info.push_back(this->getStatuStr());
    //info.push_back(to_string(this->getStatu()));//todo:״̬�ĳɶ�Ӧ����
    return info;
}

long long int Order::getId() const {
    return id;
}

long long int Order::getUserId() const {
    return userId;
}

int Order::getBookId() const {
    return bookId;
}

const SimpleTime &Order::getBorrowTime() const {
    return borrowTime;
}

const SimpleTime &Order::getReturnTime() const {
    return returnTime;
}

Status Order::getStatu() const {
    return statu;
}

std::string Order::getStatuStr() {
	string strs[] = { "", "�ڽ�", "�ѻ�","ԤԼ", "������","ԤԼ�ѵ�" };
	return strs[this->getStatu()];
}

void Order::setId(long long int id) {
    Order::id = id;
}

void Order::setUserId(long long int userId) {
    Order::userId = userId;
}

void Order::setBookId(int bookId) {
    Order::bookId = bookId;
}

void Order::setBorrowTime(const SimpleTime &borrowTime) {
    Order::borrowTime = borrowTime;
}

void Order::setReturnTime(const SimpleTime &returnTime) {
    Order::returnTime = returnTime;
}

void Order::setStatu(Status statu) {
    Order::statu = statu;
}

bool Order::updateStateAndReturnTimeById(Order order) {
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {6, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}



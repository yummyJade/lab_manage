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

    return info;
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


std::vector<Order> Order::getAssignBookBorrowedHistory(int bookId) {
    TableRecord *table = TableRecord::getInstance();
    vector<Record> copys = table->queryByBookId(bookId);
    vector<Order> result;
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}

std::vector<Order> Order::getAssignBookAppointingList(int bookId) {
    // �Ȼ�ȡ���еĽ��ļ�¼
    vector<Order> orders = Order::getAssignBookBorrowedHistory(bookId);
//    cout << "�ѻ�ȡ�����м�¼:" << orders.size() << endl;
    vector<Order> result;
    int appointmentIndex = 3;       //��ʾ���е���ԤԼ״̬
    for (int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentIndex){
            result.push_back(orders[i]);
        }
    }
    return result;
}

std::vector<Order> Order::getAssignBookOweAppointing(int bookId) {
    //�Ȼ�ȡ���е���ԤԼ��¼
    vector<Order> orders = Order::getAssignBookAppointingList(bookId);

	return orders;
}

//1�����ڽ裬2�����ѻ���3����ԤԼ��4������������ڽ裬5����ԤԼ�ѵ�
std::vector<Order> Order::getAssignUserBorrowedHistory(int firstOrderId) {
    TableRecord *table = TableRecord::getInstance();
	//cout << "firstID is" << firstOrderId << endcoul;
    vector<Record> copys = table->queryByPerson(firstOrderId);
    vector<Order> result ;
    for (int i = 0; i < copys.size(); ++i) {
        result.push_back(Order::RecordCopyToOrder(copys[i]));
    }
    return result;
}


std::vector<Order> Order::getAssignUserCompleteOrders(int firstOrderId) {
	// ��ȡ���н��ļ�¼
	vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
	vector<Order> result;
	int borringIndexs[] = { 2 }; // �Ѿ���
	for (int i = 0; i < orders.size(); ++i) {
		for (int j = 0; j < sizeof(borringIndexs) / sizeof(int); ++j) {
			if (orders[i].statu == borringIndexs[j]) {
				result.push_back(orders[i]);
				break;
			}
		}
	}
	//cout << "result size " << result.size() << endl;
	return result;
}

std::vector<Order> Order::getAssignUserBorrowingList(int firstOrderId) {
    // ��ȡ���н��ļ�¼
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    //cout << "�ѻ�ȡ�����м�¼:" << orders.size() << endl;
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
    return result;
}


std::vector<Order> Order::getAssignUserAppointmentList(int firstOrderId){
    //��ȡ���е���ԤԼ״̬
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    vector<Order> result ;
//    cout << orders.size() << endl;
    int appointmentIndexs[] = {3, 5};      //��ʾ������ԤԼ��״̬��������ԤԼ�Լ�ԤԼ�ѵ�δȡ

    for(int i = 0; i < orders.size(); ++i) {
        for(int j = 0; j < sizeof(appointmentIndexs) / sizeof(int); ++j) {
            if(orders[i].statu == appointmentIndexs[j]) {
                result.push_back(orders[i]);
                break;
            }
        }
    }
    //���Ϊ�յĴ���
//    cout << result.size() << endl;
    return result;
}


std::vector<Order> Order::getAssignUserArrivedAppointmentList(int firstOrderId) {
    //�Ȼ�ȡ���е���ԤԼ״̬
    vector<Order> orders = Order::getAssignUserBorrowedHistory(firstOrderId);
    vector<Order> result;
    //todo:�ж�ԤԼ�Ƿ���ڣ�����ǣ��޸ĸö���Ϊʲô״̬��
    int appointmentIndex = 5;      //��ʾ����ԤԼ�ѵ�δȡ
    SimpleTime appointArrivedInValidTime;
    for(int i = 0; i < orders.size(); ++i) {
        if(orders[i].statu == appointmentIndex) {
//            result.push_back(orders[i]);
            //�жϸö����ĵ���ʱ���뵱ǰʱ��Ĺ�ϵ��������ʱ����ڵ��ڵ�ǰʱ�䣬����δ���ڣ����ؽ��
            appointArrivedInValidTime = orders[i].getBorrowTime();       //ԤԼʧЧʱ��
            if(appointArrivedInValidTime.compare(SimpleTime::nowTime()) >= 0){
                result.push_back(orders[i]);
            }else{
                //todo������ط��Ĵ���Ҫ����
                orders[i].setStatu(static_cast<Status>(6));     //�������޸ĳ�ԤԼ���ڵ�״̬
                orders[i].updateStateAndReturnTimeById(orders[i]);      //�־û�
                BookInstance *instance = BookInstance::getInstanceById(orders[i].getBookId());
                //�޸��鼮ʵ����״̬Ϊ�ɽ�
                instance->setStatus(1);
                //���º������Ĳ���
                BookInstance::updateStateAndReturnTimeById(*instance);
            }

        }

    }
    //cout << "result size " << result.size() << endl;
    return result;
}

bool Order::judgeAssignUserAppointmentList(int firstOrderId, int isbn) {
    //��ȡ���е���ԤԼ״̬
	return true;
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
	SimpleTime reTime = SimpleTime(0, 0, 0, reDate);

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
    vector<string> navs = {"���","����", "�����", "����ʱ��", "����/Ӧ��ʱ��", "״̬"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < orders.size(); ++i) {
		vector<string> line;
		vector<string> temp = orders[i].getPrintLineStr();
		line.push_back(to_string(i + 1));
		line.insert(line.end(), temp.begin(), temp.end());
        render.addColume(line);
    }
    render.render();
}

std::vector<std::string> Order::getPrintLineStr() {
    vector<string> info;
    Book book;
    BookInstance *bookInstance = BookInstance::getInstanceById(this->getBookId());
	string code;
    if(this->getStatu()==3){// �����ԤԼ�Ļ�
        book = Book::searchBooksById(this->getBookId());
		code = "��";
    }else{
        book = Book::searchBooksBySingleField("isbn", bookInstance->getIsbn())[0];
		code = to_string(bookInstance->getId());
    }
	

    info.push_back(book.getName());// ����
    info.push_back(code); //�����
    SimpleTime date =  this->getBorrowTime();
    info.push_back(date.serialize());
    date = this->getReturnTime();
    info.push_back(date.date.serialize());
	info.push_back(this->getStatuStr());

    return info;
}

std::string Order::getStatuStr() {
	string strs[] = { "", "�ڽ�", "�ѻ�","ԤԼ��", "�ڽ�(������)","ԤԼ�ѵ�" };
	return strs[this->getStatu()];
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

bool Order::updateStateAndBookIdAndBorrowTimeById(Order order){
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {2, 5, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}

bool Order::updateStateAndReturnTimeAndLendTimeById(Order order) {
    TableRecord *table = TableRecord::getInstance();
    vector<int> changeIndex = {2, 6, 7};
    table->update(order.getId(), order.toRecordCopy(), changeIndex);
    return true;
}


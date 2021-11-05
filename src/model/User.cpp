#include "../../include/model/User.h"
#include "../../include/model/Book.h"
#include "../../include/model/BookInstance.h"
#include "../../include/core/SimpleString.h"
#include "../../include/core/MD5.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>
#include <util/TableRenderer.h>
#include <core/Input.h>
using namespace std;

const int User::lendDays[] = {0, 90, 60, 30, 0};
const int User::lendNums[] = {0, 90, 60, 30, 0};
const int User::appointNums[] = {0, 9, 6, 3, 0};
const int User::appointDays[] = {0, 60, 45, 30, 0};

User::User()
{
}

User::~User() {
}

vector<string> User::serialize() {
    vector<string> info;
    info.push_back(to_string(this->jobNum));
    info.push_back(to_string(this->type));
    info.push_back(this->name);
    info.push_back(this->password);
    info.push_back(to_string(this->firstOrderId));
    return info;
}

bool User::deSerialize(vector<string> info) {
    long long jobNum = stoll(info[0]);
    status type = static_cast<status>(stoi(info[1]));

    string name = info[2];
    string password = info[3];
    int firstOrderId = stoi(info[4]);
    new(this) User(jobNum, type, name, password, firstOrderId);
    return true;
}

std::string User::statuEnumToString(status statu) {
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    return strs[statu];
}

status User::stringEnumToStatu(std::string str) {
    for (int i = 0; i < str.size(); ++i) { // ת��Ϊ��д
        str[i] = toupper(str[i]);
    }
    string strs[4] = {"ADMIN", "TEACHER", "GRADUATE", "UNDERGRADUATE"};
    for (int i = 0; i < sizeof(strs) / sizeof(strs[0]); i++) {
        if (str == strs[i]) {
            return status(i);
        }
    }
    return Undergraduate;//û�ҵ�Ĭ�Ϸ��ص�
}

User::User(long long jobNum, status type, const string &name, const string &password) : jobNum(jobNum), type(type),
                                                                                        name(name), password(password) {
    this->firstOrderId = -1;
}

bool User::setPassword(const string &password) {
    this->password = calcMD5(password);
    User::updateUsersAssignField("jobNum", to_string(this->getJobNum()), "password", this->getPassword());
	return true;
}

bool User::isLegalPassword(const std::string &password) {
    return (calcMD5(password) == this->password);
}

bool User::changePwdService() {
    string pwd;
    // ����ԭ��������
    do {
        cout << "������ԭ����(����0ȡ���޸�):";
        pwd=Input::getAssignMaxLengthStr(20);
        if (pwd == "0") {
            return false;
        }
        if (this->isLegalPassword(pwd)) {
            break;
        } else {
            cout << "ԭ�������" << endl;
        }
    } while (true);

    cout << "������������:";
    pwd=Input::getAssignMaxLengthStr(20);
    this->setPassword(pwd);

    cout << "�޸ĳɹ�";

    return true;
}

int User::login(long long jobNum, std::string password, User *user = NULL) {

    if (checkUserExist(jobNum, user)) {
        if (user->isLegalPassword(password)) {// �ж��û����Ƿ���ȷ
            if (user->getType() < 0)// �ж��û�״̬,�Ƿ񱻽�ֹ��½
                return 3;// ����3,�û�����ֹ��½(��ʧ\����)

            // �ж��û����ж�����״̬,�Ƿ��г�ʱ����
            if (!Order::getAssignUserOweOrder(user->getFirstOrderId()).empty()) {
                cout << "���û������ڽ���,��Ҫ����Ա�������ܵ�½" << endl;
//                cout << "����Y��������,Nȡ����¼:";
//                char operate;
//                operate=Input::getChar();
//                if (operate == 'Y' || operate == 'y') {
//                    // �����������
//                    if (user->dealWithOverTimeOrder() == 0) {
//                        return 0; // ��½�ɹ�
//                    }
//                }
                return 4;// ����4,�����ڽ���,��ֹ��½
            }
            return 0; // ����0,��½�ɹ�
        } else {
            return 2;//����2,�������
        }
    } else {
        return 1;// ����1,�û�������
    }

}

bool User::checkUserExist(long long jobNum, User *user = NULL) {
    DbAdapter db("User");
    vector<vector<string>> results = db.searchBySingleField("jobNum", to_string(jobNum));

    if (results.size() > 0) {
		if (user != NULL) {
			*user = User(User::stringsToUsers(results)[0]);
		}

		//cout << "password is" << user->getPassword()<<endl;
        return true;
    }
    return false;
}

std::vector<Order> User::getBorrowedHistory() {
    return Order::getAssignUserBorrowedHistory(this->jobNum);
}

std::vector<Order> User::getBorrowingList() {
    return Order::getAssignUserBorrowingList(this->jobNum);
}

bool User::addUsers(std::vector<std::vector<std::string>> queryData, std::vector<long long> &ids) {
    DbAdapter dbAdapter("User");
    dbAdapter.insert(queryData, ids);
    return true;
}

int User::isAllowedLogin() {
    // �ж��û�״̬,�Ƿ񱻽�ֹ��½
    if (this->type < 0)
        return 1;
    // �ж��û����ж�����״̬,�Ƿ��г�ʱ����
    if (!Order::getAssignUserOweOrder(this->firstOrderId).empty())
        return 2;
    return 0;
}

int User::borrowAssignBookInstance(int bookInstanceId) {
    // �ж��û��ܷ����(�Ƿ���Ȩ����,���������Ƕ���)
    /*cout << "firstOrder is " << this->getFirstOrderId();
    cout << "�û�������" << Order::getAssignUserBorrowingList(this->getFirstOrderId()).size() << endl;
    cout << "�û��ɽ�����" << this->getCanLendNums() << endl;*/

    if (Order::getAssignUserBorrowingList(this->firstOrderId).size() >= this->getCanLendNums()) {
		cout << "����ʧ��,���������Ѵ�����" << endl;
        return 1; // ����1,���������Ѵ�����
    }

    // �жϸ����Ƿ��ܱ�����,
    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);
    if (instance == NULL) {
		cout << "����ʧ��,��ͼ�鲻����"<<endl;
        return 5; // ����5,ͼ�鲻����
    } else if (instance->status != 1) {
		cout << "����ʧ��,��ͼ�鵱ǰ���ɱ���" << endl;
        return 2; // ����2,���鲻�ǿɽ�
    }

    // ����һ�����ļ�¼Order,��Ҫ(�û�����,��ʵ��id,����ʱ��,Ԥ�ƹ黹ʱ��,����״̬)
    Order order(this->getJobNum(), bookInstanceId, SimpleTime::nowTime(),
                SimpleTime::nowTime().addDay(this->getCanLendDays()),
                static_cast<Status>(1));

    int orderId = Order::addSingleOrder(this->getFirstOrderId(), order);

    // ���ø�Bookinstance���ɽ�,������Ӧ��ʱ��
    instance->setStatus(2);
    instance->setPlanReturnDate(Date::today().addDay(this->getCanLendDays()));
    BookInstance::updateStateAndReturnTimeById(*instance);

    // �ж��Ƿ��״ν���,�ǵĻ����½�������ͷ���ֶ�
    if (this->getFirstOrderId() == -1) {
		this->setFirstOrderId(orderId);
        User::updateUsersAssignField("jobNum", to_string(this->getJobNum()), "firstOrderId", to_string(orderId));
    }
	cout << "����ɹ�" << endl;
    return 0;
}

//bool User::importUsers(string incomingPath="") {
//    string path;
//    ifstream fin;
//    string line;
//    if (incomingPath != "") {
//        path = SimpleString::fixPath(incomingPath);
//        fin = ifstream(path);//���ļ�������
//    }
//    else {
//        /* ��ȡһ����Ч·��,�������Ӧ���ļ�*/
//        while (true) {
//            path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
//            if (path == "")
//                return false;
//            fin = ifstream(path);//���ļ�������
//            if (fin.good()) {
//                cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
//                break;
//            }
//            cout << "�ļ�������,����·������������" << endl;
//        }
//    }
//
//    int index = 0;//Ҫ���������±�
//    fin.clear();
//    fin.seekg(0, ios::beg); // ������ת���ļ�ͷ��
//    getline(fin, line); // �Ե�����
//    vector<vector<string>> users; // Ҫinsert��User�������
//    vector<long long> existUsers; //�Ѿ����ڵ��û�����
//
//    while (getline(fin, line)) //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
//    {
//        try{
//            istringstream sin(line);
//            vector<string> fields;
//            string field;
//
//            while (getline(sin, field, ',')) {
//                fields.push_back(field);
//            }
//            if(fields.size()!=4){
//                cout<<"��"<<index+1<<"���û���������,����ʧ��"<<endl;
//                index++;
//                continue;
//            }
//
//            long long jobNum = stoll(fields[0]);
//            string name = fields[1];
//            string pwd = fields[2];
//            int state = stoi(fields[3]);
//
//            if (User::checkUserExist(jobNum)) {
//                existUsers.push_back(jobNum);
//            } else {
//                pwd = User::encryPassword(pwd);
//                User user(jobNum, static_cast<status>(state), name, pwd);
//                if(!user.isLegalUserDate()){
//                    cout<<"��"<<index+1<<"���û���������,����ʧ��"<<endl;
//                    index++;
//                    continue;
//                }
//                users.push_back(user.serialize());
//            }
//        }catch (...){
//            cout<<"��"<<index+1<<"���û���������,����ʧ��"<<endl;
//        }
//        index++;
//
//    }
//
//    vector<long long> ids;
//    User::addUsers(users, ids);
//    cout<<"---------------------------------------"<<endl;
//    cout<<"�ɹ�������"<<users.size()<<"�����û�"<<endl;
//    if (!existUsers.empty()) {
//        cout << "���¹��ŵ��û��Ѿ�����" << endl;
//        cout<<"---------------------------------------"<<endl;
//        for (int i = 0; i < existUsers.size(); ++i) {
//            cout << existUsers[i] << endl;
//        }
//        cout<<"---------------------------------------"<<endl;
//    }
//
//    return true;
//}

bool User::importUsers(string incomingPath="") {
	string path;
	ifstream fin;
	string line;
	if (incomingPath != "") {
		path = SimpleString::fixPath(incomingPath);
		fin = ifstream(path);//���ļ�������
	}
	else {
		/* ��ȡһ����Ч·��,�������Ӧ���ļ�*/
		while (true) {
			path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
			if (path == "")
				return false;
			fin = ifstream(path);//���ļ�������
			if (fin.good()) {
				cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
				break;
			}
			cout << "�ļ�������,����·������������" << endl;
		}
	}

    int index = 0;//Ҫ���������±�
    int allNum=0;
    fin.clear();
    fin.seekg(0, ios::beg); // ������ת���ļ�ͷ��
    getline(fin, line); // �Ե�����

    vector<long long> existUsers; //�Ѿ����ڵ��û�����

    while(1){
        vector<vector<string>> users; // Ҫinsert��User�������
        while (index<100 && getline(fin, line)) //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
        {
            try{
                istringstream sin(line);
                vector<string> fields;
                string field;

                while (getline(sin, field, ',')) {
                    fields.push_back(field);
                }
                if(fields.size()!=4){
                    cout<<"��"<<index+1<<"���û���������,����ʧ��"<<endl;
                    index++;
                    continue;
                }

                long long jobNum = stoll(fields[0]);
                string name = fields[1];
                string pwd = fields[2];
                int state = stoi(fields[3]);

                if (User::checkUserExist(jobNum)) {
                    existUsers.push_back(jobNum);
                } else {
                    pwd = User::encryPassword(pwd);
                    User user(jobNum, static_cast<status>(state), name, pwd);
                    if(!user.isLegalUserDate()){
                        cout<<"��"<<index+1<<"���û���������,����ʧ��"<<endl;
                        index++;
                        continue;
                    }
                    users.push_back(user.serialize());
                }
            }catch (...){
                cout<<"��"<<index+1<<"���û���������,����ʧ��"<<endl;
            }
            index++;

        }

        vector<long long> ids;
        User::addUsers(users, ids);
        allNum+=index;
        cout<<"������"<<allNum<<"��"<<endl;
        if(index==100){
            index=0;
        }else{
            break;
        }

    }

    cout<<"---------------------------------------"<<endl;
    cout<<"�ɹ�������"<<allNum<<"�����û�"<<endl;
	if (!existUsers.empty()) {
		cout << "���¹��ŵ��û��Ѿ�����" << endl;
		cout<<"---------------------------------------"<<endl;
		for (int i = 0; i < existUsers.size(); ++i) {
			cout << existUsers[i] << endl;
		}
        cout<<"---------------------------------------"<<endl;
	}

    return true;
}


std::string User::encryPassword(std::string pwd) {
    return calcMD5(pwd);
}

User::User(long long int jobNum, status type, const string &name, const string &password, int firstOrderId)
        : jobNum(jobNum), type(type), name(name), password(password), firstOrderId(firstOrderId) {}


bool User::getUserMessage() {
    bool hasMessage=false;

    // ����Ƿ���ԤԼ�ѵ����鼮
    vector<Order> AppointingOrders = Order::getAssignUserArrivedAppointmentList(this->getFirstOrderId());
    if(!AppointingOrders.empty()){//��ԤԼ�ѵ�����
        if(!hasMessage){
            printf("--------------------------------------------------------\n");
            hasMessage= true;
        }
        cout<<"����"<<AppointingOrders.size()<<"��ԤԼ���鼮�ѵ�,�뼰ʱ��ͼ�����ȡ"<<endl;
        Order::printOrderList(AppointingOrders);
    }

    // ����Ƿ��м������ڵĽ���
    vector<Order> borrowingOrders = Order::getAssignUserBorrowingList(this->getFirstOrderId());
    vector<Order> soonTimeOutOrders;
    for (int i = 0; i < borrowingOrders.size(); ++i) {
        SimpleTime returnTime = borrowingOrders[i].getReturnTime();
        if (returnTime.compare(Date::today().addDay(3)) <= 0) {// 3���ڵ��ڵ���
            soonTimeOutOrders.push_back(borrowingOrders[i]);
        }
    }
    if(!soonTimeOutOrders.empty()){//��ԤԼ�ѵ�����
        cout<<"����"<<soonTimeOutOrders.size()<<"�����ĵ��鼮����3���ڵ���,��ע�⼰ʱ�黹"<<endl;
        Order::printOrderList(soonTimeOutOrders);
    }
    if(hasMessage){
        printf("--------------------------------------------------------\n");
    }
    return true;
}


bool User::appointmentAssignBook(int bookId, std::string isbn) {
    // �ж��û��Ƿ񳬹����ԤԼ��
    if(Order::getAssignUserAppointmentList(this->firstOrderId).size() >= this->getCanAppointNums()){
        cout << "�������ԤԼ��" << endl;
        return false;
    }
    //�ж��Ƿ񳬹��ɽ�����������ɽ�����+��ԤԼ��������
    if((Order::getAssignUserBorrowingList(this->firstOrderId).size() + Order::getAssignUserAppointmentList(this->firstOrderId).size()) >= this->getCanLendNums()){
        cout << "�������ɽ���" << endl;
        return false;
    }
    // todo:�жϸ��û��Ƿ��Ѿ������˻���ԤԼ���Ȿ��
    vector<Order> AppointmentList= Order::getAssignUserAppointmentList(this->getFirstOrderId());
    for (int i = 0; i <AppointmentList.size() ; ++i) {
        //������û�����ԤԼ�Ȿ��
        if(( AppointmentList[i].getBookId()==bookId)){
            cout<<"ԤԼʧ��,���û���ǰ��ԤԼ����,�����ظ�ԤԼ"<<endl;
            return false;
        }
       //�ҵ����е�ͼ��ʵ�� ��ȷ������ԤԼ�ѵ�����������û����鼮
       //�ҵ����е�ԤԼ�ѵ����ж��Ƿ�Ϊ���ڸñ����ʵ��
       if( AppointmentList[i].getStatu() == 5) {
           BookInstance* tempInstance=BookInstance::getInstanceById(AppointmentList[i].getBookId());
           if(tempInstance->getIsbn() == isbn) {
               cout << "ԤԼʧ�ܣ����û���ǰ��δ��ȡ�ĸñ��飬������ԤԼ" << endl;
               return false;
           }
       }


    }

    vector<Order> BorrowingList= Order::getAssignUserBorrowingList(this->getFirstOrderId());
    for (int j = 0; j < BorrowingList.size(); ++j) {
        BookInstance* tempInstance=BookInstance::getInstanceById(BorrowingList[j].getBookId());
        if(tempInstance->getIsbn()==isbn){
            cout<<"ԤԼʧ��,���û���ǰ���ڽ��ĸ���,������ԤԼ"<<endl;
            return false;
        }
    }


    // �жϸ����Ƿ�ɱ�ԤԼ(�Ƿ�û��״̬Ϊ�ɽ����û���¼�) ��bookid��bookinstance
    if(!BookInstance::checkAssignBookCanAppointmentInstanceExist(isbn)){
        cout << "ԤԼʧ��,���ڵ�ǰ�пɽ�ͼ��!" << endl;
        return false;
    }


    // ����ԤԼOrder���û��ţ��鱾Ψһ��ʶisbn(xbookId)����ǰԤԼʱ�䣬״̬Ϊ���ģ�
//    cout << "bookId" << bookId << endl;
    Order order(this->getJobNum(), bookId, SimpleTime::nowTime(), SimpleTime::nowTime(), static_cast<Status>(3));
    //�־û�

    int orderId = Order::addSingleOrder(this->getFirstOrderId(), order);

    // ����Book����ԤԼ������Ϊ���ӣ���ʵ���������û�ã������Ҳ�֪����ô����book�־û������ݿ⣬updateô

    Book::updateBooksAppointmentNum(isbn,1);

    // �ж��Ƿ��״ν���,�ǵĻ����½�������ͷ���ֶΣ������Ȼ�ͽ�������ⶼ��һ����
    if (this->getFirstOrderId() == -1) {
        this->setFirstOrderId(orderId);
        User::updateUsersAssignField("jobNum", to_string(this->getJobNum()), "firstOrderId", to_string(orderId));
    }

    cout << "ԤԼ�ɹ�����ȴ�����֪ͨ" << endl;
    return true;
}

//bool User::getArrivedAppointment(){
//    //todo:�û���ȡԤԼ�ѵ����鼮
//
//}



std::vector<std::string> User::getPrintLineStr() {
    vector<string> info;
    info.push_back(to_string(this->jobNum));
    info.push_back(this->name);
	info.push_back(this->getTypeStr());
    //info.push_back(User::statuEnumToString(this->type));
    return info;
}

void User::printUserList(std::vector<User> users) {
    vector<string> navs = {"����", "����", "�˺�����"};
    TableRenderer render(navs, 8);
    for (int i = 0; i < users.size(); ++i) {
        render.addColume(users[i].getPrintLineStr());
    }
    render.render();
}

User User::getUserByJobNum(long long jobNum) {
    DbAdapter db("User");
    vector<vector<string>> results = db.searchBySingleField("jobNum", to_string(jobNum));
    User user;
    if (results.size() == 0) {
        cout << "����,���û�������,�ýӿڵ����豣֤�û�����";
    } else {
        user.deSerialize(results[0]);
    }

    return user;
}

const int *User::getLendDays() {
    return lendDays;
}

const int *User::getLendNums() {
    return lendNums;
}

const int *User::getAppointNums(){
    return appointNums;
}

long long int User::getJobNum() const {
    return jobNum;
}

status User::getType() const {
    return type;
}

std::string User::getTypeStr() {
	string strs[] = { "����Ա","��ʦ","�о���","������" };
	string result = "";
	int type = this->getType();
	if (type < 0) {
		type = -type;
		result = "�����";
	}
	return result+strs[type];
}

const string &User::getName() const {
    return name;
}

const string &User::getPassword() const {
    return password;
}

int User::getFirstOrderId() const {
    return firstOrderId;
}

bool User::updateUsersAssignField(std::string assignField, std::string assignValue, std::string changeField,
                                  std::string changeValue) {
    DbAdapter dbAdapter("User");
    dbAdapter.updateBySingleField(assignField, assignValue, changeField, changeValue);
    return true;
}

void User::setJobNum(long long jobNum) {
    User::jobNum = jobNum;
}

void User::setType(status type) {
    User::type = type;
}

void User::setName(const string &name) {
    User::name = name;
}

void User::setFirstOrderId(long long int firstOrderId) {
    User::firstOrderId = firstOrderId;
}

User::User(long long int jobNum, status type, const string &name) : jobNum(jobNum), type(type), name(name) {
    this->firstOrderId = -1;
    this->setPassword(to_string(this->jobNum));
}

vector<User> User::searchUsersBySingleField(std::string field, std::string value,bool isFuzzy) {
    DbAdapter dbAdapter("User");
    vector<vector<string> > queryData;
    if(isFuzzy){
        queryData = dbAdapter.searchFuzzyBySingleField(field, value);
    }else{
        queryData = dbAdapter.searchBySingleField(field, value);
    }
//    cout << "������" << queryData.size() << endl;
    return User::stringsToUsers(queryData);
}



std::vector<User> User::stringsToUsers(std::vector<std::vector<std::string>> users) {
    vector<User> results;
    for (int i = 0; i < users.size(); ++i) {
        User user;
        user.deSerialize(users[i]);
        results.push_back(user);
    }
    return results;
}

vector<User> User::searchAll() {
    vector<User> result;
    DbAdapter dbAdapter("User");
    vector<vector<string>> infos = dbAdapter.searchAll();
    for (int i = 0; i < infos.size(); ++i) {
        User user;
        user.deSerialize(infos[i]);
        result.push_back(user);
    }
    return result;
}

int User::getCanLendDays() {
    return lendDays[this->getType()];
}

int User::getCanLendNums() {
    return lendNums[this->getType()];
}

int User::getCanAppointNums() {
    return appointNums[this->getType()];
}

int User::getCanAppointDays() {
    return appointDays[this->getType()];
}
int User::returnAssignOrder(Order order) {
    // �޸Ķ���״̬
    order.setStatu(static_cast<Status>(2));
    order.setReturnTime(SimpleTime::nowTime());
    Order::updateStateAndReturnTimeById(order);

    // ��ȡͼ��ʵ��
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    // �ж�ͼ���Ƿ�ԤԼ��
    Book book = Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
    if (book.getAppointmentNum() > 0) {//��ԤԼ��,���⴦��
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

    } else {// û��ԤԼ,�黹ͼ���
        // �޸����ʵ����״̬(��Ϊ�ɽ�)
		int oldState = instance->getStatus();
		if (oldState != 3) { // ״̬Ϊ3 ��ʾ�Ѿ��¼�,������޸�״̬
			instance->setStatus(1);
			BookInstance::updateStateAndReturnTimeById(*instance);
		}

        cout << "�ɹ��黹�����Ϊ"<<instance->getId()<<"���鼮!" << endl;
    }
    return 0;
}

int User::renewAssignOrder(Order order) {
    // �ж��Ƿ������
    if (order.getStatu() != 1) {
        cout << "�ý����Ѳ���������" << endl;
        return 1;//�ý��Ĳ�������
    }

    // ��ȡͼ��ʵ��
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    // �ж�ͼ���Ƿ�ԤԼ��
    Book book = Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
    if (book.getAppointmentNum() > 0) {//��ԤԼ��,������ԤԼ
        cout << "�����Ѿ���ԤԼ,����������" << endl;
        return 2;
    } else {// û��ԤԼ,��������
        // �޸Ķ���״̬
        order.setStatu(static_cast<Status>(4));// �޸�״̬Ϊ4
        SimpleTime old = (SimpleTime &&) order.getReturnTime();//��ȡ�ɵ�Ӧ������
        order.setReturnTime(old.addDay(this->getCanLendDays()));
        Order::updateStateAndReturnTimeById(order);
        cout << "����ɹ�" << endl;
    }


    return 0;
}

bool User::getArrivedAppointment(Order order) {
    //�����Ȿ������������������ˣ���������Ҫ�޸Ķ�����Ϣ
    //todo:����һ����û�й���
    //������������ԤԼ�Ŀ�����ķ�Χ
    order.setStatu(static_cast<Status>(1));
    order.setBorrowTime(SimpleTime::nowTime());
    //��ʵ�������ʱ�䣬�Ҹ�����Ϊû������
    order.setReturnTime(SimpleTime::nowTime().addDay(this->getCanLendDays()));
    Order::updateStateAndReturnTimeAndLendTimeById(order);
    //�޸�bookInstance��Ӧ��ʱ���Լ�status״̬
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    instance->setStatus(2);
    instance->setPlanReturnDate(Date::today().addDay(this->getCanLendDays()));
    BookInstance::updateStateAndReturnTimeById(*instance);
    cout << "����ͼ������ȡ��" << endl;
	return true;
}


int User::dealWithOverTimeOrder() {
    // ��ӡ���ڵĽ���
    vector<Order> OweOrders = Order::getAssignUserOweOrder(this->getFirstOrderId());
    Order::printOrderList(OweOrders);

    int arrears = 0;//Ҫ����Ǯ,��λ��
    int allOverDays = 0;//�ܵ���������
    for (int i = 0; i < OweOrders.size(); ++i) {
        Date planreturnDate = OweOrders[i].getReturnTime().date;
        allOverDays += Date::today().compare(planreturnDate);
    }

    printf("�ܹ�����%d����,��������ʱ��%d��,Ӧ��%.2fԪ����\n", OweOrders.size(), allOverDays, allOverDays * 0.03);
    printf("����Y���ɷ���黹�鼮,Nȡ������");
    char operate;
    operate=Input::getChar();
    if (operate == 'Y' || operate == 'y') {
        for (int i = 0; i < OweOrders.size(); ++i) {
            // ����
            this->returnAssignOrder(OweOrders[i]);
        }
        printf("���ɷ���ɹ�,�����鼮�ѹ黹\n");
        return 0; // �ѽ��ɷ���
    }
    return 1;// δ���ɷ���


}


bool User::dealWithOverTimeAppointment(Order order) {
    //���Ӧ��bookId->isbn
    BookInstance *instance = BookInstance::getInstanceById(order.getBookId());
    Book book = Book::searchBooksBySingleField("isbn", instance->getIsbn())[0];
    // �ж�ͼ���Ƿ�ԤԼ��
    if(book.getAppointmentNum() == 0) {
        instance->setStatus(1);     //�����鱾ʵ��Ϊ�ɽ�
        BookInstance::updateStateAndReturnTimeById(*instance);
        order.setStatu(static_cast<Status>(6));
        order.setReturnTime(SimpleTime::nowTime());
        order.updateStateAndReturnTimeById(order);
        cout << "ԤԼ�����鼮����ɹ�,�����ѷŻ������!" << endl;
    } else if(book.getAppointmentNum() > 0){
        //�ҵ���һ���ȴ����ˣ�statu == 3
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
        orders[earliestIndex].setStatu(static_cast<Status>(5));
        orders[earliestIndex].setBookId(instance->getId());
        User earliestUser = User::getUserByJobNum(orders[earliestIndex].getUserId());       //��ȡ�ü�¼���û�
        orders[earliestIndex].setBorrowTime(SimpleTime::nowTime().addDay(earliestUser.getCanAppointDays()));
        Order::updateStateAndBookIdAndBorrowTimeById(orders[earliestIndex]);
        instance->setStatus(5);
        BookInstance::updateStateAndReturnTimeById(*instance);
        Book::updateBooksAppointmentNum(instance->getIsbn(),-1);
        cout << "ԤԼ�����鼮����ɹ�,������ת������һλ�ȴ��û�!" << endl;
    }
    return true;
}

long long User::readAndSetJobNum() {
    cout<<"�����빤��:";
    long long result=Input::getLongLong();
    this->setJobNum(result);
    return result;
}

int User::readAndSetType() {
    cout<<"�������û�����(0:����Ա 1:��ʦ 2:�о��� 3:������:";
    int result=Input::getInt();
    while(result<0||result>3){
        result=Input::getInt();
    }
    this->setType(static_cast<status>(result));
    return result;
}

std::string User::readAndSetName() {
    cout<<"����������:";
    string result=Input::getAssignMaxLengthStr(20);
    this->setName(result);
    return result;
}

std::string User::readAndSetPassword() {
    cout<<"����������:";
    string result=Input::getAssignMaxLengthStr(20);
    this->setPassword(result);
    return result;
}

bool User::isLegalUserDate() {
    if(this->getName().length()>20 ){
        return false;
    }
    return true;
}





#include "../../include/model/User.h"
#include "../../include/model/Book.h"
#include "../../include/model/BookInstance.h"
#include "../../include/core/SimpleString.h"
#include "../../include/core/MD5.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>
#include <util/TableRenderer.h>

using namespace std;

const int User::lendDays[] = {0, 90, 60, 30, 0};
const int User::lendNums[] = {0, 90, 60, 30, 0};
const int User::appointNums[] = {0, 9, 6, 3, 0};

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
    return Undergraduate;//û�ҵ�Ĭ�Ϸ��ص�,//todo:��������ôд
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
        cin >> pwd;
        if (pwd == "0") {
            return false;
        }
        if (this->isLegalPassword(pwd)) {
            break;
        } else {
            cout << "ԭ�������" << endl;
        }
    } while (true);

    // ����ȷ���������µ�����  todo: ��һ�����볤������
    cout << "������������:";
    cin >> pwd;
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
                cout << "���û������ڽ���,��Ҫ�������ܵ�½" << endl;
                cout << "����Y��������,Nȡ����¼:";
                char operate;
                cin >> operate;
                if (operate == 'Y' || operate == 'y') {
                    // �����������
                    if (user->dealWithOverTimeOrder() == 0) {
                        return 0;// ��½�ɹ�
                    }
                }
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
        User::updateUsersAssignField("jobNum", to_string(this->jobNum), "firstOrderId", to_string(orderId));
    }
	cout << "����ɹ�" << endl;
    return 0;
}

bool User::importUsers(string incomingPath="") {
	string path;
	ifstream fin;
	string line;
	if (incomingPath != "") {
		fin = ifstream(path);//���ļ�������
	}
	else {
		/* ��ȡһ����Ч·��,�������Ӧ���ļ�*/
		while (true) {
			path = SimpleString::readPathFromCmd();// "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\newBooks.csv"
			fin = ifstream(path);//���ļ�������
			if (fin.good()) {
				cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
				break;
			}
			cout << "�ļ�������,����·������������" << endl;
		}
	}

    

    int index = 0;//Ҫ���������±�
    fin.clear();
    fin.seekg(0, ios::beg); // ������ת���ļ�ͷ��
    getline(fin, line); // �Ե�����
    vector<vector<string>> users; // Ҫinsert��User�������
    vector<long long> existUsers; //�Ѿ����ڵ��û�����


    while (getline(fin, line)) //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
    {
        istringstream sin(line);

        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
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
            users.push_back(user.serialize());
        }
    }
    vector<long long> ids;
    User::addUsers(users, ids);
    cout << "����ɹ�,���¹��ŵ��û��Ѿ�����" << endl;
    for (int i = 0; i < existUsers.size(); ++i) {
        cout << existUsers[i] << endl;
    }
    return true;
}


std::string User::encryPassword(std::string pwd) {
    return calcMD5(pwd);
}

User::User(long long int jobNum, status type, const string &name, const string &password, int firstOrderId)
        : jobNum(jobNum), type(type), name(name), password(password), firstOrderId(firstOrderId) {}


bool User::getUserMessage() {
    printf("--------------------------------------------------------\n");
    // ����Ƿ���ԤԼ�ѵ����鼮
    vector<Order> AppointingOrders = Order::getAssignUserArrivedAppointmentList(this->getFirstOrderId());
    if(!AppointingOrders.empty()){//��ԤԼ�ѵ�����
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
    printf("--------------------------------------------------------\n");
    return true;
}

// todo:�������͸�һ��
bool User::appointmentAssignBook(int bookId, std::string isbn) {
    // �ж��û��Ƿ񳬹����ԤԼ��
    if(Order::getAssignUserAppointmentList(this->firstOrderId).size() >= this->getCanAppointNums()){
        cout << "�������ԤԼ��" << endl;
        return 1;
    }
    //�ж��Ƿ񳬹��ɽ�����������ɽ�����+��ԤԼ��������
    if((Order::getAssignUserBorrowingList(this->firstOrderId).size() + Order::getAssignUserAppointmentList(this->firstOrderId).size()) >= this->getCanLendNums()){
        cout << "�������ɽ���" << endl;
        return 2;
    }
    // todo:�жϸ��û��Ƿ��Ѿ������˻���ԤԼ���Ȿ��

    // �жϸ����Ƿ�ɱ�ԤԼ(�Ƿ�û��״̬Ϊ�ɽ����û���¼�) ��bookid��bookinstance
    if(!BookInstance::checkAssignBookCanAppointmentInstanceExist(isbn)){
        cout << "������Խ" << endl;
        return 3;
    }
    // ����ԤԼOrder���û��ţ��鱾Ψһ��ʶisbn(xbookId)����ǰԤԼʱ�䣬״̬Ϊ���ģ�
    Order order(this->getJobNum(), bookId, SimpleTime::nowTime(), SimpleTime::nowTime(), static_cast<Status>(3));
    //�־û�

    int orderId = Order::addSingleOrder(this->getFirstOrderId(), order);
    cout << "������" << endl;
    // ����Book����ԤԼ������Ϊ���ӣ���ʵ���������û�ã������Ҳ�֪����ô����book�־û������ݿ⣬updateô

    Book::updateBooksAppointmentNum(isbn,1);
    cout << "������" << endl;
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

void User::setJobNum1(long long int jobNum) {
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

vector<User> User::searchUsersBySingleField(std::string field, std::string value) {
    DbAdapter dbAdapter("User");
    vector<vector<string> > queryData = dbAdapter.searchBySingleField(field, value);
    cout << "������" << queryData.size() << endl;
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
        //todo:ע��һ�����string��intת���ϲ��Ϸ�,���Ϸ�
        vector<Order> orders = Order::getAssignBookAppointingList(book.getId());
        int earliestIndex = 0;       //����ʱ��������±�
        SimpleTime earliestDate = orders[0].getBorrowTime();         //��¼ʱ��������һ�죬����ĳ�ֵ����order�����һ��Ҳ�У�
        for (int i = 1; i < orders.size(); ++i) {
            //�������Ƚ�ʱ��ǰ���ҵ���С������Ǹ���
            if(earliestDate.compare(orders[i].getBorrowTime()) > 0) {    //����0˵����ǰʱ���compare���Ҫ��Ҳ����Ҫ����
                earliestIndex = i;
                earliestDate = orders[i].getBorrowTime();
            }
        }
        //�ҵ������Ժ���Ķ�Ӧ��������Ϣ����bookId�޸�Ϊ��ǰinstanceId����status��Ϊ5
        orders[earliestIndex].setStatu(static_cast<Status>(5));
        //���ʵ��id�������ô��
        orders[earliestIndex].setBookId(instance->getId());
        Order::updateStateAndBookIdById(orders[earliestIndex]);

        //�޸�BookInstance��״̬statusΪ5,����ֻ�޸���һ����᲻�ᱨ��
        instance->setStatus(5);
        //todo:�ǵ�����һ���޸ĵ���status�ĺ���
        BookInstance::updateStateAndReturnTimeById(*instance);

        //�޸�BookԤԼ������һ
        Book::updateBooksAppointmentNum(instance->getIsbn(),-1);


    } else {// û��ԤԼ,�黹ͼ���
        // �޸����ʵ����״̬(��Ϊ�ɽ�)
		int oldState = instance->getStatus();
		if (oldState != 3) { // ״̬Ϊ3 ��ʾ�Ѿ��¼�,������޸�״̬
			instance->setStatus(1);
			BookInstance::updateStateAndReturnTimeById(*instance);

		}else{
            cout<<"�����ѱ���־Ϊ�¼�,�ɹ���Ա����,������ͼ���"<<endl;
		}
        
        cout << "����ɹ�!" << endl;
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
    cin >> operate;
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






#include "../../include/model/User.h"
#include "../../include/model/Book.h"
#include "../../include/model/BookInstance.h"
#include "../../include/core/SimpleString.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>

using namespace std;

const int User::lendDays[3] = {30, 60, 90};
const int User::lendNums[3] = {30, 60, 90};

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
    // todo:��ʵ�ּ��ܺ���
    this->password = password;
    return true;
}

bool User::isLegalPassword(const std::string &password) {
    // todo:��ʵ����֤���ܺ���
    return (password == this->password);
}

bool User::changePwd(const std::string &password) {
    // ����ԭ��������

    // �ж�ԭ���������Ƿ���ȷ������ȷ������������������

    // ����ȷ���������µ�����
    return this->setPassword(password);
}

User User::login(std::string name, std::string password) {

    return User();
}

bool User::checkUserExist(long long jobNum, User *user = NULL) {
    DbAdapter db("User");
    vector<vector<string>> results = db.searchBySingleField("jobNum", to_string(jobNum));

    if (results.size() > 0) {
        if (user != NULL) {
            User temp;
            temp.deSerialize(results[0]);
            *user = temp;
        }
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
    if (this->type == status::Ban)
        return 1;
    // �ж��û����ж�����״̬,�Ƿ��г�ʱ����
    if (!Order::getAssignUserOweOrder(this->firstOrderId).empty())
        return 2;
    return 0;
}

bool User::borrowAssignBookInstance(long long bookInstanceId) {
    // �ж��û��ܷ����(�Ƿ���Ȩ����,���������Ƕ���)
    if (Order::getAssignUserBorrowingList(this->firstOrderId).size() >= User::lendNums[this->type]) {
        return 1; // ����1,���������Ѵ�����
    }
    // �жϸ����Ƿ��ܱ�����
    BookInstance *instance = BookInstance::getInstanceById(bookInstanceId);

//    if(instance == NULL){
//        return 5; // ����5,ͼ�鲻����
//    }
//    BookInstance instance=instances[0];

//    if(instances[0].status==)

    // ����һ�����ļ�¼Order,��Ҫ�ܽ���,
    Order order();
    // ���ø�Bookinstance���ɽ�,������Ӧ��ʱ��
    return false;
}

bool User::importUsers() {
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
    return "123456";
}

User::User(long long int jobNum, status type, const string &name, const string &password, long long int firstOrderId)
        : jobNum(jobNum), type(type), name(name), password(password), firstOrderId(firstOrderId) {}




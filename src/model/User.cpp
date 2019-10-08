#include "../../include/model/User.h"
#include "../../include/core/SimpleString.h"
#include <fstream>
#include <sstream>
#include <util/DbAdapter.h>

using namespace std;

User::User()
{
}


User::~User() {
}


void User::addBooksOperate() {
    string path = "";
    ifstream fin;
    while (true) {
        printf("�������ļ�·��:");
        cin >> path;
        if (path != SimpleString::fixPath(path)) {
            cout << "�ļ�·���޸�Ϊ:" << SimpleString::fixPath(path) << endl;
            path = SimpleString::fixPath(path);
        }

        cout << "����Ѱ���ļ�" << path << endl;
        // "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
        fin = ifstream(path);//���ļ�������
        if (fin.good()) {
            cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
            break;
        } else {
            cout << "�ļ�������,����·������������" << endl;
            continue;
        }

    }


    string line;
    int num = 0;
    vector<vector<string>> books;

    while (getline(fin, line))   //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
    {

        if (num++ > 10) // ��ӡ10��������
            break;
        cout << "ԭʼ�ַ�����" << line << endl;
        istringstream sin(line);

        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
        books.push_back(fields);

//        string name = fields[0];
//        string press = fields[1];
//        string author = fields[2];
//        char type = fields[3][0];
//        string isbn = fields[4];
//        int price=stof(fields[5])*100;
//        int num = stoi(fields[6]);
//        bool isLend = fields[7][0] == 'Y' || fields[7][0] == 'y';

//        vector<string> temp;
//        cout << "����֮����ַ�����" << name << "\t" << press << "\t" << author << endl;
    }

    // todo:��books���в���,�����浽���ݿ���
    return;
}


vector<string> User::serialize() {
    // todo: ��������ǵĺ�������������
    vector<string> info;
////    info.push_back(to_string(this->id));
//    info.push_back(to_string(this->jobNum));
//
//    info.push_back(to_string(this->type));
//    info.push_back(password);
    return info;
}

bool User::deSerialize(vector<string> info) {
    //todo: ��������ǵĺ�������������
//    int id = stoi(info[0]);
//    int jobNum = stoi(info[1]);
//    int borrowNum = stoi(info[2]);
//    status type;
//    type = Teacher;// todo:���ö��Ҫ��ô����
//    string password = info[4];
//    User(id, jobNum, borrowNum, type, password);
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

User::User(int jobNum, status type, const string &name, const string &password) : jobNum(jobNum), type(type),
                                                                                  name(name), password(password) {}

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

    return this->setPassword(password);
}

User User::login(std::string name, std::string password) {

    return User();
}

User User::checkUserExist(int jobNum) {
    DbAdapter db("User");
//    vector<vector<string>> results= db.searchBySingleField("jobNum",to_string(jobNum));
//    if (results.size()>0){
//        return
//    }
    return User();
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




//
// Created by Tjm on 2019/9/22.
//
#include "../include/libm.h"
#include "../src/libcpp.h"
#include "../linkDatebase/datacpp.h"
#include <fstream>
#include <regex>

using namespace std;

int regexTest();


int readUsersInfo() {
    string path;
    ifstream fin;
    while (true) {
        path = SimpleString::readPathFromCmd();
        // "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
        fin = ifstream(path);//���ļ�������
        if (fin.good()) {
            cout << "���ҵ��ļ�,���ڶ�ȡ" << endl;
            break;
        }

        cout << "�ļ�������,����·������������" << endl;
    }

    string line;
    int num = 0;
    getline(fin, line); // �Ե�����
    vector<vector<string>> users;
    while (getline(fin, line)) //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ
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


        long long jobNum = stoll(fields[0]); // ����
        string name = fields[1]; // ����
        string password = fields[2]; // ����
        int statu = stoi(fields[3]); //״̬


        User user(jobNum, static_cast<status>(statu), name, password);
        users.push_back(user.serialize());

    }
    vector<long long> ids;
    // todo: ������û��������ظ�,����Ҫ����
    User::addUsers(users, ids);


    return 0;
}

int regexTest() {

    std::string text = "{{ name }} for brown fox", replace_result;
    std::regex double_brace("(\\{\\{) (.*) (\\}\\})");

    //regex_replace����ֵ��Ϊ�滻����ַ���
    replace_result = regex_replace(text, double_brace, "*");
    cout << replace_result << "\n";

    //������н�����ַ�����[$&]��Ϊ��[]��ƥ��ɹ�����������
    // $&	Inserts the matched substring.
    cout << regex_replace(text, double_brace, "[$&]") << '\n';

    // $i�����double_brace�е�i������ƥ�䵽��ֵ
    cout << regex_replace(text, double_brace, "$1") << '\n';
    cout << regex_replace(text, double_brace, "[$2]") << '\n';
    cout << regex_replace(text, double_brace, "$3") << '\n';

    return 0;
}



int main() {

    string password;
    cin >> password;
    cout << calcMD5(password) << endl;


    Date date = Date::today();
    cout << date.serialize() << endl;
    cout << date.toInt() << endl;
    SimpleTime time = SimpleTime::nowTime();
    cout << time.serialize() << endl;
    cout << time.toLLTime() << endl;

    SimpleTime newTime = SimpleTime::llTimeToSimpleTime(time.toLLTime());
    cout << newTime.serialize() << endl;

    Date newDate = Date::intDate2Date(date.toInt());
    cout << newDate.serialize() << endl;
//     ���������鼮
//    Book::printBookList(Book::searchAll());
//////    Book::updateBooks("isbn","978-5-664-71497-5","count","999");
//    Book::importBooksService();// �����鼮�ĺ���
//    Book::printBookList(Book::searchAll());


//    User::importUsers();

//    printBookDetailInfo("978-5-664-71597-5");

    // �ֶ����뵥���鼮
//    Book::printBookList(Book::searchAll());
//    Book::addOneBookService();
//    Book::printBookList(Book::searchAll());


    // �ֶ�����ͼ���ʵ��
//    Book::printBookList(Book::searchAll());
//    BookInstance::addOneBookInstancesService();
//    Book::printBookList(Book::searchAll());

//======================================================================================================

//    // ���������û�
//    User::printUserList(User::searchAll());
//    User::importUsers();// �����û��ĺ���
//    User::printUserList(User::searchAll());

//    User::printUserList(User::searchUsersBySingleField("jobNum","15774591043"));
//    User::printUserList(User::searchUsersBySingleField("name","������"));
//    getchar();
//    getchar();
//    getchar();

    // �û�����
//    User* loginUser=Library::getSimpleUserInstance();
//    cout<<"���������� "<<loginUser->borrowAssignBookInstance(60);


}


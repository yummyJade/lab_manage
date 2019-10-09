//
// Created by Tjm on 2019/9/22.
//
#include "../include/libm.h"
#include "../src/libcpp.h"

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
    Book::printBookList(Book::searchAll());
//    Book::updateBooks("isbn","978-5-664-71497-5","count","999");
    Book::importBooks();// �����鼮�ĺ���
    Book::printBookList(Book::searchAll());
//    DbAdapter db("Book");
//    vector<vector<string> > queryData=db.searchBySingleField("isbn","978-5-664-71497-5");



//    Book::printBookList(Book::searchAll());

//    DbAdapter *dbHelper = new DbAdapter("��");
//    vector<vector<string> > queryData = dbHelper->searchBySingleField("����", "л��");
//
//
//    for (ll i = 0; i < queryData.size(); i++) {
////        printf("2");
//        for (ll j = 0; j < queryData[0].size(); j++) {
//            printf("%s\t", queryData[i][j].data());
//        }
//        printf("\n");
//    }
//    return 1;



}


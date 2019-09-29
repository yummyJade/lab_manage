//
// Created by Tjm on 2019/9/22.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../include/libm.h"
#include "../src/libcpp.h"
#include <regex>
//#include "../thirdParty/sqlite3/sqlite3.h"

using namespace std;

int regexTest();


int readBooksInfo() {

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
    getline(fin, line); // �Ե�����
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
        string name = fields[0];
        string author = fields[1];
        string press = fields[2];
        char type = fields[3][0];
        string isbn = fields[4];
        int price = stof(fields[5]) * 100;
        string position = fields[6];
        int num = stoi(fields[7]);


        cout << "����֮����ַ�����" << name << "\t" << press << "\t" << author << endl;
    }
    return 0;
}


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

        long long workNum = stoll(fields[0]);
        string name = fields[1];
        string password = fields[2];
        int statu = stoi(fields[3]);

//        User();
//        cout << "����֮����ַ�����" << name << "\t" << press << "\t" << author << endl;
    }
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


//void sqlTest();
//
//void sqlTest() {
//    sqlite3 *sql = NULL; // һ���򿪵����ݿ�ʵ��
//    const char *path = "E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\libManage.db";//ĳ��sql�ļ���·��
//
//    // �����ļ�·�������ݿ����ӡ�������ݿⲻ���ڣ��򴴽���
//    // ���ݿ��ļ���·��������C�ַ������롣
//    int result = sqlite3_open_v2(path, &sql, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX |
//                                             SQLITE_OPEN_SHAREDCACHE, NULL);
//
//    if (result == SQLITE_OK) {
//        std::clog << "�����ݿ����ӳɹ�";
//    } else {
//        std::clog << "�����ݿ�����ʧ��";
//    }
//
//
//    const char *sqlSentence = "SELECT name, author FROM Books;";    //SQL���
//    sqlite3_stmt *stmt = NULL;    // stmt�����
//
//    //���в�ѯǰ��׼����������������Ϸ���
//    //-1����ϵͳ���Զ�����SQL���ĳ���
//    int result2 = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);
//
//    if (result2 == SQLITE_OK) {
//        std::clog << "��ѯ���OK";
//        // ÿ��һ��sqlite3_step()������stmt������ͻ�ָ����һ����¼
//        while (sqlite3_step(stmt) == SQLITE_ROW) {
//            // ȡ����0���ֶε�ֵ
//
//            // ȡ����1���ֶε�ֵ
//            const void *author = sqlite3_column_text16(stmt, 1);
////            int age = sqlite3_column_int(stmt, 1);
//            //�����ز�ѯ������
//            std::clog << "name = " << ", author = " << (char *) author << endl;
//        }
//    } else {
//        std::clog << "��ѯ���������";
//    }
//    //�����������׼��ִ����һ�����
//    sqlite3_finalize(stmt);
//}


int main() {

//    readBooksInfo();

    Book::printBookList(Book::searchAll());

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


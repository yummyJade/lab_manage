#include "../../include/model/Book.h"
#include <iostream>

using namespace std;

Book::Book() {
}


Book::~Book() {
}

string Book::printType() {
    string temp;
    switch (type) {
        case 'A':
            temp = "���˼���塢�������塢ë��˼�롢��Сƽ����";
            break;
        case 'B':
            temp = "��ѧ���ڽ�";
            break;
        case 'C':
            temp = "����ѧ����";
            break;
        case 'D':
            temp = "���Ρ�����";
            break;
        case 'E':
            temp = "����";
            break;
        case 'F':
            temp = "����";
            break;
        case 'G':
            temp = "�Ļ�����ѧ������������";
            break;
        case 'H':
            temp = "���ԡ�����";
            break;
        case 'I':
            temp = "��ѧ";
            break;
        case 'J':
            temp = "����";
            break;
        case 'K':
            temp = "��ʷ������";
            break;
        case 'N':
            temp = "��Ȼ��ѧ����";
            break;
        case 'O':
            temp = "�����ѧ�ͻ�ѧ";
            break;
        case 'P':
            temp = "����ѧ�������ѧ";
            break;
        case 'Q':
            temp = "�����ѧ";
            break;
        case 'R':
            temp = "ҽҩ������";
            break;
        case 'S':
            temp = "ũҵ��ѧ";
            break;
        case 'T':
            temp = "��ҵ����";
            break;
        case 'U':
            temp = "��ͨ����";
            break;
        case 'V':
            temp = "���ա�����";
            break;
        case 'X':
            temp = "������ѧ����ȫ��ѧ";
            break;
        case 'Z':
            temp = "�ۺ���ͼ��";
            break;

        default:
            temp = "�ڿ���־";
            break;
    }
    return temp;
}

string Book::printIsLend() {
    return isLend ? "�ɽ�" : "���ɽ�";
//    if (isLend) {
//        return "�ɽ�";
//    } else if (!isLend) {
//        return "���ɽ�";
//    }
}

void Book::printBookInfo() {
    cout << id << name << author << isbn << type;
}

void Book::setBookInfo(string namet, string authort, string isbnt, char typet, double pricet) {
    name = namet;
    author = authort;
    isbn = isbnt;
    type = typet;
    price = pricet;
}


std::vector<std::string> Book::serialize() {
    vector<string> info;
//    info.push_back(to_string(this->id));
    info.push_back(to_string(this->type));
    info.push_back(to_string(this->isLend));
    info.push_back(to_string(this->isValid));
    info.push_back(to_string(this->price));
    info.push_back(this->name);
    info.push_back(this->author);
    info.push_back(this->isbn);
    info.push_back(this->press);

    return info;
}

bool Book::deSerialize(std::vector<std::string> info) {
    bool isLend = info[1].data() == "true" ? true : false;
    return true;
}

std::vector<std::vector<std::string>> Book::searchBooksBySingleField(std::string field, std::string value) {
    DbAdapter dbAdapter("��");
    vector<vector<string> > queryData = dbAdapter.searchBySingleField(field, value);
    return queryData;
}


void Book::printBookList(std::vector<std::vector<std::string>> queryData) {
    for (ll i = 0; i < queryData.size(); i++) {
//        printf("2");
        for (ll j = 0; j < queryData[0].size(); j++) {
            printf("%s\t", queryData[i][j].data());
        }
        printf("\n");
    }
}

bool
Book::updateBooks(std::string assignField, std::string assignValue, std::string changeField, std::string changeValue) {
    DbAdapter dbAdapter("��");
    dbAdapter.updateBySingleField(assignField, assignValue, changeField, changeValue);
    return true;
}

bool Book::deleteBooksByBookIds(std::vector<std::string> bookIds) {
    for (int i = 0; i < bookIds.size(); ++i) {
        Book::updateBooks("Id", bookIds[i].data(), "isValid", "false");
    }
    return true;
}

std::vector<std::vector<std::string>> Book::searchAll() {
    DbAdapter dbAdapter("��");
    return dbAdapter.searchAll();
}




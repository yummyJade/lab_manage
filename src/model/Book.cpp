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
    if (isLend) {
        return "�ɽ�";
    } else if (!isLend) {
        return "���ɽ�";
    }
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


Book *Book::searchBooksByAssignInfo(string assignInfo, int type) {
    Book *books = new Book[10];
    // todo: ̷����-�������ݿ������
    switch (type) {
        case 1:
            break;
        default:
            break;
    }
    return books;
}


//
// Created by Tjm on 2019/10/15.
//

#include <iostream>
#include <iomanip>
#include<string>
#include<windows.h>
#include <vector>
#include "libm.h"
#include "../src/libcpp.h"
#include "../src/libcpp.h"
#include "../yu/Verify.h"
#include "../yu/Verify.cpp"

using namespace std;

bool printTree(int level, string str, int deepIndex = -1);

bool printMenu(string operaNum);

void printSearchMenu();

//������ʽ
void printSearchMenu() {
    system("cls");
    while (1) {
        system("cls");
        cout << "------------ͼ������--------------" << endl
             << "1.����" << endl
             << "2.����" << endl
             << "3.����" << endl
             << "4.ISBN" << endl
             << "5.����" << endl
             << "0.������һ��" << endl
             << "---------------------------------" << endl
             << "����������ѡ��:";
        int choose;
        char input[50];
        cin.clear();
        cin >> input;
        // todo: input���ȳ���������ô��
        choose = Verify::convertDigtal(input);
        cout << "����ѡ����:" << choose << endl;


        string fieds[] = {"", "all", "name", "author", "isbn", "type"};
        int options[] = {1, 2, 3, 4, 5, 0};
        // todo: �����ж�choose�Ƿ�Ϸ�
        if (Verify::optionExist(choose, 6)) {
            cout << "����������������:";
            string keyWord;
            cin >> keyWord;
            vector<Book> books = Book::searchBooksBySingleField(fieds[choose], keyWord);
            Book::printBookList(books);
            if (choose == 0) {
                break;
            }
        }
    }
}


bool printMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.��������");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����", deepIndex);

        printTree(2, "12.�û�����(δ���)", deepIndex);
    }

    printTree(1, "2.�û�����");
    if (userOpera[0] == '2') {
        printTree(2, "21.�����û�", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "211.���������û�", deepIndex);
            printTree(3, "212.���ӵ����û�(δ���)", deepIndex);
        }

        printTree(2, "22.ɾ���û�(δ���)", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "221.����ɾ���û�", deepIndex);
            printTree(3, "222.ɾ�������û�", deepIndex);
        }
    }

    printTree(1, "3.ͼ�����");
    if (userOpera[0] == '3') {
        printTree(2, "31.����ͼ��", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "311.��������ͼ��", deepIndex);
            printTree(3, "312.���ӵ����鼮", deepIndex);
            printTree(3, "313.���ӵ����鼮", deepIndex);
        }

        printTree(2, "32.ɾ��ͼ��(δ���)", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "321.����ɾ��ͼ��", deepIndex);
            printTree(3, "322.ɾ�������鼮", deepIndex);
            printTree(3, "323.ɾ�������鼮", deepIndex);
        }

    }

    if (userOpera != "") {
        cout << endl << "��ѡ�����:" << endl;
        int operaNum = 0;
        cin >> operaNum;
        if (to_string(operaNum).length() <= deepIndex)
            printMenu(to_string(operaNum));
        switch (operaNum) {
            case 11:
                printSearchMenu();
                break;
            case 12:
                break;
            case 13:
                break;


            case 211:
                User::importUsers();
                break;
            case 212:
                break;
            case 23:
                break;


            case 311:
                Book::importBooksService();//��������ͼ��
                break;
            case 312:
                Book::addOneBookService();//���ӵ����鼮
                break;
            case 313:
                BookInstance::addOneBookInstancesService();//���ӵ����鼮
                break;
            case 32:
                break;


        }
    }


    return false;
}


bool printTree(int level, string str, int deepIndex) {
    bool isHigh = (deepIndex > 1) && (deepIndex == level);
    while (level > 1) {
        level--;
        cout << setw(2) << "";
    }
    if (isHigh) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                BACKGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
    }
    cout << "--> " << str << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return true;
}


int main() {

    string operaNum = "";
    printMenu(operaNum);
    while (operaNum != "-1") {
        cout << "�����������" << endl;
        cin >> operaNum;
        system("cls");
        printMenu(operaNum);
    }


    /*cout << "-------------------------------" << endl;
    cout << "-->" << 1 << " ͼ�����" << endl;
    cout << "  -->" << 11 << " ����ͼ��" << endl;
    cout << "    -->" << 111 << " ���ӵ���ͼ��" << endl;
    cout << "    -->" << 112 << " ���ӵ���ͼ��" << endl;
    cout << "  -->" << 12 << " ɾ��ͼ��" << endl;
    cout << "  -->" << 13 << " ����ͼ��" << endl;
    cout << "  -->" << 14 << " ����ͼ��" << endl;
    cout << "-------------------------------" << endl;
    cout << "-->" << 2 << " �û�����" << endl;
    cout << "  -->" << 11 << " �����û�" << endl;
    cout << "  -->" << 12 << " ɾ���û�" << endl;
    cout << "  -->" << 13 << " �����û�" << endl;
    cout << "  -->" << 14 << " �����û�" << endl;
    cout << "-------------------------------" << endl;
    cout << "-->" << 3 << " ��Ϣ����" << endl;
    cout << "-------------------------------" << endl;*/



    /*vector<string> navs = { "������", "����"};
    TableRenderer render(navs, 8);
    vector<vector<string>> lines = { { "1","ͼ�����" },{ "2","�û�����" },{ "3","��Ϣ����" } };
    for (int i = 0; i < lines.size(); ++i) {
        render.addColume(lines[i]);
    }
    render.render();*/
}



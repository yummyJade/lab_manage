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

bool printAdminMenu(string userOpera);

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
            cout << "��ѡ��Ҫ������ͼ�����:";


            cin >> keyWord;
            if (choose == 0) {
                break;
            }
        }
    }
}

// ��ͨ�û���ҳ
bool printUserMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.��������");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����", deepIndex);
    }

    printTree(1, "2.�ҵĽ���");
    if (userOpera[0] == '2') {
        printTree(2, "21.�ҵ��ڽ�ͼ��", deepIndex);
        printTree(2, "22.�ҵ�ԤԼͼ��", deepIndex);
        printTree(2, "21.�ҵĽ���ͼ��", deepIndex);
    }

    printTree(1, "3.�˺����");
    if (userOpera[0] == '3') {
        printTree(2, "31.�޸�����", deepIndex);
    }

}

// ����Ա�����û���ҳ��
bool printAdminDealUserMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.����/���Ĳ���");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����/����", deepIndex);
        printTree(2, "12.ԤԼ�ѵ��鼮", deepIndex);
    }

    printTree(1, "2.���ļ�¼");
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


}

// ����Ա��ҳ
bool printAdminMenu(string userOpera = "0") {
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
            printTree(3, "212.���ӵ����û�", deepIndex);
        }

        printTree(2, "22.����/�ⶳ�û�", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "221.����ָ���û�", deepIndex);
            printTree(3, "222.�ⶳָ���û�", deepIndex);
        }

        printTree(2, "23.��ʼ���û�����", deepIndex);

    }

    printTree(1, "3.ͼ�����");
    if (userOpera[0] == '3') {
        printTree(2, "31.����ͼ��", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "311.��������ͼ��", deepIndex);
            printTree(3, "312.���ӵ����鼮", deepIndex);
            printTree(3, "313.���ӵ����鼮", deepIndex);
        }

        printTree(2, "32.�¼�ͼ��(δ���)", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "321.�����¼�ͼ��", deepIndex);
            printTree(3, "322.�¼ܵ����鼮", deepIndex);
            printTree(3, "323.�¼ܵ����鼮", deepIndex);
        }

        printTree(2, "33.�޸�ͼ��", deepIndex);
        if (userOpera[1] == '3') {
            printTree(3, "331.�޸�ָ������ͼ����Ϣ", deepIndex);
            printTree(3, "332.�޸�ָ��ͼ��ʵ����Ϣ", deepIndex);
        }
    }

    printTree(1, "400.�����û�����");

    if (userOpera != "") {
        cout << endl << "��ѡ�����:" << endl;
        int operaNum = 0;
        cin >> operaNum;
//        if (to_string(operaNum).length() <= deepIndex-1){
//            printAdminMenu(to_string(operaNum));
//        }

        switch (operaNum) {
            case 11:
                printSearchMenu();
                break;
            case 12:
                break;
            case 13:
                break;


            case 211:// ���������û�
                User::importUsers();
                break;
            case 212:// ���ӵ����û�
                addSingleUserService();
                break;

            case 221:// ����ָ���û�
                freezeAssignUser();
                break;
            case 222:// �ⶳָ���û�
                unfreezeAssignUser();
                break;

            case 23:// ��ʼ���û�����
                resetAssignUserPassword();
                break;


            case 31:
                printAdminMenu(to_string(operaNum));
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
                printAdminMenu(to_string(operaNum));
            case 321:
                Book::importBooksService();//��������ͼ��
                break;
            case 322:
                Book::addOneBookService();//���ӵ����鼮
                break;
            case 323:
                BookInstance::addOneBookInstancesService();//���ӵ����鼮
                break;

            case 33:
                printAdminMenu(to_string(operaNum));
            case 331:
                changeAssignBookInfo();
                break;
            case 332:
                changeAssignBookInstanceInfo();
                break;


            case 400:
                printAdminDealUserMenu();
                break;

            default:
                printAdminMenu(to_string(operaNum));
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
    printAdminMenu(operaNum);
    while (operaNum != "-1") {
        cout << "�����������" << endl;
        cin >> operaNum;
        system("cls");
        printAdminMenu(operaNum);
    }


}



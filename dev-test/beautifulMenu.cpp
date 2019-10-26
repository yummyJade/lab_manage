#include <iostream>
#include <iomanip>
#include<string>
#include<windows.h>
#include <vector>
#include "libm.h"
#include "../src/libcpp.h"
#include "../src/libcpp.h"
#include "../linkDatebase/datacpp.h"
#include "../yu/Verify.h"
#include "../yu/Verify.cpp"

using namespace std;

bool printTree(int level, string str, int deepIndex = -1);

int printAdminMenu(string userOpera);

void printBookSearchMenu(bool canLend);

void printUserSearchMenu();

bool printLoginUserInfo();

// ��ӡ��½���û�����Ϣ
bool printLoginUserInfo() {
    User *loginUser = Library::getSimpleUserInstance();
    printf("����:%lld    ����:%s\n", loginUser->getJobNum(), loginUser->getName().c_str());
	return true;
}

// �û�����
void printUserSearchMenu() {
    system("cls");
    while (1) {
        system("cls");
        cout << "------------�û�����--------------" << endl
             << "1.����" << endl
             << "2.����" << endl
             << "3.����" << endl
             << "4.����" << endl
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


        string fieds[] = {"", "all", "jobNum", "name", "type"};
        int options[] = {1, 2, 3, 4, 0};
        // todo: �����ж�choose�Ƿ�Ϸ�
        if (Verify::optionExist(choose, 6)) {
            cout << "����������������:";
            string keyWord;
            cin >> keyWord;
            vector<User> users = User::searchUsersBySingleField(fieds[choose], keyWord);
            User::printUserList(users);
            cout << "��ѡ��Ҫ�������û�jobNum:";

            cin >> keyWord;
            if (choose == 0) {
                break;
            }
        }
    }
}

// ͼ������
void printBookSearchMenu(bool canLend=false) {
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
		if (choose == 0) {
			return;
		}
        cout << "����ѡ����:" << choose << endl;


        string fieds[] = {"", "all", "name", "author", "isbn", "type"};
        int options[] = {1, 2, 3, 4, 5, 0};
        // todo: �����ж�choose�Ƿ�Ϸ�
        if (Verify::optionExist(choose, 6)) {
            cout << "����������������:";
            string keyWord;
            cin >> keyWord;
            vector<Book> books = Book::searchBooksBySingleField(fieds[choose], keyWord);
			//cout << "�ѵ��ĵ����ݳ���Ϊ:" << books.size() << endl;
            Book::printBookList(books);
            cout << "������Ҫ�鿴�����ͼ�����(����0����):";
            int operaNum;
            cin >> operaNum;
            if (operaNum == 0) {
                break;
            } else {
                // ��ӡ��operaNum�������ϸ��Ϣ
                printBookDetailInfo(books[operaNum - 1].getIsbn());

				if (!canLend) {
					cout << "���س�������" ;
					cin.get();
					cin.get();
				}
				else {// ���Խ���
					// ��ӡ������Ϣ
					cout << "-----------------����--------------" << endl
						<< "����ͼ��(���������)" << endl
						<< "ԤԼ����(����-1)" << endl
						<< "����(����0)" << endl
						<< "����:";
					cin >> operaNum;
					if (operaNum == 0) {
						break;
					}
					else if (operaNum == -1) {
						User *loginUser = Library::getSimpleUserInstance();
                        loginUser->appointmentAssignBook(books[bookIndex - 1].getId(), books[bookIndex - 1].getIsbn());
					}
					else {
						User* loginUser = Library::getSimpleUserInstance();
						loginUser->borrowAssignBookInstance(operaNum);
					}
				}
            }
        }
    }
}

// ��ͨ�û���ҳ
int printUserMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.��������");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����", deepIndex);
    }

    printTree(1, "2.�ҵĽ���");
    if (userOpera[0] == '2') {
        printTree(2, "21.�ҵ��ڽ�ͼ��", deepIndex);
        printTree(2, "22.�ҵĽ�����ʷ", deepIndex);
        printTree(2, "21.�ҵ�ԤԼͼ��", deepIndex);
    }

    printTree(1, "3.�˺����");
    if (userOpera[0] == '3') {
        printTree(2, "31.�޸�����", deepIndex);
    }


    if (userOpera != "") {
        cout << endl << "��ѡ�����:" << endl;
        int operaNum = 0;
        cin >> operaNum;

        switch (operaNum) {
            case 11:
                printBookSearchMenu(true);
                break;
            case 12: {//��ȡԤԼ�ѵ��鼮

                break;
            }

            case 13:
                break;

            case 21:// �鿴�ڽ��¼
                Order::printOrderList(
                        Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId()));
                cout << "���س�������";
                cin.get();
                cin.get();
                break;
            case 22:// �鿴��ʷ���ļ�¼
                Order::printOrderList(
                        Order::getAssignUserBorrowedHistory(Library::getSimpleUserInstance()->getFirstOrderId()));
                cout << "���س�������";
                cin.get();
                cin.get();
                break;

            case 23:// todo:�鿴����ԤԼ�Ľ��ļ�¼

                break;

            case 31: { //�޸�����
                user->changePwd();
                break;
            }
            default :
                return operaNum;
        }
    }

	return 0;

}

// ����Ա�����û���ҳ��
int printAdminDealUserMenu(string userOpera = "0") {
    system("cls");
    printLoginUserInfo();
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.����/���Ĳ���");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����/����", deepIndex);
        printTree(2, "12.��ȡԤԼ�ѵ��鼮(�����)", deepIndex);
    }

    printTree(1, "2.���ļ�¼");
    if (userOpera[0] == '2') {
        printTree(2, "21.�鿴�ڽ��¼", deepIndex);
        printTree(2, "22.�鿴��ʷ��¼", deepIndex);
    }

    printTree(1, "3.����/����");
    if (userOpera[0] == '3') {
        printTree(2, "31.�������", deepIndex);
        printTree(2, "32.�������", deepIndex);
    }

	printTree(1, "9.ע�����û��ĵ�½");


    if (userOpera != "") {
        cout << endl << "��ѡ�����:" << endl;
        int operaNum = 0;
        cin >> operaNum;

        switch (operaNum) {
            case 11:
                printBookSearchMenu(true);
                break;
            case 12: {//��ȡԤԼ�ѵ��鼮
                //��ӡ�����ѵ���ԤԼ�鼮
                vector<Order> resultSet = Order::getAssignUserArrivedAppointmentList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                Order::printOrderList(resultSet);
                int operaNum;
                cout << "��ѡ��Ҫ��ȡ���鼮������0���أ�";
                cin >> operaNum;
                if(operaNum > 0) {  //��ȡ
                    Library::getSimpleUserInstance();
//                    User::getArrivedAppointment(resultSet[operaNum - 1]);
                }
                break;
            }

            case 13:
                break;

            case 21:// �鿴�ڽ��¼
                Order::printOrderList(
                        Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId()));
				cout << "���س�������";
				cin.get();
				cin.get();
				break;
            case 22:// �鿴��ʷ���ļ�¼
                Order::printOrderList(
                        Order::getAssignUserBorrowedHistory(Library::getSimpleUserInstance()->getFirstOrderId()));
				cout << "���س�������" ;
				cin.get();
				cin.get();
				break;

            case 31: {//����
                // ��ӡ�ڽ��б�
                vector<Order> resultSet = Order::getAssignUserBorrowingList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                Order::printOrderList(resultSet);
                int operaNum;
                cout << "ѡ��Ҫ�黹���鼮(����0����):";
                cin >> operaNum;
                if (operaNum > 0) { //����
                    Library::getSimpleUserInstance();
                    User::returnAssignOrder(resultSet[operaNum - 1]);
                }
                break;
            }

            case 32: {//����
                // ��ӡ�ڽ��б�
                vector<Order> resultSet = Order::getAssignUserBorrowingList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                Order::printOrderList(resultSet);
                int operaNum;
                cout << "ѡ��Ҫ������鼮(����0����):";
                cin >> operaNum;
                if (operaNum > 0) { //����
                    User *loginUser = Library::getSimpleUserInstance();
                    loginUser->renewAssignOrder(resultSet[operaNum - 1]);
                }
                break;
            }
			default :
				return operaNum;
				break;
        }
    }
	return 0;
}

// ����Ա��ҳ
int printAdminMenu(string userOpera = "0") {
    system("cls");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.��������");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����", deepIndex);
        printTree(2, "12.�û�����", deepIndex);
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

        switch (operaNum) {
            case 11:
                printBookSearchMenu();
                break;
            case 12:
                printUserSearchMenu();
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
                if (Library::login(true) != NULL) {
                    printAdminDealUserMenu();
                }
                break;

            default:
				return operaNum;
                printAdminMenu(to_string(operaNum));
                break;
        }
    }


    return 0;
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

	// ����Ա�����û��Ľ���
 //   string operaNum = "";
	//int resultCode= printAdminDealUserMenu(operaNum);
 //   
 //   while (resultCode != 9) {//9 ��ע������
 //       /*cout << "�����������" << endl;
 //       cin >> operaNum;
 //       system("cls");*/
	//	resultCode=printAdminDealUserMenu(to_string(resultCode));
 //   }


	// ����Ա����
	string operaNum = "";
	int resultCode= printAdminMenu(operaNum);
    
    while (resultCode != 9) {//9 ��ע������
        /*cout << "�����������" << endl;
        cin >> operaNum;
        system("cls");*/
		resultCode= printAdminMenu(to_string(resultCode));
    }

}


void trueMain() {
    User *user = Library.login();
    if (user == NULL) {// �û�����¼
        cout << "�˳�Ӧ�óɹ�" << endl;
        return;
    }
    if (user->getType() == 0) {//����Ա�˺�
        printAdminMenu();
    } else {
        printUserMenu();
    }


}


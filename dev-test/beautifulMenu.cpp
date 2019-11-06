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
#include "../src/core/Input.cpp"

using namespace std;


// ���س�������
void EnterToContinue() {
    cout << "��Enter������";
    cin.get();
}

bool printTree(int level, string str, int deepIndex = -1);

int printAdminMenu(string userOpera);

void printBookSearchMenu(bool canLend, bool isAdmin);

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
    int choose;
    while (1) {
        system("cls");
        cout << "------------�û�����--------------" << endl
             << "1.�ۺ�(ģ������)" << endl
             << "2.����" << endl
             << "3.����" << endl
             << "0.������һ��" << endl
             << "---------------------------------" << endl
             << "����������ѡ��:";

        choose = Input::getInt();
        if (choose == 0) {
            return;
        }

        //cout << "����ѡ����:" << choose << endl;
        string fieds[] = {"", "all", "jobNum", "name"};
        int options[] = {1, 2, 3, 0};

        if (choose > 0 && choose <= 3) {
            cout << "����������������:";
            string keyWord;
            keyWord = Input::getAssignMaxLengthStr(50);
            vector<User> users;
            if (choose == 1) {// ȫ������
                for (int i = 2; i <= 3; i++) {// ������,����,isbn�ֱ��������ϲ����
                    vector<User> tempUsers = User::searchUsersBySingleField(fieds[i], keyWord, true);
                    users.insert(users.end(), tempUsers.begin(), tempUsers.end());
                }
                // ����id��������
                sort(users.begin(), users.end(), [](const User &user1, const User &user2) {
                    return user1.getJobNum() > user2.getJobNum();
                });
                // ȥ��
                for (int i = users.size() - 1; i > 0; i--) {
                    if (users[i].getJobNum() == users[i - 1].getJobNum()) {
                        users.erase(users.begin() + i);
                    }
                }
            } else {
                users = User::searchUsersBySingleField(fieds[choose], keyWord);
            }

            // �жϲ�ѯ������Ƿ�Ϊ��
            if (users.empty()) {//û�鵽���
                cout << "δ�����������������û�" << endl;
                EnterToContinue();
                continue;
            }

            User::printUserList(users);
            EnterToContinue();
            //         cout << "��ѡ��Ҫ�������û����(����0����):";

            //int operaNum = -1;
            //while (operaNum<0 || operaNum > users.size()) {
            //             operaNum=Input::getInt();
            //	if (operaNum == 0) {
            //		break;
            //	}
            //	else if (operaNum<0 || operaNum > users.size()) {
            //		continue;
            //	}
            //	// todo: ��ӡ����û��������Ϣ(�������)
            //}

        }
    }
}

// ͼ������
void printBookSearchMenu(bool canLend = false, bool isAdmin=false) {
    system("cls");
    while (1) {
        system("cls");
        cout << "------------ͼ������--------------" << endl
             << "1.�ۺ�(ģ������)" << endl
             << "2.����" << endl
             << "3.����" << endl
             << "4.ISBN" << endl
             << "0.������һ��" << endl
             << "---------------------------------" << endl
             << "����������ѡ��:";
        int choose;
        choose = Input::getInt();
        if (choose == 0) {
            return;
        } else if (choose > 4 || choose < 0) {
            continue;
        }

        string fieds[] = {"", "all", "name", "author", "isbn"};
        int options[] = {1, 2, 3, 4, 0};
        if (Verify::optionExist(choose, 5)) {
            cout << "����������������:";
            string keyWord;
            keyWord = Input::getAssignMaxLengthStr(50);
            vector<Book> books;
            if (choose == 1) {// ȫ������(ģ������)
                for (int i = 2; i <= 4; i++) {// ������,����,isbn�ֱ��������ϲ����
                    vector<Book> tempbooks = Book::searchBooksBySingleField(fieds[i], keyWord, true);
                    books.insert(books.end(), tempbooks.begin(), tempbooks.end());
                }
                // ����id��������
                sort(books.begin(), books.end(), [](const Book &book1, const Book &book2) {
                    return book1.getId() > book2.getId();
                });
                // ȥ��
                for (int i = books.size() - 1; i > 0; i--) {
                    if (books[i].getId() == books[i - 1].getId()) {
                        books.erase(books.begin() + i);
                    }
                }
            } else {
                books = Book::searchBooksBySingleField(fieds[choose], keyWord);
            }

            // ����ѯ���йݲ���Ϊ0 ������ȥ��
            if(!isAdmin){
                for (int j = books.size()-1; j >=0 ; j--) {
                    if (books[j].getCount() == 0) {
                        books.erase(books.begin() + j);
                    }
                }
            }


            // �жϲ�ѯ������Ƿ�Ϊ��
            if (books.empty()) {//û�鵽���
                cout << "ͼ��������޸�ͼ��" << endl;
                EnterToContinue();
                continue;
            }

            Book::printBookList(books);
            int operaNum = -1;
            while (operaNum < 0 || operaNum > books.size()) {
                cout << "������Ҫ�鿴�����ͼ�����(����0����):";
                operaNum = Input::getInt();
                if (operaNum == 0) {
                    break;
                } else if (operaNum < 0 || operaNum > books.size()) {
                    continue;
                } else {
                    // ��ӡ��operaNum�������ϸ��Ϣ
                    int bookIndex = operaNum - 1;
                    vector<BookInstance> *instances = new vector<BookInstance>;
                    //�ж����޳���ԤԼ
                    dealWithOverTimeAppoint(books[bookIndex].getIsbn(), instances);
                    printBookDetailInfo(books[bookIndex].getIsbn(), instances);

                    if (!canLend) {
                        EnterToContinue();
                    } else {// ���Խ���
                        // ��ӡ������Ϣ
                        operaNum = -2;
                        while (operaNum < -1 || operaNum > instances->size()) {
                            cout << "-----------------����--------------" << endl
                                 << "����ͼ��(������)" << endl
                                 << "ԤԼ����(����-1)" << endl
                                 << "����(����0)" << endl
                                 << "����:";
                            operaNum = Input::getInt();
                            if (operaNum == 0) {
                                break;
                            } else if (operaNum == -1) { // ԤԼͼ��
                                User *loginUser = Library::getSimpleUserInstance();
                                loginUser->appointmentAssignBook(books[bookIndex].getId(), books[bookIndex].getIsbn());
                            } else if (operaNum > 0 && operaNum <= instances->size()) { // ����
                                User *loginUser = Library::getSimpleUserInstance();
                                int resultCode = loginUser->borrowAssignBookInstance(
                                        (*instances)[operaNum - 1].getId());
                            }else{
                                continue;
                            }
                            operaNum=0;
                            EnterToContinue();

                        }
                    }
                }
            }
        }
    }
}

// ��ͨ�û���ҳ
int printUserMenu(string userOpera = "0") {
    system("cls");
    cout<<"-----------------ϵͳʱ��:"+ Date::today().serialize()+"---------------------"<<endl;
    printf("----------------------�û�����-------------------------\n");
    printf("--------------------------------------------------------\n");
    User *user = Library::getSimpleUserInstance();
    user->getUserMessage();
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.��������");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����", deepIndex);
    }

    printTree(1, "2.�ҵĽ���");
    if (userOpera[0] == '2') {
        printTree(2, "21.�ҵ��ڽ�ͼ��", deepIndex);
        printTree(2, "22.�ҵ��ѻ�����", deepIndex);
        printTree(2, "23.�ҵ�ԤԼͼ��", deepIndex);
    }

    printTree(1, "3.�˺����");
    if (userOpera[0] == '3') {
        printTree(2, "31.�޸�����", deepIndex);
    }

    printTree(1, "9.ע�����û��ĵ�½");

    if (userOpera != "") {
        cout << endl << "��ѡ�����:" << endl;
        int operaNum = 0;
        operaNum = Input::getInt();

        switch (operaNum) {
            case 11:
                printBookSearchMenu(true);
                break;
            case 12: {

                break;
            }

            case 13:
                break;

            case 21:{// �鿴�ڽ��¼
                vector<Order> resultSet =Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ�������ڽ��ĵ�ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                }
//                Order::printOrderList(
//                        Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId()));
                EnterToContinue();
                break;
            }

            case 22:{// �鿴�ѻ�����
                vector<Order> resultSet =Order::getAssignUserCompleteOrders(Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ�����ѻ���ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                }
//                Order::printOrderList(
//                        Order::getAssignUserCompleteOrders(Library::getSimpleUserInstance()->getFirstOrderId()));
                EnterToContinue();
                break;
            }
            case 23:{
                vector<Order> resultSet =Order::getAssignUserAppointmentList(Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ��������ԤԼ��ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                }
//                Order::printOrderList(
//                        Order::getAssignUserAppointmentList(Library::getSimpleUserInstance()->getFirstOrderId()));
                EnterToContinue();
                break;
            }

            case 31: { //�޸�����
                User *user = Library::getSimpleUserInstance();
                user->changePwdService();
                EnterToContinue();
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
    cout<<"-----------------ϵͳʱ��:"+ Date::today().serialize()+"---------------------"<<endl;
    printf("------------------����Ա�����û�����--------------------\n");
    printf("--------------------------------------------------------\n");
    printLoginUserInfo();
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.����/����/ԤԼ����");
    if (userOpera[0] == '1') {
        printTree(2, "11.ͼ�����/����/ԤԼ", deepIndex);
        printTree(2, "12.��ȡԤԼ�ѵ��鼮", deepIndex);
    }

    printTree(1, "2.���ļ�¼");
    if (userOpera[0] == '2') {
        printTree(2, "21.�鿴�ڽ��¼", deepIndex);
        printTree(2, "22.�鿴�ѻ�����", deepIndex);
        printTree(2, "23.�鿴ԤԼ����", deepIndex);

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
        operaNum = Input::getInt();

        switch (operaNum) {
            case 11:
                printBookSearchMenu(true);
                break;
            case 12: {//��ȡԤԼ�ѵ��鼮
                //��ӡ�����ѵ���ԤԼ�鼮
                vector<Order> resultSet = Order::getAssignUserArrivedAppointmentList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "������ԤԼ�ѵ����鼮,�����ĵȴ�" << endl;
                } else {
                    Order::printOrderList(resultSet);
                    int operaNum;
                    cout << "��ѡ��Ҫ��ȡ���鼮�ı�ţ�����0���أ�:";
                    operaNum = Input::getInt();
                    if (operaNum > 0 && operaNum <= resultSet.size()) {  //��ȡ
                        User *loginUser = Library::getSimpleUserInstance();
                        loginUser->getArrivedAppointment(resultSet[operaNum - 1]);
                    } else {
                        cout << "����ı������" << endl;
                    }
                }
                EnterToContinue();
                break;
            }

            case 13:
                break;

            case 21:{// �鿴�ڽ��¼
                vector<Order> resultSet =Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ�������ڽ��ĵ�ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                }
//                Order::printOrderList(
//                        Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId()));
                EnterToContinue();
                break;
            }

            case 22:{// �鿴�ѻ�����
                vector<Order> resultSet =Order::getAssignUserCompleteOrders(Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ�����ѻ���ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                }
//                Order::printOrderList(
//                        Order::getAssignUserCompleteOrders(Library::getSimpleUserInstance()->getFirstOrderId()));
                EnterToContinue();
                break;
            }
            case 23:{// todo:�鿴����ԤԼ�Ľ��ļ�¼
                vector<Order> resultSet =Order::getAssignUserAppointmentList(Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ��������ԤԼ��ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                }
//                Order::printOrderList(
//                        Order::getAssignUserAppointmentList(Library::getSimpleUserInstance()->getFirstOrderId()));
                EnterToContinue();
                break;
            }


            case 31: {//����
                // ��ӡ�ڽ��б�
                vector<Order> resultSet = Order::getAssignUserBorrowingList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ�����ڽ�ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                    int operaNum;
                    cout << "ѡ��Ҫ�黹���鼮(����0����):";
                    operaNum = Input::getInt();
                    if (operaNum > 0 && operaNum <= resultSet.size()) { //����
                        Library::getSimpleUserInstance();
                        int resultCode = User::returnAssignOrder(resultSet[operaNum - 1]);
//                        if (resultCode == 0) {
//                            cout << "����ɹ�,";
//                        } else {
//                            cout << "����ʧ��,";
//                        }
                    }
                }
                EnterToContinue();
                break;
            }

            case 32: { //����
                // ��ӡ�ڽ��б�
                vector<Order> resultSet = Order::getAssignUserBorrowingList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                if (resultSet.empty()) {
                    cout << "����ǰ�����ڽ�ͼ��" << endl;
                } else {
                    Order::printOrderList(resultSet);
                    int operaNum;
                    cout << "ѡ��Ҫ������鼮(����0����):";
                    operaNum = Input::getInt();
                    if (operaNum > 0 && operaNum <= resultSet.size()) { //����
                        User *loginUser = Library::getSimpleUserInstance();
                        loginUser->renewAssignOrder(resultSet[operaNum - 1]);

                    }
                }
                EnterToContinue();

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

    cout<<"-----------------ϵͳʱ��:"+ Date::today().serialize()+"---------------------"<<endl;
    printf("--------------------����Ա����--------------------------\n");
    printf("--------------------------------------------------------\n");
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

        printTree(2, "24.�����û����ڽ���", deepIndex);

    }

    printTree(1, "3.ͼ�����");
    if (userOpera[0] == '3') {
        printTree(2, "31.����ͼ��", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "311.��������ͼ��", deepIndex);
            printTree(3, "312.���ӵ����鼮", deepIndex);
            printTree(3, "313.���ӵ����鼮", deepIndex);
        }

        printTree(2, "32.�޸�ͼ����Ϣ", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "321.�޸�ָ��ISBNͼ����Ϣ", deepIndex);
            printTree(3, "322.�޸�ָ��ͼ��ʵ����Ϣ", deepIndex);
        }

        printTree(2, "33.�¼�ͼ��", deepIndex);
        if (userOpera[1] == '3') {
            printTree(3, "331.�¼�ָ��ISBN������ͼ��", deepIndex);
            printTree(3, "332.�¼�ָ������ŵ�ͼ��", deepIndex);
        }
    }

    printTree(1, "4.�˺����");
    if (userOpera[0] == '4') {
        printTree(2, "41.�޸�����", deepIndex);
    }

    printTree(1, "5.ͼ�����Ϣ");
    if (userOpera[0] == '5') {
        printTree(2, "51.�鿴��������δ�����", deepIndex);
        printTree(2, "52.�鿴ԤԼ����δȡ���", deepIndex);
        printTree(2, "53.�������а�(������߰�)", deepIndex);
        printTree(2, "54.�鿴�����û��Ĺ�ͬ����(��߰�)", deepIndex);
    }

    printTree(1, "800.�����û�����");
    printTree(1, "9.�˳�����");

    if (userOpera != "") {
        cout << "\n��ѡ�����:" << endl;
        int operaNum = 0;
        operaNum = Input::getInt();

        switch (operaNum) {
            case 11:
                printBookSearchMenu(false,true);
                break;
            case 12:
                printUserSearchMenu();
                break;
            case 13:
                break;


            case 211:// ���������û�
                User::importUsers();
                EnterToContinue();
                break;
            case 212:// ���ӵ����û�
                addSingleUserService();
                EnterToContinue();
                break;

            case 221:// ����ָ���û�
                freezeAssignUser();
                EnterToContinue();
                break;
            case 222:// �ⶳָ���û�
                unfreezeAssignUser();
                EnterToContinue();
                break;

            case 23:// ��ʼ���û�����
                resetAssignUserPassword();
                EnterToContinue();
                break;

            case 24: {//�����û����ڼ�¼
                dealWithOverTimeOrder();
                EnterToContinue();
                break;
            }


            case 31:
                printAdminMenu(to_string(operaNum));
                break;
            case 311:
                Book::importBooksService();//��������ͼ��
                EnterToContinue();
                break;
            case 312:
                Book::addOneBookService();//���ӵ����鼮
                EnterToContinue();
                break;
            case 313:
                BookInstance::addOneBookInstancesService();//���ӵ����鼮
                EnterToContinue();
                break;

            case 32:
                printAdminMenu(to_string(operaNum));
                break;
            case 321:
                changeAssignBookInfo();// �޸�ͼ��������Ϣ
                EnterToContinue();
                break;
            case 322:
                changeAssignBookInstanceInfo(); // �޸ĵ���ͼ��ʵ����Ϣ
                EnterToContinue();
                break;

            case 33: //�¼�ͼ��
                printAdminMenu(to_string(operaNum));
                break;
            case 331:
                deleteAssignIsbnBook();//�¼�ָ��ISBN��ͼ��
                EnterToContinue();
                break;
            case 332:
                deleteAssignBookInstance();//�¼�ָ��ͼ��ʵ��
                EnterToContinue();
                break;

            case 41: {
                User *user = Library::getAdminUserInstance();
                user->changePwdService();
                EnterToContinue();
                break;
            }

            case 51: {// ��ӡ���ڽ���δ��
                vector<Order> resultSet = Order::getAllBorrowedOweList();
//                vector<>
                if(resultSet.empty()) {
                    cout << "��ǰ�������ڽ��Ķ���" << endl;
                } else {

                    sort(resultSet.begin(), resultSet.end(), [](const Order &order1, const Order &order2) {
                        SimpleTime returnTimeOne = order1.getReturnTime();
                        return (returnTimeOne.compare(order2.getReturnTime()) < 0);
                    });
                    Order::printOrderOweList(resultSet,2);
                }
                EnterToContinue();
                break;
            }

            case 52: {// ��ӡ����ԤԼδȡ
                vector<Order> resultSet = Order::getAllAppointArrivedOweList();
                if (resultSet.empty()) {
                    cout << "��ǰ��������ԤԼ����" << endl;
                    EnterToContinue();
                    break;
                } else {
                    sort(resultSet.begin(), resultSet.end(), [](const Order &order1, const Order &order2) {
                        SimpleTime borrowTimeOne = order1.getBorrowTime();
                        return (borrowTimeOne.compare(order2.getBorrowTime()) < 0);
                    });
//                    Order::printOrderOweList(resultSet);
                    Order::printOrderOweList(resultSet,3);
                    int operaNum = -2;
                    while (operaNum < -1 || operaNum > resultSet.size()) {
                        cout << "-----------------����--------------" << endl
                             << "������ͼ��(������)" << endl
                             //                             << "ԤԼ����(����-1)" << endl
                             << "����(����0)" << endl
                             << "����:";
                        operaNum = Input::getInt();
                        if (operaNum == 0) {
                            EnterToContinue();
                            break;
                        } else if (operaNum > 0 && operaNum <= resultSet.size()) {
//                            User *loginUser = Library::getSimpleUserInstance();
//                            int resultCode = loginUser->borrowAssignBookInstance(
//                                    (*instances)[operaNum - 1].getId());
                            User *loginUser = Library::getSimpleUserInstance();
                            loginUser->dealWithOverTimeAppointment(resultSet[operaNum - 1]);
                        } else {
                            continue;
                        }
                        operaNum = 0;
                        EnterToContinue();
                        break;
                    }
//                    EnterToContinue();
                }
                break;
            }

            case 53:{// �������а�
                while (1){
                    cout<<"������Ҫ�鿴�İ񵥳���(����0ȡ��):";
                    int operaNum = Input::getInt();
                    if(operaNum==0){
                        break;
                    }else if(operaNum>0){
                        Book::getMostPopularBook(operaNum);
                        break;
                    }
                }
                EnterToContinue();
                break;
            }

            case 54:{// ��ͬ����
                printSameBorrowBooksBetweenTwoUser();
                EnterToContinue();
                break;
            }

            case 800: // �����û�����
                if (Library::login(true) != NULL) {
                    int code = printAdminDealUserMenu();
                    while (code != 9) {
                        code = printAdminDealUserMenu(to_string(code));
                    }
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

void trueMain() {
    User *user = Library::login();
    if (user == NULL) {// �û�����¼
        cout << "�˳�Ӧ�óɹ�" << endl;
        return;
    }
    if (user->getType() == 0) {//����Ա�˺�
        string operaNum = "";
        int resultCode = printAdminMenu(operaNum);

        while (resultCode != 9) {//9 ��ע������
            resultCode = printAdminMenu(to_string(resultCode));
        }
    } else {
        string operaNum = "";
        int resultCode = printUserMenu(operaNum);

        while (resultCode != 9) {//9 ��ע������
            resultCode = printUserMenu(to_string(resultCode));
        }
    }


}

int main() {
    if (!Library::checkDBLegal()) {
        cout << "���ݿ��ļ���,�������������" << endl;
        return 0;
    }



    //Book::printBookList(Book::searchAll());
    //Book::importBooksService();//"E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
    //Book::printBookList(Book::searchAll());
    ////
    ////// �����û�
    //User::printUserList(User::searchAll());
    //User::importUsers();//"E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\users.csv"
    //User::printUserList(User::searchAll());


    // ����Ա�����û��Ľ���
//       string operaNum = "";
//    int resultCode= printAdminDealUserMenu(operaNum);
//
//       while (resultCode != 9) {//9 ��ע������
//           /*cout << "�����������" << endl;
//           cin >> operaNum;
//           system("cls");*/
//    	resultCode=printAdminDealUserMenu(to_string(resultCode));
//       }


    // ����Ա����
	string operaNum = "";
	int resultCode= printAdminMenu(operaNum);

    while (resultCode != 9) {//9 ��ע������
        /*cout << "�����������" << endl;
        cin >> operaNum;
        system("cls");*/
		resultCode= printAdminMenu(to_string(resultCode));
    }

    // ��ͨ�û�����
//	string operaNum = "";
//	int resultCode= printAdminMenu(operaNum);
//
//    while (resultCode != 9) {//9 ��ע������
//        /*cout << "�����������" << endl;
//        cin >> operaNum;
//        system("cls");*/
//		resultCode= printUserMenu(to_string(resultCode));
//    }


//    trueMain();

}





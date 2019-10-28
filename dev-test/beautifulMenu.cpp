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


// 按回车键返回
void EnterToContinue() {
	cout << "按Enter键返回";
	cin.get();
	cin.get();
}

bool printTree(int level, string str, int deepIndex = -1);

int printAdminMenu(string userOpera);

void printBookSearchMenu(bool canLend);

void printUserSearchMenu();

bool printLoginUserInfo();

// 打印登陆的用户的信息
bool printLoginUserInfo() {
    User *loginUser = Library::getSimpleUserInstance();
    printf("工号:%lld    姓名:%s\n", loginUser->getJobNum(), loginUser->getName().c_str());
	return true;
}


// 用户搜索
void printUserSearchMenu() {
    system("cls");
	int choose;
    while (1) {
        system("cls");
        cout << "------------用户搜索--------------" << endl
             << "1.综合" << endl
             << "2.工号" << endl
             << "3.姓名" << endl
             << "0.返回上一级" << endl
             << "---------------------------------" << endl
             << "请输入您的选择:";
        
        cin >> choose;
		if (choose == 0) {
			return;
		}

        cout << "您的选择是:" << choose << endl;
        string fieds[] = {"", "all", "jobNum", "name"};
        int options[] = {1, 2, 3, 0};

        if (choose>0 && choose<=3) {
            cout << "请输入您的搜索词:";
            string keyWord;
            cin >> keyWord;
			vector<User> users;
			if (choose == 1) {// 全部搜索
				for (int i = 2; i <= 3; i++) {// 按姓名,作者,isbn分别搜索并合并结果
					vector<User> tempUsers = User::searchUsersBySingleField(fieds[i], keyWord);
					users.insert(users.end(), tempUsers.begin(), tempUsers.end());
				}
				// 根据id进行排序
				sort(users.begin(), users.end(), [](const User& user1, const User& user2) {
					return user1.getJobNum() > user2.getJobNum();
					});
				// 去重
				for (int i = users.size() - 1; i > 0; i--) {
					if (users[i].getJobNum() == users[i - 1].getJobNum()) {
						users.erase(users.begin() + i);
					}
				}
			}

			else {
				users = User::searchUsersBySingleField(fieds[choose], keyWord);
			}

			// 判断查询结果集是否为空
			if (users.empty()) {//没查到结果
				cout << "未检索到符合条件的用户" << endl;
				EnterToContinue();
				continue;
			}

            User::printUserList(users);
            cout << "请选择要操作的用户编号(输入0返回):";

			int operaNum = -1;
			while (operaNum<0 || operaNum > users.size()) {
				cin >> operaNum;
				if (operaNum == 0) {
					break;
				}
				else if (operaNum<0 || operaNum > users.size()) {
					continue;
				}
				// todo: 打印这个用户的相关信息(借阅情况)
			}
            
        }
    }
}

// 图书搜索
void printBookSearchMenu(bool canLend=false) {
    system("cls");
    while (1) {
        system("cls");
        cout << "------------图书搜索--------------" << endl
             << "1.综合" << endl
             << "2.书名" << endl
             << "3.作者" << endl
             << "4.ISBN" << endl
             << "0.返回上一级" << endl
             << "---------------------------------" << endl
             << "请输入您的选择:";
        int choose;
        char input[50];
        cin.clear();
        cin >> input;
        // todo: input长度超出来了怎么办
        choose = Verify::convertDigtal(input);
		if (choose == 0) {
			return;
		}
        cout << "您的选择是:" << choose << endl;


        string fieds[] = {"", "all", "name", "author", "isbn"};
        int options[] = {1, 2, 3, 4, 0};
        if (Verify::optionExist(choose, 5)) {
            cout << "请输入您的搜索词:";
            string keyWord;
            cin >> keyWord;
			vector<Book> books;
			if (choose == 1) {// 全部搜索
				for (int i = 2; i <= 4; i++) {// 按姓名,作者,isbn分别搜索并合并结果
					vector<Book> tempbooks= Book::searchBooksBySingleField(fieds[i], keyWord);
					books.insert(books.end(), tempbooks.begin(), tempbooks.end());
				}
				// 根据id进行排序
				sort(books.begin(), books.end(), [](const Book& book1, const Book& book2){ 
						return book1.getId()>book2.getId();
					});
				// 去重
				for (int i = books.size()-1; i >0; i--) {
					if (books[i].getId() == books[i - 1].getId()) {
						books.erase(books.begin()+i);
					}
				}
			}
			else {
				books = Book::searchBooksBySingleField(fieds[choose], keyWord);
			}
            
			// 判断查询结果集是否为空
			if (books.empty()) {//没查到结果
				cout << "图书馆内暂无该图书" << endl;
				EnterToContinue();

				continue;
			}

            Book::printBookList(books);
			int operaNum=-1;
			while (operaNum<0 || operaNum > books.size()) {
				cout << "请输入要查看详情的图书序号(输入0返回):";

				cin >> operaNum;
				if (operaNum == 0) {
					break;
				} else if(operaNum<0 || operaNum > books.size()) {
					continue;
				}
				else {
					// 打印第operaNum本书的详细信息
					int bookIndex = operaNum - 1;
					vector<BookInstance>* instances = new vector<BookInstance>;
					printBookDetailInfo(books[bookIndex].getIsbn(), instances);

					if (!canLend) {
						EnterToContinue();
					}
					else {// 可以借阅
						// 打印操作信息
						operaNum = -2;
						while (operaNum<-1 || operaNum> instances->size()) {
							cout << "-----------------操作--------------" << endl
								<< "借阅图书(输入编号)" << endl
								<< "预约该书(输入-1)" << endl
								<< "返回(输入0)" << endl
								<< "输入:";
							cin >> operaNum;
							if (operaNum == 0) {
								break;
							}
							else if (operaNum == -1) { // 预约图书
								User* loginUser = Library::getSimpleUserInstance();
								loginUser->appointmentAssignBook(books[bookIndex].getId(), books[bookIndex].getIsbn());
							}
							else if(operaNum>0 && operaNum<= instances->size()) { // 借书

								User* loginUser = Library::getSimpleUserInstance();
								int resultCode = loginUser->borrowAssignBookInstance((*instances)[operaNum - 1].getId());
								
								EnterToContinue();
							}
						}
					}
				}
			}
        }
    }
}

// 普通用户首页
int printUserMenu(string userOpera = "0") {
    system("cls");
	printf("--------------------------------------------------------\n");
	printf("----------------------用户界面-------------------------\n");
	printf("--------------------------------------------------------\n");
    User* user=Library::getSimpleUserInstance();
    user->getUserMessage();
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.检索操作");
    if (userOpera[0] == '1') {
        printTree(2, "11.图书检索", deepIndex);
    }

    printTree(1, "2.我的借阅");
    if (userOpera[0] == '2') {
        printTree(2, "21.我的在借图书", deepIndex);
        printTree(2, "22.我的借阅历史", deepIndex);
        printTree(2, "23.我的预约图书", deepIndex);
    }

    printTree(1, "3.账号相关");
    if (userOpera[0] == '3') {
        printTree(2, "31.修改密码", deepIndex);
    }

	printTree(1, "9.注销该用户的登陆");

    if (userOpera != "") {
        cout << endl << "请选择操作:" << endl;
        int operaNum = 0;
        cin >> operaNum;

        switch (operaNum) {
            case 11:
                printBookSearchMenu(true);
                break;
            case 12: {

                break;
            }

            case 13:
                break;

            case 21:// 查看在借记录
                Order::printOrderList(
                        Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId()));
				EnterToContinue();
                break;
            case 22:// 查看历史借阅记录
                Order::printOrderList(
                        Order::getAssignUserBorrowedHistory(Library::getSimpleUserInstance()->getFirstOrderId()));
				EnterToContinue();
                break;

            case 23:// todo:查看正在预约的借阅记录
				Order::printOrderList(
					Order::getAssignUserAppointmentList(Library::getSimpleUserInstance()->getFirstOrderId()));
				EnterToContinue();
                break;

            case 31: { //修改密码
                User* user=Library::getSimpleUserInstance();
                user->changePwdService();
                break;
            }
            default :
                return operaNum;
        }
    }

	return 0;

}

// 管理员操作用户的页面
int printAdminDealUserMenu(string userOpera = "0") {
    system("cls");
	printf("--------------------------------------------------------\n");
	printf("------------------管理员操作用户界面--------------------\n");
	printf("--------------------------------------------------------\n");
    printLoginUserInfo();
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.检索/借阅操作");
    if (userOpera[0] == '1') {
        printTree(2, "11.图书检索/借阅", deepIndex);
        printTree(2, "12.领取预约已到书籍", deepIndex);
    }

    printTree(1, "2.借阅记录");
    if (userOpera[0] == '2') {
        printTree(2, "21.查看在借记录", deepIndex);
        printTree(2, "22.查看历史记录", deepIndex);
    }

    printTree(1, "3.还书/续借");
    if (userOpera[0] == '3') {
        printTree(2, "31.还书操作", deepIndex);
        printTree(2, "32.续借操作", deepIndex);
    }

	printTree(1, "9.注销该用户的登陆");


    if (userOpera != "") {
        cout << endl << "请选择操作:" << endl;
        int operaNum = 0;
        cin >> operaNum;

        switch (operaNum) {
            case 11:
                printBookSearchMenu(true);
                break;
            case 12: {//领取预约已到书籍
                //打印所有已到的预约书籍
                vector<Order> resultSet = Order::getAssignUserArrivedAppointmentList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                Order::printOrderList(resultSet);
                int operaNum;
                cout << "请选择要领取的书籍（输入0返回）";
                cin >> operaNum;
                if(operaNum > 0) {  //领取
                    User* loginUser=Library::getSimpleUserInstance();
					loginUser->getArrivedAppointment(resultSet[operaNum - 1]);
                }
                break;
            }

            case 13:
                break;

            case 21:// 查看在借记录
                Order::printOrderList(
                        Order::getAssignUserBorrowingList(Library::getSimpleUserInstance()->getFirstOrderId()));
				cout << "按回车键返回";
				cin.get();
				cin.get();
				break;
            case 22:// 查看历史借阅记录
                Order::printOrderList(
                        Order::getAssignUserBorrowedHistory(Library::getSimpleUserInstance()->getFirstOrderId()));
				cout << "按回车键返回" ;
				cin.get();
				cin.get();
				break;

            case 31: {//还书
                // 打印在借列表
                vector<Order> resultSet = Order::getAssignUserBorrowingList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                Order::printOrderList(resultSet);
                int operaNum;
                cout << "选择要归还的书籍(输入0返回):";
                cin >> operaNum;
                if (operaNum > 0) { //还书
                    Library::getSimpleUserInstance();
                    int resultCode=User::returnAssignOrder(resultSet[operaNum - 1]);
					if (resultCode == 0) {
						cout << "还书成功,按Enter返回";
					}
					else {
						cout << "还书失败,按Enter返回";
					}
					cin.get();
					cin.get();
                }
                break;
            }

			case 32: { //续借
                // 打印在借列表
                vector<Order> resultSet = Order::getAssignUserBorrowingList(
                        Library::getSimpleUserInstance()->getFirstOrderId());
                Order::printOrderList(resultSet);
                int operaNum;
                cout << "选择要续借的书籍(输入0返回):";
                cin >> operaNum;
                if (operaNum > 0) { //续借
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

// 管理员首页
int printAdminMenu(string userOpera = "0") {
    system("cls");
	printf("--------------------------------------------------------\n");
	printf("--------------------管理员界面--------------------------\n");
	printf("--------------------------------------------------------\n");
    int deepIndex = userOpera.length() + 1;

    printTree(1, "1.检索操作");
    if (userOpera[0] == '1') {
        printTree(2, "11.图书检索", deepIndex);
        printTree(2, "12.用户检索", deepIndex);
    }

    printTree(1, "2.用户管理");
    if (userOpera[0] == '2') {
        printTree(2, "21.增加用户", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "211.批量导入用户", deepIndex);
            printTree(3, "212.增加单个用户", deepIndex);
        }

        printTree(2, "22.冻结/解冻用户", deepIndex);
        if (userOpera[1] == '2') {
            printTree(3, "221.冻结指定用户", deepIndex);
            printTree(3, "222.解冻指定用户", deepIndex);
        }

        printTree(2, "23.初始化用户密码", deepIndex);

    }

    printTree(1, "3.图书管理");
    if (userOpera[0] == '3') {
        printTree(2, "31.增加图书", deepIndex);
        if (userOpera[1] == '1') {
            printTree(3, "311.批量导入图书", deepIndex);
            printTree(3, "312.增加单种书籍", deepIndex);
            printTree(3, "313.增加单本书籍", deepIndex);
        }

		printTree(2, "32.修改图书信息", deepIndex);
		if (userOpera[1] == '2') {
			printTree(3, "321.修改指定ISBN图书信息", deepIndex);
			printTree(3, "322.修改指定图书实例信息", deepIndex);
		}

        printTree(2, "33.下架图书", deepIndex);
        if (userOpera[1] == '3') {
            printTree(3, "331.下架指定ISBN的所有图书", deepIndex);
			printTree(3, "332.下架指定条码号的图书", deepIndex);
        }

        
    }

    printTree(1, "400.处理用户操作");
	printTree(1, "9.注销该用户的登陆");

    if (userOpera != "") {
        cout << endl << "请选择操作:" << endl;
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


            case 211:// 批量导入用户
                User::importUsers();
                break;
            case 212:// 增加单个用户
                addSingleUserService();
                break;

            case 221:// 冻结指定用户
                freezeAssignUser();
				EnterToContinue();
                break;
            case 222:// 解冻指定用户
                unfreezeAssignUser();
				EnterToContinue();
                break;

            case 23:// 初始化用户密码
                resetAssignUserPassword();
                break;


            case 31:
                printAdminMenu(to_string(operaNum));
				break;
            case 311:
                Book::importBooksService();//批量导入图书
				EnterToContinue();
                break;
            case 312:
                Book::addOneBookService();//增加单种书籍
				EnterToContinue();
                break;
            case 313:
                BookInstance::addOneBookInstancesService();//增加单本书籍
				EnterToContinue();
                break;

            case 32: 
                printAdminMenu(to_string(operaNum));
				break;
            case 321: 
                changeAssignBookInfo();// 修改图书种类信息
                break;
            case 322: 
                changeAssignBookInstanceInfo(); // 修改单个图书实例信息
                break;

			case 33: //下架图书
			    printAdminMenu(to_string(operaNum));
				break;
			case 331:
				deleteAssignIsbnBook();//下架指定ISBN的图书
			    break;
			case 332:
				deleteAssignBookInstance();//下架指定图书实例
			    break;

            case 400: // 处理用户操作
                if (Library::login(true) != NULL) {
                    int code=printAdminDealUserMenu();
					while (code != 9) {
						code=printAdminDealUserMenu(to_string(code));
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
    if (user == NULL) {// 用户不登录
        cout << "退出应用成功" << endl;
        return;
    }
    if (user->getType() == 0) {//管理员账号
		string operaNum = "";
		int resultCode= printAdminMenu(operaNum);

		while (resultCode != 9) {//9 是注销操作
			resultCode= printAdminMenu(to_string(resultCode));
		}
    } else {
		string operaNum = "";
		int resultCode = printUserMenu(operaNum);

		while (resultCode != 9) {//9 是注销操作
			resultCode = printUserMenu(to_string(resultCode));
		}
    }


}

int main() {
	//Book::printBookList(Book::searchAll());
	//Book::importBooksService();//"E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\books.csv"
	//Book::printBookList(Book::searchAll());
	////
	////// 导入用户
	//User::printUserList(User::searchAll());
	//User::importUsers();//"E:\\Sources\\Cpp\\repos\\Lib_manage\\dev-Tan\\users.csv"
	//User::printUserList(User::searchAll());

	// 管理员操作用户的界面
 //   string operaNum = "";
	//int resultCode= printAdminDealUserMenu(operaNum);
 //   
 //   while (resultCode != 9) {//9 是注销操作
 //       /*cout << "请输入操作数" << endl;
 //       cin >> operaNum;
 //       system("cls");*/
	//	resultCode=printAdminDealUserMenu(to_string(resultCode));
 //   }


	// 管理员界面
//	string operaNum = "";
//	int resultCode= printAdminMenu(operaNum);
//
//    while (resultCode != 9) {//9 是注销操作
//        /*cout << "请输入操作数" << endl;
//        cin >> operaNum;
//        system("cls");*/
//		resultCode= printAdminMenu(to_string(resultCode));
//    }

	// 普通用户界面
	//string operaNum = "";
	//int resultCode= printAdminMenu(operaNum);

 //   while (resultCode != 9) {//9 是注销操作
 //       /*cout << "请输入操作数" << endl;
 //       cin >> operaNum;
 //       system("cls");*/
	//	resultCode= printUserMenu(to_string(resultCode));
 //   }


    trueMain();

}





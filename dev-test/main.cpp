#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include "libm.h"
#include "../src/util/DbAdapter.cpp"

using namespace std;

//��������
void printUserMenu();
void printMainMenu();
void printAdminMenu();
void printSearchMenu();
void printUserManageMenu();
void printSearchResult();

bool offFlag = false;
//һЩ��Ҫ�ȳ�ʼ���Ķ���
void init(){
//    DbAdapter *dbHelper = new DbAdapter("��");

}
//��ӡ���˵�
void printMainMenu() {

    while (1) {
        system("cls");
        cout << "------------ͼ��ݹ���ϵͳ--------------" << endl
             << "1.�û���¼" << endl
             << "2.����Ա��¼" << endl
             << "3.ͼ������" << endl
             << "0.�˳�ϵͳ" << endl
             << "--------------------------------------" << endl
             << "����������ѡ��:";
        int choose;
        cin >> choose;
        cout << "����ѡ����:" << choose << endl;
        switch (choose) {
            case 1:
                printUserMenu();
                break;
            case 2: //todo:��Ҫ���ӵ�¼����֤
                printAdminMenu();
                break;
            case 3:
                printSearchMenu();
                break;
            case 0:
                break;

            default:
                cout << "��ѡ����ڣ�����������!" << endl;
                break;

        }
        if (choose == 0) {
            cout << "��ӭ�����´η���" << endl;
            break;
        }

    }
}

//�û��˵�
void printUserMenu(){
    while (1){


        cout << "------------�û�����--------------" << endl
             << "1.ͼ������" << endl
             << "2.���˶���" << endl
             << "3.�޸�����" << endl
             << "0.������һ��" << endl
             << "---------------------------------" << endl
             << "����������ѡ��:";
        int choose;
        cin >> choose;
        cout << "����ѡ����:" << choose << endl;
        switch (choose){
            case 1:
                printSearchMenu();break;
            case 2:
                cout << "���Ķ�����ѯ������£�" << endl;
                void printSearchResult();
                cout << std::left << setw(10) << "1" << setw(20) << "�ں�֮��" << setw(20) << "����"
                     << setw(20) << "123456678" << setw(20) << "����ѧ" << endl;
                break;  //�����û���ѯ�����ĺ���
            case 3:
                break;
            case 0:
                break;
            default:
                cout << "��ѡ����ڣ�����������!" << endl;
                break;
        }
        if (choose == 0) {

            break;

        }

    }


}

//����Ա�˵�
void printAdminMenu(){
    while (1){


        cout << "------------����Ա����--------------" << endl
             << "1.ͼ������" << endl
             << "2.�����Ŀ" << endl
             << "3.������Ŀ" << endl
             << "4.�û�����" << endl
             << "0.������һ��" << endl
             << "-----------------------------------" << endl
             << "����������ѡ��:";
        int choose;
        cin >> choose;
        cout << "����ѡ����:" << choose << endl;
        switch (choose){
            case 1:
                printSearchMenu();break;
            case 2:

                cout << "��������" << endl;
                break;
            case 3:
                cout << "��������" << endl;
                break;
            case 4:

                while (1){
                    cout << "�������û�����:";
                    int userNum;
                    cin >> userNum;
                    if(userNum == 170200){
                        cout << "���ѽ���" << userNum << "�û��������ģ�" << endl;
                        printUserManageMenu();
                        break;
                    } else{
                        cout << "�û������ڣ����������룡" << endl;
                    }
                }


                break;
            case 5:

            case 0:
                break;
            default:
                cout << "��ѡ����ڣ�����������!" << endl;
        }
        if(choose == 0){
            break;
        }
    }




}

//��ѯ��Ŀͷ
void printSearchResult(){
    cout
            << "----------------------------------------------------------------------------------------------------------------------------------" << endl
            << setiosflags(ios::left) << setw(10) << "���" << setw(20) << "����" << setw(20) << "������" << setw(10) << "����"
            << setw(10) << "����" << setw(20) << "ISBN" << setw(10) << "�۸�" << setw(10) << "����" << setw(10) << "�Ƿ�ɽ�" << endl
            << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void printQueryData(vector<vector<string> > queryData){
    for (ll i = 0; i < queryData.size(); i++) {
        printf("%-10d",i+1);
        for (ll j = 0; j < queryData[0].size(); j++) {

            printf("%s\t", queryData[i][j].data());
        }
        printf("\n");
    }
};
//������ʽ
void printSearchMenu(){

    while (1) {
        DbAdapter *dbHelper = new DbAdapter("��");
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
        cin >> choose;
        cout << "����ѡ����:" << choose << endl;
        string keyWord;
        if(choose != 0){
            cout << "����������������:";
            cin >> keyWord;
        }

        vector<vector<string> > queryData;
        printSearchResult();
        switch (choose){
            case 1:
                cout<< "xxxxxx" << endl;break;
            case 2:
                queryData = dbHelper->searchBySingleField("����", keyWord);
                break;
            case 3:
                queryData = dbHelper->searchBySingleField("����", keyWord);
                printQueryData(queryData);
                break;
            case 4:
                queryData = dbHelper->searchBySingleField("isbn", keyWord);
                printQueryData(queryData);
                break;
            case 5:
                queryData = dbHelper->searchBySingleField("����",keyWord);
                printQueryData(queryData);
                break;
            case 0:
                break;
            default:
                cout << "��ѡ����ڣ�����������!" << endl;
        }
        if(choose == 0){
            break;
        }
        getchar();
        getchar();

    }
}

//�û�����
void printUserManageMenu(){
    while (1) {
        if(offFlag){
            offFlag = false;
            break;
        }
        cout << "------------�û�����--------------" << endl
             << "1.���˶���" << endl
             << "2.����" << endl
             << "3.����" << endl
             << "4.��������" << endl
             << "5.ע��" << endl
             << "0.������һ��" << endl
             << "---------------------------------" << endl
             << "����������ѡ��:";
        int choose;
        cin >> choose;
        cout << "����ѡ����:" << choose << endl;
        switch (choose){
            case 1:
                cout << "���û��Ķ����������:" << endl;
                printSearchResult();
                cout<< "xxxxxx" << endl;break;
            case 2:
                printSearchMenu();
            case 3:
                break;
            case 4:
                cout << "�������óɹ����뾡��֪ͨ���û��޸�����!" << endl;
            case 5:
                while (1){
                    char yn;
                    cout << "��ȷ��Ҫע��ô��(Y/N):";
                    cin >> yn;
                    if(yn == 'Y' || yn == 'y'){
                        offFlag = true;
                        cout << "ע���ɹ���" << endl;
                        break;


                    } else{
                        break;
                    }
                }


            case 0:
                break;
            default:
                cout << "��ѡ����ڣ�����������!" << endl;
        }
        if(choose == 0){
            break;
        }
    }
}

int main(){
    system("mode con cols=140 lines=40");
    init();
    printMainMenu();

    return 0;

}

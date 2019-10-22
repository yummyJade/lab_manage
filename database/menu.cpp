#include "database.h"
#include <string>

using namespace std;

int main() {

    //todo����һ����������?
    //windows �����������Ҫ�л��ַ���
    //system("chcp 65001"); //�л��ַ�??
    system("cls");
    DataBase *d = new DataBase();
    ll opt = 1;
    string chooseTableName = "δѡ��";

    while (opt) {
        chooseTableName = d->getTableName();
        if (chooseTableName == "") chooseTableName = "δѡ��";
        system("cls");
        printf("\
��ѡ�б�%s\n\
----------���ݿ����----------\n\
0.�˳�\n\
1.������\n\
*2.ɾ����\n\
3.ѡ���\n\
4.��ʾ���б�\n\
5.������ݿ�\n\
----------�������----------\n\
6.��������\n\
*7.��ʾ��ǰ��������\n\
8.���ݾ������ֲ�ѯֵ��where��\n\
9.ɾ��\n\
10.��յ�ǰ��\n\
11.��������\n\
12.����id��Χѡ������\n\
*13.��ȡԤ��id\n\
----------�����������-------\n\
*100.����\n\
*101.��֤����\n", chooseTableName.data());
        scanf("%d", &opt);
        //��������
        vector<string> col_name;
        vector<ll> col_size;
        vector<ll> id(0);
        vector<char> isHash;
        vector<vector<string> > insertData(0);
        vector<vector<string> > queryData;
        char s[1000];
        wchar_t sw[1000];
        string name, name2, str, str2;
        char temp_c;
        ll num, num2;
        int code;
        //��������
        switch (opt) {
            case 1:  //����??

                printf("�������:");
                scanf("%s", s);
                name = s;

                printf("��������?:");
                scanf("%lld", &num);

                col_name.resize(num);
                col_size.resize(num);
                isHash.clear();
                isHash.resize(num);
                printf("�������������\n");
                for (ll i = 0; i < num; i++) {

                    scanf("%s", s);
                    col_name[i] = s;

                }
                printf("�����������󳤶�\n");
                for (ll i = 0; i < num; i++) {

                    scanf("%lld", &col_size[i]);
                }
                printf("�������ÿ���ֶ��Ƿ���hash��������������T������������F\n");
                for (ll i = 0; i < num; i++) {
                    ll hashTemp;
                    scanf("%s", s);
                    isHash[i] = s[0];
                }
                printf("��ʼ����......\n");
                d->createTable(name, col_name, col_size, isHash);
                printf("�����ɹ�\n");
                break;
            case 2:
                // printf("����ϣ��ɾ���ı�?:");
                // scanf("%s", s);
                // name = s;
                // printf("��ʼɾ��?......\n");
                // d->deleteTable(name);
                // printf("ɾ���ɹ�\n");
                break;
            case 3:
                printf("������ѡ��ı�:");
                scanf("%s", s);
                name = s;
                code = d->chooseTable(name);
                if (code == -1) {
                    printf("�����ڸñ�\n");
                } else {
                    printf("ѡ��\n");
                }
                break;
            case 4:
                d->showTables();
                break;
            case 5:
                d->clear();
                break;
            case 6: // ??
                if (!d->isChoose()) {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("����ϣ���������������?:");
                scanf("%lld", &num);
                num2 = d->getColNum();

                for (ll i = 0; i < num; i++) {
                    printf("����� %lld �����ݵ� %lld ������\n", i + 1, num2);
                    vector<string> line(num2);
                    for (ll j = 0; j < num2; j++) {
                        scanf("%s", s);
                        line[j] = s;
                        //printf("%s\n", s);
                    }
                    insertData.push_back(line);
                }
                code = d->insert(insertData, id);
                if (code == -1) {
                    printf("δѡ�б�\n");
                    break;
                }
                printf("����Ԫ��id����\n");
                for (ll i = 0; i < id.size(); i++) {
                    printf("%lld\n", id[i]);
                }
                break;
            case 7:
                // if(d->getTableName() == ""){
                // 	printf("��ѡ��һ�ű�\n");
                // 	break;
                // }
                // d->showDatas();
                break;
            case 8:   //??
                if (d->getTableName() == "") {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("�������ֶ���?");
                scanf("%s", s);
                name = s;
                printf("������ֵ��");
                scanf("%s", s);
                name2 = s;
                //��ȡ����
                col_name = d->getColName();
                printf("id\t");
                for (ll i = 0; i < col_name.size(); i++) {
                    printf("%s\t", col_name[i].data());
                }
                printf("\n");
                //��ȡ����

                d->query(name, name2, id, queryData);
                for (ll i = 0; i < queryData.size(); i++) {
                    printf("%lld\t", id[i]);
                    for (ll j = 0; j < queryData[0].size(); j++) {
                        printf("%s\t", queryData[i][j].data());
                    }
                    printf("\n");
                }
                break;
            case 9: //??
                if (!d->isChoose()) {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("�������ֶ���?");
                scanf("%s", s);
                name = s;
                printf("������ֵ��");
                scanf("%s", s);
                name2 = s;
                //��ȡ����

                d->deleteData(name, name2);
                break;
            case 10: //���??
                if (!d->isChoose()) {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                d->clearTable();
                break;
            case 11: //??
                if (!d->isChoose()) {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("�������ѯ�ֶ���\n");
                scanf("%s", s);
                name = s;
                printf("�������ѯֵ\n");
                scanf("%s", s);
                name2 = s;
                printf("�������޸��ֶ���\n");
                scanf("%s", s);
                str = s;
                printf("�������޸ĺ�ֵ\n");
                scanf("%s", s);
                str2 = s;
                d->update(name, name2, str, str2);
                break;
            case 12:  //��id
                if (!d->isChoose()) {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("��������ʼid\n");
                scanf("%lld", &num);
                printf("��������ֹid, ������-1��ΪĬ��ֵ\n");
                scanf("%lld", &num2);
                //��ȡ����
                col_name = d->getColName();
                printf("id\t");
                for (ll i = 0; i < col_name.size(); i++) {
                    printf("%s\t", col_name[i].data());
                }
                printf("\n");
                d->queryById(id, queryData, num, num2);
                for (ll i = 0; i < queryData.size(); i++) {
                    printf("%lld\t", id[i]);
                    for (ll j = 0; j < queryData[0].size(); j++) {
                        printf("%s\t", queryData[i][j].data());
                    }
                    printf("\n");
                }
                break;
            case 13: //Ԥ��
                // if(d->getTableName() == ""){
                // 	printf("��ѡ��һ�ű�\n");
                // 	break;
                // }
                // printf("��������\n");
                // scanf("%lld", &num);
                // d->predictId(num, id);
                // num = id.size();
                // for(int i = 0; i < num; i++){
                // 	printf("%lld\n", id[i]);
                // }
                break;
            case 100:
                // memset(s, 0, sizeof(s));
                // scanf("%s", s);
                // str = s;
                // scanf("%s", s);
                // name = s;
                // if(name == str)
                // 	printf("YES");
                // else
                // 	printf("No");

                // for(int i = 0; i < 10; i++){
                // 	printf("%x ", s[i]);
                // }
                // printf("\n");
                // s[2] = 0;
                // printf("%s\n", s);

                break;


        }
        getchar();
        getchar();
    }
} 
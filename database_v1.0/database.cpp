#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <algorithm>

#define ll long long
using namespace std;

namespace settings {
    string settings_name = "settings.ini";
    string table_settings_name = "table_settings_name.ini";
}
class DataBase {
/*
��д�淶��
1. �κ�size���� ָ���Ƕ��ٸ�char�� sizec ������ٸ� size* sizeof(char) 
2. ���д���Ĳ�����Ӧ�ÿ������� 1�� ������ʹ���߼���
3. �кŴ�1��ʼ����

//�����뺬�壺
0.�ɹ�
-1. δ֪����ʧ��
*/

private:
    ll table_name_max_len;
    ll col_name_max_len;
    ll table_number;
    static DataBase *DBinstance;
    //��ǰ������
    string table_name; //����
    ll table_col_num; //������
    vector<string> table_col_name; //����
    vector<ll> table_col_size; // ��ǰ��ÿ�г���
    vector<ll> table_col_pre_size; //ÿ�б�ǰ׺��
    //todo����һ������ǰ׺������
    //todo����һ���ڴ�أ�ջʽ���ô���-������ʽ
    DataBase() {
        //ȫ������
        this->table_name_max_len = 50;
        this->col_name_max_len = 50;
        this->table_number = 0;
        FILE *fp = fopen(settings::settings_name.data(), "r");
        //��ȡ���б���
        if (fread(&this->table_number, sizeof(ll), 1, fp) == 0) {
            this->table_number = 0;
        }
        //���ñ�������
        //����
        fclose(fp);
        //������
        fp = fopen(settings::table_settings_name.data(), "a");
        fclose(fp);
    }

public:
    /*���ݿⱾ�����*/
    static DataBase *getInstance() {
        return new DataBase;
//        if (DBinstance == NULL) {
//            DBinstance = new DataBase();
//        }
//        return DBinstance;
    }

    void closeDataBase() {
        FILE *fp = fopen("settings", "w");
        fwrite(&this->table_number, sizeof(ll), 1, fp);
        fclose(fp);
    }

    /*����������*/
    //todo�������ж�
    //�������ݿ��¼
    void createTable(string name, vector<string> col_name, vector<ll> col_size) {

        //�������ļ�
        vector<char> s;
        FILE *fp = fopen(settings::table_settings_name.data(), "rb+");
        fseek(fp, 0, SEEK_END);
        char valid = 'T';
        //д����Ч��־λ
        // printf("%d\n", fwrite(&valid, sizeof(char), 1, fp));
        // fwrite(&valid, sizeof(char), 1, fp);
        fwrite(&valid, sizeof(char), 1, fp);
        //д���ļ���
        fwrite(name.data(), sizeof(char), this->table_name_max_len, fp);
        //д������
        ll len = col_size.size();
        fwrite(&len, sizeof(ll), 1, fp);
        //д��ÿһ������
        for (ll i = 0; i < len; i++) {
            fwrite(col_name[i].data(), sizeof(char), this->col_name_max_len, fp);
        }

        //д��ÿһ�г���

        for (ll i = 0; i < len; i++) {
            fwrite(&col_size[i], sizeof(ll), 1, fp);
        }
        fclose(fp);

        //���������ݿ��ļ�
        fp = fopen(name.data(), "w");

        rewind(fp);
        //д���ͷ
        ll nextLocation = -1;
        fwrite(&nextLocation, sizeof(ll), 1, fp);
        fclose(fp);
        this->table_number++;
    }

    void showTables() {
        printf("show\n");
        FILE *fp = fopen(settings::table_settings_name.data(), "rb+");
        fseek(fp, 0, SEEK_SET);
        // ѭ����ȡ
        char valid;
        while ((valid = fgetc(fp)) != EOF) {
            printf("tell:%d\n", ftell(fp));
            //��ȡ��Чλ
            printf("%c ", valid);
            //��ȡ����
            vector<char> s(this->table_name_max_len);
            fread(&s[0], sizeof(char), this->table_name_max_len, fp);
            printf("%s ", &s[0]);
            //��ȡ����
            ll len;
            fread(&len, sizeof(ll), 1, fp);
            printf("%d ", len);
            vector<char> col(this->col_name_max_len + 10);
            for (ll j = 0; j < len; j++) {
                fread(&col[0], sizeof(char), this->col_name_max_len, fp);
                printf("%s ", &col[0]);
            }
            ll t;
            for (ll j = 0; j < len; j++) {
                fread(&t, sizeof(ll), 1, fp);
                printf("%ld ", t);
            }
            printf("\n");
        }

        fclose(fp);
    }

    int deleteTable(string name) {
        FILE *fp = fopen(settings::settings_name.data(), "rb+");
        fseek(fp, 0, SEEK_SET);
        char valid;
        vector<char> s(this->table_name_max_len);
        ll num;
        while ((valid = fgetc(fp)) != EOF) {
            printf("tell:%d\n", ftell(fp));
            if (valid == 'T') {

                fread(&s[0], sizeof(char), this->table_name_max_len, fp);
                string ss(&s[0]);
                printf("��⵽ %s\n", &s[0]);
                if (ss == name) {
                    printf("����ɾ��\n");
                    valid = 'F';
                    fseek(fp, -((this->table_name_max_len + 1) * sizeof(char)), SEEK_CUR);
                    fwrite(&valid, sizeof(char), 1, fp);
                    break;
                }
                fread(&num, sizeof(ll), 1, fp);
                fseek(fp, num * (this->col_name_max_len) * sizeof(char), SEEK_CUR);
                fseek(fp, num * sizeof(ll), SEEK_CUR);
            } else {
                fseek(fp, this->table_name_max_len * sizeof(char), SEEK_CUR);
                fread(&num, sizeof(ll), 1, fp);
                fseek(fp, num * (this->col_name_max_len) * sizeof(char), SEEK_CUR);
                fseek(fp, num * sizeof(ll), SEEK_CUR);
            }
        }
        //fflush(fp);
        fclose(fp);
    }

    //��ȡ���ݿ⵱ǰ����Ϣ
    string getTableName() {
        return this->table_name;
    }

    ll getColNum() {
        return this->table_col_num;
    }

    vector<string> getColName() {
        return this->table_col_name;
    }

    //�������
    //ѡ��� 
    string chooseTable(string name) {
        bool getFlag = false; //�Ƿ��ȡ����־
        FILE *fp = fopen(settings::table_settings_name.data(), "rb");
        fseek(fp, 0, SEEK_SET);
        //��ȡȫ�ֱ���
        ll clen = this->col_name_max_len;
        char valid;
        vector<char> s(this->table_name_max_len);
        ll num;
        while ((valid = fgetc(fp)) != EOF) {
            // printf("choose:%d\n", ftell(fp));
            // �жϸ����Ƿ���Ч
            if (valid == 'T') {
                fread(&s[0], sizeof(char), this->table_name_max_len, fp);
                string ss(&s[0]);
                //�ж��Ƿ�����ͬ����
                if (ss == name) {
//                    printf("�ҵ�������ݱ�\n");
                    this->table_name = name;
                    //��ȡ����
                    fread(&num, sizeof(ll), 1, fp);
                    this->table_col_num = num;
                    //��������
                    vector<string> &cname = this->table_col_name;
                    vector<ll> &csize = this->table_col_size;
                    vector<ll> &csizep = this->table_col_pre_size;
                    //��ʱ����
                    vector<char> temp_c(clen);
                    //��ʼ������
                    cname.resize(this->table_col_num);
                    csize.resize(this->table_col_num);
                    csizep.resize(this->table_col_num + 1);
                    //��ȡÿһ������
                    for (ll i = 0; i < num; i++) {
                        fread(&temp_c[0], sizeof(char), clen, fp);
                        cname[i] = &temp_c[0];
                    }
                    //��ȡÿһ�еĿ��
                    csizep[0] = 0;
                    for (ll i = 0; i < num; i++) {
                        fread(&csize[i], sizeof(ll), 1, fp);
                        //printf("csize:%d\n", csize[i]);
                        csizep[i + 1] = csizep[i] + csize[i];
                    }
                    getFlag = true;
                    break;
                }
                    //��ͬ�������������
                else {
                    ll num;
                    fread(&num, sizeof(ll), 1, fp);
                    //������������ �� �г���
                    fseek(fp, num * sizeof(char) * clen, SEEK_CUR);
                    fseek(fp, num * sizeof(ll), SEEK_CUR);
                }
            } else {
                ll num;
                fseek(fp, sizeof(char) * this->table_name_max_len, SEEK_CUR);
                fread(&num, sizeof(ll), 1, fp);
                fseek(fp, num * sizeof(char) * clen, SEEK_CUR);
                fseek(fp, num * sizeof(ll), SEEK_CUR);
            }
            //todo��������Ч���δ����
            //todo��δ�ҵ����δ����
        }
        fclose(fp);
        if (getFlag) {
//            cout<<"������"<<endl;
//            printf("%s", this->table_name.data());
            return name;
        } else {
//            cout<<"������"<<endl;
            return "δ����";
        }
    }

    //��
    int insert(vector<vector<string> > s) {
        //todo�����һ������
        printf("start insert\n");



        //�õ���������
        ll cnum = this->table_col_num; //����
        char valid = 'T';
        vector<ll> &csize = this->table_col_size;
        vector<ll> &cpsize = this->table_col_pre_size;
        FILE *fp = fopen(this->table_name.data(), "rb+");
        rewind(fp);

        //��ȡ��ͷ
        ll nextLocation;
        fread(&nextLocation, sizeof(ll), 1, fp);
        ll n = s.size();
        for (int i = 0; i < n; i++) {
            //ÿ�ζ�ȡ��ͷ
            if (nextLocation == -1) {
                //�����ļ�ĩβ
                fseek(fp, 0, SEEK_END);
                //д���־λ
                fwrite(&valid, sizeof(char), 1, fp);

                //д����б�λ
                nextLocation = -1;
                fwrite(&nextLocation, sizeof(ll), 1, fp);

                ll m = this->table_col_num;

                for (ll j = 0; j < m; j++) {
                    fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
                }


            } else { //�п�����
                //�ӿ�ʼ��ת������
                int temp;
                fseek(fp, nextLocation + sizeof(ll), SEEK_SET);
                // temp = ftell(fp);
                // printf("%d\n", temp);
                //д���־λ
                fwrite(&valid, sizeof(char), 1, fp);
                fflush(fp);
                // temp = ftell(fp);
                // printf("%d\n", temp);
                //��ȡ������nextֵ(�������)
                //ͬʱ��дҪ�ض�λ���������
                // fseek(fp, ftell(fp), SEEK_CUR);
                fread(&nextLocation, sizeof(ll), 1, fp);
                // temp = ftell(fp);
                // printf("%d\n", temp);
                // fseek(fp, ftell(fp)-sizeof(ll), SEEK_CUR);
                // temp = ftell(fp);
                // printf("%d\n", temp);
                ll tempNum = -1;
                fseek(fp, -sizeof(ll), SEEK_CUR);
                fwrite(&tempNum, sizeof(ll), 1, fp);
                //д������
                ll m = this->table_col_num;
                for (ll j = 0; j < m; j++) {
                    fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
                }
            }
        }
        //�����ļ�ͷ
        rewind(fp);
        //�����һ�λ�õ�nextֵд��ͷ��
        fwrite(&nextLocation, sizeof(ll), 1, fp);
        fclose(fp);
        printf("insert end\n");

        // ll cnum = this->table_col_num;
        // vector <ll> &csize = this->table_col_size;
        // FILE * fp = fopen(this->table_name.data() , "a");
        // // printf("name:%s\n", this->table_name.data());
        // ll line_num = s.size();
        // char valid = 'T';
        // for(ll i = 0; i < line_num; i++){
        // 	//д��������Чλ
        // 	fwrite(&valid, sizeof(char), 1, fp);
        // 	for(ll j = 0; j < cnum; j++){
        // 		printf("%d %d %s %d\n", i, j, s[i][j].data(), csize[j]);
        // 		fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
        // 	}
        // }
        // fclose(fp);
    }

    void showDatas(string name = "") {
        if (name != "")
            this->chooseTable(name);
        name = this->table_name;

        //��������
        char valid;
        ll nextLocation;
        ll idx = 0;
        ll totalLen = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num] * sizeof(char);

        FILE *fp = fopen(this->table_name.data(), "rb+");
        rewind(fp);
        fread(&nextLocation, sizeof(ll), 1, fp);

        printf("��ͷ��%lld  id:%lld\n", nextLocation, nextLocation / totalLen);


        //��ӡ��ͷ
        printf("��Чλ\t");
        printf("id\t");
        printf("nextLocation\t");
        for (ll i = 0; i < this->table_col_num; i++) {
            printf("%s\t", this->table_col_name[i].data());
        }
        printf("\n");
        //ȷ����ֶγ���
        ll maxLen = 0;
        for (ll i = 0; i < this->table_col_num; i++) {
            maxLen = max(maxLen, this->table_col_size[i]);
        }
        vector<char> s(maxLen);
        string str;
        //��ȡ����ʾÿһ��
        while ((valid = fgetc(fp)) != EOF) {
            idx++;
            //printf("tell:%d\n", ftell(fp));
            //��ȡ��Чλ
            printf("%c\t", valid);
            printf("%lld\t", idx);
            //��ȡ����λ
            fread(&nextLocation, sizeof(ll), 1, fp);
            printf("%lld nextId:%lld\t", nextLocation, nextLocation / totalLen);
            //��ȡ����ÿһλ���ݲ���ӡ
            for (ll i = 0; i < this->table_col_num; i++) {
                fread(&s[0], sizeof(char), this->table_col_size[i], fp);

                printf("%s\t", &s[0]);
            }
            printf("\n");
        }

        fclose(fp);
    }

    //ɾ
    void deleteData(string key, string value, string name = "") {
        //���id
        vector<ll> id(0);
        vector<vector<string> > ans;
        this->query(key, value, id, ans);
        //��ʼ������
        ll n = id.size();
        ll len = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num] * sizeof(char);
        char valid = 'F';
        ll nextLocation = 0;
        ll headLocation = 0;
        //���ļ�
        FILE *fp = fopen(this->table_name.data(), "rb+");
        fseek(fp, 0, SEEK_SET);
        fread(&headLocation, sizeof(ll), 1, fp);
        for (ll i = 0; i < n; i++) {
            printf("tell%lld id:%lld\n", headLocation, id[i]);
            //�ӿ�ͷ��ת��ָ��λ��
            fseek(fp, len * (id[i] - 1) + sizeof(ll), SEEK_SET);
            fwrite(&valid, sizeof(char), 1, fp);
            fwrite(&headLocation, sizeof(ll), 1, fp);
            headLocation = (id[i] - 1) * len;
        }
        rewind(fp);
        //������һ��ͨ���ļ�ͷ����
        fwrite(&headLocation, sizeof(ll), 1, fp);
        fclose(fp);
    }

    // ��
    bool query(string key, string value, vector<ll> &id, vector<vector<string> > &ans, string name = "") {
        if (name != "")
            this->chooseTable(name);
        name = this->table_name;

        //��ȡ��������
        ll idx, selfLen, preSeek, lastSeek, totalSeek;
        if (!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)) {
            //���޴��ֶ�
            return false;
        }
        //��ʼ������
        id.resize(0); //����id����
        ll lineNum = 0; //��¼��ǰ�кţ�����id
        ll maxLen = 0;
        for (ll i = 0; i < this->table_col_num; i++) {
            maxLen = max(maxLen, this->table_col_size[i]);
        }
        vector<char> s(maxLen + 10);  //��ʱ����
        string str;  //��ʱ����


        vector<ll> &csize = this->table_col_size;
        //���ļ���ѯ
        FILE *fp = fopen(name.data(), "rb");
        //����ͷ��
        fseek(fp, sizeof(ll), SEEK_SET);
        char valid;
        while ((valid = fgetc(fp)) != EOF) {
            lineNum++;
            //����������Ч
            if (valid == 'T') {
                //��������λ
                fseek(fp, sizeof(ll), SEEK_CUR);
                //����ǰ�����ݿ�
                fseek(fp, preSeek, SEEK_CUR);
                //��ȡ���ݲ�����string
                fread(&s[0], sizeof(char), selfLen, fp);
                str = &s[0];
                // ������ȷ
                if (str == value) {
                    // printf("%s������ͬ", str.data());
                    //�ҵ���ǰ��Ծ
                    fseek(fp, -(preSeek + selfLen * sizeof(char)), SEEK_CUR);
                    //��ȡ��������
                    vector<string> line(this->table_col_num); //һ�н��
                    for (ll i = 0; i < this->table_col_num; i++) {
                        fread(&s[0], sizeof(char), csize[i], fp);
                        line[i] = &s[0];
                    }
                    //vector�Ŀ��������
                    ans.push_back(line);
                    id.push_back(lineNum);
                } else {
                    //�������沿��
                    fseek(fp, lastSeek, SEEK_CUR);
                }
            } else {
                fseek(fp, sizeof(ll), SEEK_CUR);
                fseek(fp, totalSeek, SEEK_CUR);
            }
        }
        return true;
    }

    //��
    int update(string key, string value, string key2, string value2, string name = "") {
        vector<ll> id(0);
        vector<vector<string> > ans;
        this->query(key, value, id, ans);
        ll n = id.size();
        ll len = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num] * sizeof(char);
        ll idx, selfLen, preSeek, lastSeek, totalSeek;
        if (!this->getKeyLocation(key2, idx, selfLen, preSeek, lastSeek, totalSeek)) {
            //���޴��ֶ�
            return -1;
        }
        //�����ļ�����
        FILE *fp = fopen(this->table_name.data(), "rb+");
        for (int i = 0; i < n; i++) {
            //������ͷ
            fseek(fp, sizeof(ll), SEEK_SET);
            //��ת������
            fseek(fp, (id[i] - 1) * (totalSeek + sizeof(ll) + sizeof(char)), SEEK_CUR);
            //������Чλ������λ
            fseek(fp, sizeof(char) + sizeof(ll), SEEK_CUR);
            //������������λ��

            fseek(fp, preSeek, SEEK_CUR);
            //д��������

            fwrite(value2.data(), sizeof(char), selfLen, fp);
            //fflush(fp);
        }
        fclose(fp);
        return 0;
    }

    //��ѯĳ���ֶ��������ݶε�λ��(��������־λ�Ϳ�������λ)
    bool getKeyLocation(string key, ll &idx, ll &selfLen, ll &preSeek, ll &lastSeek, ll &totalSeek) {
        idx = -1;
        for (ll i = 0; i < this->table_col_num; i++) {
            if (this->table_col_name[i] == key) {
                idx = i;
            }
        }
        if (idx == -1) return false; //�ֶβ�����
        //����ǰ�����ݳ���
        selfLen = this->table_col_size[idx];
        preSeek = (this->table_col_pre_size[idx] - this->table_col_pre_size[0]) * sizeof(char);
        lastSeek = (this->table_col_pre_size[this->table_col_num] - this->table_col_pre_size[idx + 1]) * sizeof(char);
        totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //�����ܳ�
        return true;
    }

    //������ݿ�
    void emptyDataBase() {
        FILE *fp = fopen(settings::settings_name.data(), "wb");
        fclose(fp);
        fp = fopen(settings::table_settings_name.data(), "wb");
        fclose(fp);
    }

    //������ݱ�
    void emptyTable() {
        ll nextLocation = -1;
        FILE *fp = fopen(this->table_name.data(), "w");
        fwrite(&nextLocation, sizeof(ll), 1, fp);
        fclose(fp);
    }


};

//Ī������ľ�̬˽�б�����ʼ��
DataBase *DataBase::DBinstance = NULL;

int main2() {
    //todo����һ����������
    //windows �����������Ҫ�л��ַ���
    //system("chcp 65001"); //�л��ַ���
    DataBase *d = DataBase::getInstance();
    ll opt = 1;
    string chooseTableName = "δѡ��";
    while (opt) {
        system("cls");
        printf("\
��ѡ�б�%s\n\
----------���ݿ����----------\n\
0.�˳� \n\
1.������\n\
2.ɾ����\n\
3.ѡ���\n\
4.��ʾ���б�\n\
5.������ݿ�\n\
----------�������----------\n\
6.��������\n\
7.��ʾ��ǰ��������\n\
8.���ݾ������ֲ�ѯֵ��where��\n\
9.ɾ��\n\
10.��յ�ǰ��\n\
11.��������\n\
----------�����������-------\n\
100.����\n", chooseTableName.data());
        scanf("%d", &opt);
        //��������
        vector<string> col_name;
        vector<ll> col_size;
        vector<ll> id(0);
        vector<vector<string> > insertData(0);
        vector<vector<string> > queryData;
        char s[1000];
        wchar_t sw[1000];
        string name, name2, str, str2;
        ll num, num2;
        //��������
        switch (opt) {
            case 1:

                printf("�������:");
                scanf("%s", s);
                name = s;

                printf("����������:");
                scanf("%d", &num);

                col_name.resize(num);
                col_size.resize(num);
                printf("�������������\n");
                for (ll i = 0; i < num; i++) {

                    scanf("%s", s);
                    col_name[i] = s;

                }
                printf("�����������󳤶�\n");
                for (ll i = 0; i < num; i++) {

                    scanf("%d", &col_size[i]);
                }
                printf("��ʼ����......\n");
                d->createTable(name, col_name, col_size);
                printf("�����ɹ�\n");
                break;
            case 2:
                printf("����ϣ��ɾ���ı���:");
                scanf("%s", s);
                name = s;
                printf("��ʼɾ��......\n");
                d->deleteTable(name);
                printf("ɾ���ɹ�\n");
                break;
            case 3:
                printf("������ѡ��ı���:");
                scanf("%s", s);
                name = s;
                chooseTableName = d->chooseTable(name);
                break;
            case 4:
                d->showTables();
                break;
            case 5:
                d->emptyDataBase();
                break;
            case 6:
                if (d->getTableName() == "") {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("����ϣ���������������:");
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
                d->insert(insertData);
                break;
            case 7:
                if (d->getTableName() == "") {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                d->showDatas();
                break;
            case 8:   //��
                if (d->getTableName() == "") {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("�������ֶ�����");
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
            case 9: //ɾ
                if (d->getTableName() == "") {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                printf("�������ֶ�����");
                scanf("%s", s);
                name = s;
                printf("������ֵ��");
                scanf("%s", s);
                name2 = s;
                //��ȡ����

                d->deleteData(name, name2);
                break;
            case 10: //��ձ�
                if (d->getTableName() == "") {
                    printf("��ѡ��һ�ű�\n");
                    break;
                }
                d->emptyTable();
                break;
            case 11: //��
                if (d->getTableName() == "") {
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
            case 100:
                memset(s, 0, sizeof(s));
                scanf("%s", s);
                str = s;
                scanf("%s", s);
                name = s;
                if (name == str)
                    printf("YES");
                else
                    printf("No");

                for (int i = 0; i < 10; i++) {
                    printf("%x ", s[i]);
                }
                printf("\n");
                s[2] = 0;
                printf("%s\n", s);

                break;


        }
        getchar();
        getchar();
    }
    d->closeDataBase();
//	vector<string> s(2);
//	s[0] = "123";
//	s[1] = "456";
//	vector<ll> t(2);
//	t[0] = 1;
//	t[1] = 2;
//	d->createTable("testTable", s, t);
//	d->createTable("testTable2", s, t);
//	d->deleteTable("testTable2");
//	d->showColumLine("testTable");
//	d->closeDataBase();
//	d->chooseTable("tantan");
//	d->showTables();
//	vector<vector<string> > s(1);
//	vector<string> t(2);
//	t[0] = "1";
//	t[1] = "2";
//	s[0] = t;
//	d->insert(s);
}

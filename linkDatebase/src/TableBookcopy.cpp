#include "../../linkDatebase/include/TableBookcopy.h"
#include "../../linkDatebase/include/IndexRecord.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
using namespace std;
TableBookcopy *TableBookcopy::tableBookcopy = NULL;

void writeBookcopy(int head_size, int data_size, Bookcopy bookcopy, int id) {
    /*����˵����
    head_size���ļ�ͷ����
    data_size��ÿ����¼�ĳ���
    id��������¼Ҫд���ļ��еڶ����У���������һ��������-1���ǲ����ļ�ĩβ
    */
    //ע�⣺�ú���δУ��id�Ƿ񳬹��˵�ǰ�ļ�¼��Ŀ
    FILE *fp = fopen("table_bookcopy", "rb+");
    if (id == -1) {
        fseek(fp, 0, SEEK_END);
    } else if (id > 0) {
        fseek(fp, head_size + (id - 1) * data_size, SEEK_SET);
    }
    int temp1[4];
    string temp2[2];
    temp1[0] = bookcopy.getId();
    temp1[1] = bookcopy.getState();
    temp1[2] = bookcopy.getReTime();
    temp1[3] = bookcopy.getNextId();
    temp2[0] = bookcopy.getIsbn();
    temp2[1] = bookcopy.getPosition();
    fwrite(temp1, sizeof(int), 4, fp);
    fwrite(temp2[0].data(), sizeof(char), 15, fp);
    fwrite(temp2[1].data(), sizeof(char), 40, fp);
    fclose(fp);
}

Bookcopy readBookcopy(int head_size, int data_size, int id) {
    /*����˵����
    head_size���ļ�ͷ����
    data_size��ÿ����¼�ĳ���
    id������ȡ�ļ�¼���ļ��еĵڼ�����¼*/
    FILE *fp = fopen("table_bookcopy", "rb");
    fseek(fp, head_size + (id - 1) * data_size, SEEK_SET);              //�ҵ���id����¼����ʼ��ַ
    Bookcopy bookcopy;
    int temp1[4];
    char temp2[15];
    char temp3[40];
    fread(&temp1, sizeof(int), 4, fp);
    //cout << "fine" << endl;
    fread(&temp2, sizeof(char), 15, fp);
    fread(&temp3, sizeof(char), 40, fp);
    //cout << temp2 << temp3 << "enmmmmm" << endl;
    fclose(fp);
    bookcopy.setId(temp1[0]);
    bookcopy.setState(temp1[1]);
    bookcopy.setReTime(temp1[2]);
    bookcopy.setNextId(temp1[3]);
    bookcopy.setIsbn(temp2);
    bookcopy.setPosition(temp3 + '\0');
    return bookcopy;
}

TableBookcopy::~TableBookcopy() {
    //dtor
}

TableBookcopy::TableBookcopy() {
    //˽�й��캯��
    //this->table_name = "table_bookcopy";
    FILE *fp = fopen("table_bookcopy", "rb+");
    if (fp == NULL) {
        fp = fopen("table_bookcopy", "wb+");
    }
    fseek(fp, 0, SEEK_SET);                     //��ָ���ƶ����ļ�ͷ��
    bool flag = false;
    //��ȡ�ļ�ͷ�����в���
    if (fread(&this->data_size, sizeof(int), 1, fp) == 0) {
        this->data_size = sizeof(int) * 4 + sizeof(char) * 55;
        flag = true;
    }
    if (fread(&this->data_number, sizeof(int), 1, fp) == 0) {
        this->data_number = 0;
        flag = true;
    }
    this->head_size = sizeof(int) * 2;
    if (flag) {
        rewind(fp);
        fwrite(&this->data_size, sizeof(int), 1, fp);
        fwrite(&this->data_number, sizeof(int), 1, fp);
    }
    fclose(fp);
}

TableBookcopy *TableBookcopy::getInstance() {
    if (tableBookcopy == NULL) {
        tableBookcopy = new TableBookcopy;
    }
    return tableBookcopy;
}

int TableBookcopy::insertData(int id, Bookcopy bookcopy, int num) {
    /*
    ����˵����
        id: �����������׸�������id�����ڴ˴β���ִ��ǰ����û����ظ������ò�����-1
        bookcopy: �������ĸ����������Ϣ���ڴ�Bean����
        num: ���������Ҫ������ٱ�
    ����ֵ˵����
        ���ظ����������׸�������id
    */
    if (id > this->data_number || (id != -1 && id <= 0)) {                   //�������
        //�����id���ݲ�����condition3��ֱ�ӷ���-1
        return -1;
    }
    vector<Bookcopy> v;

    //�����м�¼�޸ĺ÷���һ����̬������
    for (int i = 1; i <= num; i++) {
        bookcopy.setId(this->data_number + i);
        if (i == num) {
            //���⴦��һ�����һ����¼
            if (id == -1) {
                bookcopy.setNextId(-1);
            } else {
                Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
                bookcopy.setNextId(temp.getNextId());
            }
        } else {
            bookcopy.setNextId(this->data_number + i + 1);
        }
        v.push_back(bookcopy);
    }

    FILE *fp = fopen("table_bookcopy", "rb+");
    fseek(fp, 0, SEEK_END);
    for (int i = 0; i < (int) v.size(); i++) {
        int temp1[4];
        string temp2[2];
        temp1[0] = v[i].getId();
        temp1[1] = v[i].getState();
        temp1[2] = v[i].getReTime();
        temp1[3] = v[i].getNextId();
        temp2[0] = v[i].getIsbn();
        temp2[1] = v[i].getPosition();
        fwrite(temp1, sizeof(int), 4, fp);
        fwrite(temp2[0].data(), sizeof(char), 15, fp);
        fwrite(temp2[1].data(), sizeof(char), 40, fp);
    }
    fclose(fp);

    //�������id����-1����ȥ����һ��id����Ӧ����һ�е�����
    if (id != -1) {
        //��Щ�������Ǹ���ĵ�һ��������Ӧ�ð�������
        Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
        temp.setNextId(this->data_number + 1);
        writeBookcopy(this->head_size, this->data_size, temp, id);
    }
    this->data_number = this->data_number + num;
    fp = fopen("table_bookcopy", "rb+");
    fseek(fp, sizeof(int), SEEK_SET);                       //�����ļ�ͷ�ϵļ�¼����
    int number = this->data_number;
    fwrite(&number, sizeof(int), 1, fp);
    fclose(fp);
    return (id == -1) ? (this->data_number - num + 1) : (id);                     //��������ز�ͬ�ķ���ֵ
}

vector<Bookcopy> TableBookcopy::query(int id) {
    //��id��ѯһ������
    vector<Bookcopy> result;
    if (id > this->data_number) {
        return result;
    }
    Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
    result.push_back(temp);
    return result;
}

int TableBookcopy::update(int id, Bookcopy bookcopy, vector<int> reviseField) {
    //��id�޸�һ������
    if (id > this->data_number) {
        return -1;
    }
    Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
    int t[7] = {0};                                                     //�ñ���6���ֶΣ��ֶα�Ŵ�1��ʼ
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 6)
            t[reviseField[i]] = 1;
    }
    if (t[1]) {
        //id
        temp.setId(bookcopy.getId());
    }
    if (t[2]) {
        //state
        temp.setState(bookcopy.getState());
    }
    if (t[3]) {
        //reTime
        temp.setReTime(bookcopy.getReTime());
    }
    if (t[4]) {
        //nextId
        temp.setNextId(bookcopy.getNextId());
    }
    if (t[5]) {
        //isbn
        temp.setIsbn(bookcopy.getIsbn());
    }
    if (t[6]) {
        //position
        temp.setPosition(bookcopy.getPosition());
    }
    writeBookcopy(this->head_size, this->data_size, temp, id);
    return 1;
}

int TableBookcopy::update(int id, Bookcopy bookcopy) {
    //��id�޸�һ������
    if (id > this->data_number) {
        return -1;
    }
    Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
    temp.setId(bookcopy.getId());
    temp.setState(bookcopy.getState());
    temp.setReTime(bookcopy.getReTime());
    temp.setNextId(bookcopy.getNextId());
    temp.setIsbn(bookcopy.getIsbn());
    temp.setPosition(bookcopy.getPosition());
    writeBookcopy(this->head_size, this->data_size, temp, id);
    return 1;
}

vector<int> TableBookcopy::updateByBookId(int id, Bookcopy bookcopy, vector<int> reviseField) {
    //���������޸�
    vector<int> result;             //�ö�̬�������ڴ�ŷ��ؽ�����޸�����Щid�ļ�¼��
    if (id > this->data_number) {
        return result;
    }
    int t[7] = {0};                                                     //�ñ���6���ֶΣ��ֶα�Ŵ�1��ʼ
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 6)
            t[reviseField[i]] = 1;
    }
    while (id != -1) {
        //id!=-1˵������δ��ĩβ
        Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
        int now = id;
        result.push_back(id);
        id = temp.getNextId();
        if (t[1]) {
            //id
            temp.setId(bookcopy.getId());
        }
        if (t[2]) {
            //state
            temp.setState(bookcopy.getState());
        }
        if (t[3]) {
            //reTime
            temp.setReTime(bookcopy.getReTime());
        }
        if (t[4]) {
            //nextId
            temp.setNextId(bookcopy.getNextId());
        }
        if (t[5]) {
            //isbn
            temp.setIsbn(bookcopy.getIsbn());
        }
        if (t[6]) {
            //position
            temp.setPosition(bookcopy.getPosition());
        }
        writeBookcopy(this->head_size, this->data_size, temp, now);
    }
    return result;
}

vector<int> TableBookcopy::updateByBookId(int id, Bookcopy bookcopy) {
    //���������޸�
    vector<int> result;             //�ö�̬�������ڴ�ŷ��ؽ�����޸�����Щid�ļ�¼��
    if (id > this->data_number) {
        return result;
    }
    while (id != -1) {
        //id!=-1˵������δ��ĩβ
        Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
        int now = id;
        id = temp.getNextId();
        result.push_back(id);
        temp.setId(bookcopy.getId());
        temp.setState(bookcopy.getState());
        temp.setReTime(bookcopy.getReTime());
        temp.setNextId(bookcopy.getNextId());
        temp.setIsbn(bookcopy.getIsbn());
        temp.setPosition(bookcopy.getPosition());
        writeBookcopy(this->head_size, this->data_size, temp, now);
    }
    return result;
}

vector<Bookcopy> TableBookcopy::queryByBookId(int id) {
    //�������в�ѯ
    vector<Bookcopy> result;
    if (id > this->data_number) {
        return result;
    }
    while (id != -1) {
        Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
        result.push_back(temp);
        id = temp.getNextId();
    }
    return result;
}

#include "../../linkDatebase/include/TableBookcopy.h"
#include "../../linkDatebase/include/IndexRecord.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
using namespace std;
TableBookcopy *TableBookcopy::tableBookcopy = NULL;

void writeBookcopy(int head_size, int data_size, Bookcopy bookcopy, int id) {
    /*参数说明：
    head_size：文件头长度
    data_size：每条记录的长度
    id：该条记录要写到文件中第多少行（覆盖在哪一条），填-1则是插在文件末尾
    */
    //注意：该函数未校验id是否超过了当前的记录数目
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
    /*参数说明：
    head_size：文件头长度
    data_size：每条记录的长度
    id：待读取的记录是文件中的第几条记录*/
    FILE *fp = fopen("table_bookcopy", "rb");
    fseek(fp, head_size + (id - 1) * data_size, SEEK_SET);              //找到第id条记录的起始地址
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
    //私有构造函数
    //this->table_name = "table_bookcopy";
    FILE *fp = fopen("table_bookcopy", "rb+");
    if (fp == NULL) {
        fp = fopen("table_bookcopy", "wb+");
    }
    fseek(fp, 0, SEEK_SET);                     //将指针移动到文件头处
    bool flag = false;
    //读取文件头中已有参数
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
    参数说明：
        id: 该种类的书的首个副本的id，若在此次插入执行前表内没有相关副本，该参数填-1
        bookcopy: 待创建的副本的相关信息放在此Bean类中
        num: 该种类的书要插入多少本
    返回值说明：
        返回该种类的书的首个副本的id
    */
    if (id > this->data_number || (id != -1 && id <= 0)) {                   //参数检查
        //输入的id数据不合理，condition3，直接返回-1
        return -1;
    }
    vector<Bookcopy> v;

    //将所有记录修改好放入一个动态数组中
    for (int i = 1; i <= num; i++) {
        bookcopy.setId(this->data_number + i);
        if (i == num) {
            //特殊处理一下最后一条记录
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

    //如果参数id不是-1，再去调整一下id所对应的哪一行的数据
    if (id != -1) {
        //这些副本不是该书的第一批副本，应该把链链上
        Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
        temp.setNextId(this->data_number + 1);
        writeBookcopy(this->head_size, this->data_size, temp, id);
    }
    this->data_number = this->data_number + num;
    fp = fopen("table_bookcopy", "rb+");
    fseek(fp, sizeof(int), SEEK_SET);                       //调整文件头上的记录数量
    int number = this->data_number;
    fwrite(&number, sizeof(int), 1, fp);
    fclose(fp);
    return (id == -1) ? (this->data_number - num + 1) : (id);                     //分情况返回不同的返回值
}

vector<Bookcopy> TableBookcopy::query(int id) {
    //按id查询一条数据
    vector<Bookcopy> result;
    if (id > this->data_number) {
        return result;
    }
    Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
    result.push_back(temp);
    return result;
}

int TableBookcopy::update(int id, Bookcopy bookcopy, vector<int> reviseField) {
    //按id修改一条数据
    if (id > this->data_number) {
        return -1;
    }
    Bookcopy temp = readBookcopy(this->head_size, this->data_size, id);
    int t[7] = {0};                                                     //该表共有6个字段，字段编号从1开始
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
    //按id修改一条数据
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
    //按链进行修改
    vector<int> result;             //该动态数组用于存放返回结果（修改了哪些id的记录）
    if (id > this->data_number) {
        return result;
    }
    int t[7] = {0};                                                     //该表共有6个字段，字段编号从1开始
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 6)
            t[reviseField[i]] = 1;
    }
    while (id != -1) {
        //id!=-1说明该链未到末尾
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
    //按链进行修改
    vector<int> result;             //该动态数组用于存放返回结果（修改了哪些id的记录）
    if (id > this->data_number) {
        return result;
    }
    while (id != -1) {
        //id!=-1说明该链未到末尾
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
    //按链进行查询
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

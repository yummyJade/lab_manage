#include "../../linkDatebase/include/TableRecord.h"
#include "../../linkDatebase/include/IndexRecord.h"
#include <iostream>

using namespace std;
IndexRecord *IndexRecord::indexRecord = NULL;                       //静态变量初始化
TableRecord *TableRecord::tableRecord = NULL;

void writeRecord(int head_size, int data_size, Record record, int id) {
    /*参数说明：
    head_size：文件头长度
    data_size：每条记录的长度
    id：该条记录要写到文件中第多少行（覆盖在哪一条），填-1则是插在文件末尾
    */
    //注意：该函数未校验id是否超过了当前的记录数目
    FILE *fp = fopen("table_record", "rb+");
    if (id == -1) {
        fseek(fp, 0, SEEK_END);
    } else if (id > 0) {
        fseek(fp, head_size + (id - 1) * data_size, SEEK_SET);
    }
    long long temp1[2];
    int temp2[5];
    temp1[0] = record.getStId();
    temp1[1] = record.getBoTime();
    temp2[0] = record.getNextId();
    temp2[1] = record.getId();
    temp2[2] = record.getBookId();
    temp2[3] = record.getReTime();
    temp2[4] = record.getState();
    fwrite(temp1, sizeof(long long), 2, fp);
    fwrite(temp2, sizeof(int), 5, fp);
    fclose(fp);
}

Record readRecord(int head_size, int data_size, int id) {
    /*参数说明：
    head_size：文件头长度
    data_size：每条记录的长度
    id：待读取的记录是文件中的第几条记录*/
    FILE *fp = fopen("table_record", "rb");
    fseek(fp, head_size + (id - 1) * data_size, SEEK_SET);              //找到第id条记录的起始地址
    Record record;
    long long temp1[2];
    int temp2[5];
    fread(&temp1, sizeof(long long), 2, fp);
    fread(&temp2, sizeof(int), 5, fp);
    fclose(fp);
    record.setStId(temp1[0]);
    record.setBoTime(temp1[1]);
    record.setNextId(temp2[0]);
    record.setId(temp2[1]);
    record.setBookId(temp2[2]);
    record.setReTime(temp2[3]);
    record.setState(temp2[4]);
    return record;
}

TableRecord::~TableRecord() {
    //dtor
}

TableRecord::TableRecord() {
    //私有构造函数
    //this->table_name = "table_record";
    FILE *fp = fopen("table_record", "rb+");
    if (fp == NULL) {
        fp = fopen("table_record", "wb+");
    }
    fseek(fp, 0, SEEK_SET);                     //将指针移动到文件头处
    bool flag = false;
    //读取文件头中已有参数
    if (fread(&this->data_size, sizeof(int), 1, fp) == 0) {
        this->data_size = sizeof(long long) * 2 + sizeof(int) * 5;
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

TableRecord *TableRecord::getInstance() {
    if (tableRecord == NULL) {
        tableRecord = new TableRecord;
    }
    return tableRecord;
}

int TableRecord::insertData(int id, Record record) {
    ///插入一条数据
    if (id > this->data_number || (id != -1 && id <= 0)) {                   //参数检查
        //输入的id数据不合理，condition3，直接返回-1
        return -1;
    }
    record.setId(this->data_number + 1);
    if (id == -1) {
        record.setNextId(-1);
        writeRecord(this->head_size, this->data_size, record, -1);                        //调用函数将记录插入数据库中
    } else {
        //该条记录非该名学生的第一条借阅记录，应该把链链上
        Record temp = readRecord(this->head_size, this->data_size, id);
        int nextId = temp.getNextId();
        temp.setNextId(this->data_number + 1);
        writeRecord(this->head_size, this->data_size, temp, id);
        record.setNextId(nextId);
        writeRecord(this->head_size, this->data_size, record, -1);                        //调用函数将记录插入数据库中
    }
    this->data_number++;
    FILE *fp = fopen("table_record", "rb+");
    fseek(fp, sizeof(int), SEEK_SET);                       //调整文件头上的记录数量
    int number = this->data_number;
    fwrite(&number, sizeof(int), 1, fp);
    fclose(fp);
    return (id == -1) ? (this->data_number) : (id);                     //分情况返回不同的返回值
}

int TableRecord::update(int id, Record record, vector<int> reviseField) {
    //按id修改一条数据
    if (id > this->data_number) {
        return -1;
    }
    Record temp = readRecord(this->head_size, this->data_size, id);
    int t[8] = {0};                                                     //该表共有7个字段，字段编号从1开始
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 7)
            t[reviseField[i]] = 1;
    }
    if (t[1]) {
        //stId
        temp.setStId(record.getStId());
    }
    if (t[2]) {
        //boTime
        temp.setBoTime(record.getBoTime());
    }
    if (t[3]) {
        //nextId
        temp.setNextId(record.getNextId());
    }
    if (t[4]) {
        //id
        temp.setId(record.getId());
    }
    if (t[5]) {
        //bookId
        temp.setBookId(record.getBookId());
    }
    if (t[6]) {
        //reTime
        temp.setReTime(record.getReTime());
    }
    if (t[7]) {
        //state
        temp.setState(record.getState());
    }
    writeRecord(this->head_size, this->data_size, temp, id);
    return 1;
}

int TableRecord::update(int id, Record record) {
    //按id修改一条数据
    if (id > this->data_number) {
        return -1;
    }
    Record temp = readRecord(this->head_size, this->data_size, id);
    temp.setStId(record.getStId());
    temp.setBoTime(record.getBoTime());
    temp.setNextId(record.getNextId());
    temp.setId(record.getId());
    temp.setBookId(record.getBookId());
    temp.setReTime(record.getReTime());
    temp.setState(record.getState());
    writeRecord(this->head_size, this->data_size, temp, id);
    return 1;
}

vector<Record> TableRecord::query(int id) {
    //按id查询一条数据
    vector<Record> result;
    if (id > this->data_number) {
        return result;
    }
    Record temp = readRecord(this->head_size, this->data_size, id);
    result.push_back(temp);
    return result;
}

vector<int> TableRecord::updateByPerson(int id, Record record, vector<int> reviseField) {
    //按借阅者的id进行数据修改
    /*参数说明：
    id:该借阅者的首条借阅记录的id
    reviseField:要修改的字段
    record:要改成的值放在record里面
    */
    vector<int> result;                             //该动态数组用于存放返回结果（修改了哪些id的记录）
    if (id > this->data_number) {
        return result;
    }
    int t[8] = {0};                                                     //该表共有7个字段，字段编号从1开始
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 7)
            t[reviseField[i]] = 1;
    }
    while (id != -1) {
        //id!=-1说明该链未到末尾
        Record temp = readRecord(this->head_size, this->data_size, id);
        result.push_back(id);
        int now = id;
        id = temp.getNextId();
        if (t[1]) {
            //stId
            temp.setStId(record.getStId());
        }
        if (t[2]) {
            //boTime
            temp.setBoTime(record.getBoTime());
        }
        if (t[3]) {
            //nextId
            temp.setNextId(record.getNextId());
        }
        if (t[4]) {
            //id
            temp.setId(record.getId());
        }
        if (t[5]) {
            //bookId
            temp.setBookId(record.getBookId());
        }
        if (t[6]) {
            //reTime
            temp.setReTime(record.getReTime());
        }
        if (t[7]) {
            //state
            temp.setState(record.getState());
        }
        writeRecord(this->head_size, this->data_size, temp, now);
    }
    return result;
}

vector<int> TableRecord::updateByPerson(int id, Record record) {
    vector<int> result;                             //该动态数组用于存放返回结果（修改了哪些id的记录）
    if (id > this->data_number) {
        return result;
    }
    while (id != -1) {
        Record temp = readRecord(this->head_size, this->data_size, id);
        int now = id;
        result.push_back(id);
        id = temp.getNextId();
        temp.setStId(record.getStId());
        temp.setBoTime(record.getBoTime());
        temp.setNextId(record.getNextId());
        temp.setId(record.getId());
        temp.setBookId(record.getBookId());
        temp.setReTime(record.getReTime());
        temp.setState(record.getState());
        writeRecord(this->head_size, this->data_size, temp, now);
    }
    return result;
}

vector<Record> TableRecord::queryByPerson(int id) {
    //按借阅者的id进行查询
    vector<Record> result;
    if (id > this->data_number) {
        return result;
    }
    while (id != -1) {
        Record temp = readRecord(this->head_size, this->data_size, id);
        result.push_back(temp);
        id = temp.getNextId();
    }
    return result;
}

vector<int> TableRecord::updateByBookId(int bookId, Record record, vector<int> reviseField) {
    IndexRecord *instance = IndexRecord::getInstance();                  //索引类的单例模式
    vector<int> v = instance->queryIndex(bookId);                        //用bookId对应的索引去查询bookId对应的借阅记录所在的行号
    if ((int) v.size() == 0) {                                    //如果索引返回了一个空的动态数组，该函数直接返回一个空的动态数组
        return v;
    }
    int t[8] = {0};                                                     //该表共有7个字段，字段编号从1开始
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 7)
            t[reviseField[i]] = 1;
    }
    for (int i = 0; i < (int) v.size(); i++) {                            //遍历动态数组v，对每一条记录进行修改
        Record temp = readRecord(this->head_size, this->data_size, v[i]);
        if (t[1]) {
            //stId
            temp.setStId(record.getStId());
        }
        if (t[2]) {
            //boTime
            temp.setBoTime(record.getBoTime());
        }
        if (t[3]) {
            //nextId
            temp.setNextId(record.getNextId());
        }
        if (t[4]) {
            //id
            temp.setId(record.getId());
        }
        if (t[5]) {
            //bookId
            temp.setBookId(record.getBookId());
        }
        if (t[6]) {
            //reTime
            temp.setReTime(record.getReTime());
        }
        if (t[7]) {
            //state
            temp.setState(record.getState());
        }
        writeRecord(this->head_size, this->data_size, temp, v[i]);      //将记录修改到文件中
    }
    return v;
}

vector<int> TableRecord::updateByBookId(int bookId, Record record) {
    IndexRecord *instance = IndexRecord::getInstance();                  //索引类的单例模式
    vector<int> v = instance->queryIndex(bookId);                        //用bookId对应的索引去查询bookId对应的借阅记录所在的行号
    if ((int) v.size() == 0) {                                    //如果索引返回了一个空的动态数组，该函数直接返回一个空的动态数组
        return v;
    }
    for (int i = 0; i < (int) v.size(); i++) {                            //遍历动态数组v，对每一条记录进行修改
        Record temp = readRecord(this->head_size, this->data_size, v[i]);
        temp.setStId(record.getStId());
        temp.setBoTime(record.getBoTime());
        temp.setNextId(record.getNextId());
        temp.setId(record.getId());
        temp.setBookId(record.getBookId());
        temp.setReTime(record.getReTime());
        temp.setState(record.getState());
        writeRecord(this->head_size, this->data_size, temp, v[i]);      //将记录修改到文件中
    }
    return v;
}

vector<Record> TableRecord::queryByBookId(int bookId) {
    //按书id进行查询
    IndexRecord *instance = IndexRecord::getInstance();
    vector<int> v = instance->queryIndex(bookId);
    vector<Record> result;
    if ((int) v.size() == 0) {
        return result;
    }
    for (int i = 0; i < (int) v.size(); i++) {
        Record temp = readRecord(this->head_size, this->data_size, v[i]);
        result.push_back(temp);
    }
    return result;
}

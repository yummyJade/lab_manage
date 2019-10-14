#ifndef TABLERECORD_H
#define TABLERECORD_H

#include <cstdio>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include "Record.h"

using namespace std;

class TableRecord {
private:
    char *table_name;             //表名
    int head_size;                               //文件头占据多少字节
    int data_size;                               //每一条数据占多少字节
    int data_number;                             //表中目前共有多少条数据

    TableRecord();

public:
    static TableRecord *tableRecord;               //单例模式
    static TableRecord *getInstance();             //获取数据库类的实例

    virtual ~TableRecord();

    int insertData(int id, Record record);

    int update(int id, vector<int> reviseField, Record record);

    Record query(int id);

    vector<int> updateByPerson(int id, vector<int> reviseField, Record record);

    vector<Record> queryByPerson(int id);

    vector<int> updateByBookId(int bookId, vector<int> reviseField, Record record);

    vector<Record> queryByBookId(int bookId);
};

#endif // TABLERECORD_H

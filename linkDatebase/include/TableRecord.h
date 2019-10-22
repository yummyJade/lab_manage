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
    char *table_name;             //����
    int head_size;                               //�ļ�ͷռ�ݶ����ֽ�
    int data_size;                               //ÿһ������ռ�����ֽ�
    int data_number;                             //����Ŀǰ���ж���������

    TableRecord();

public:
    static TableRecord *tableRecord;               //����ģʽ
    static TableRecord *getInstance();             //��ȡ���ݿ����ʵ��

    virtual ~TableRecord();

    int insertData(int id, Record record);

    int update(int id, Record record, vector<int> reviseField);

    int update(int id, Record record);

    vector<Record> query(int id);

    vector<int> updateByPerson(int id, Record record, vector<int> reviseField);

    vector<int> updateByPerson(int id, Record record);

    vector<Record> queryByPerson(int id);

    vector<int> updateByBookId(int bookId, Record record, vector<int> reviseField);

    vector<int> updateByBookId(int bookId, Record record);

    vector<Record> queryByBookId(int bookId);
};

#endif // TABLERECORD_H

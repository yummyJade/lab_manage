#include "../../linkDatebase/include/TableRecord.h"
#include "../../linkDatebase/include/IndexRecord.h"
#include <iostream>

using namespace std;
IndexRecord *IndexRecord::indexRecord = NULL;                       //��̬������ʼ��
TableRecord *TableRecord::tableRecord = NULL;

void writeRecord(int head_size, int data_size, Record record, int id) {
    /*����˵����
    head_size���ļ�ͷ����
    data_size��ÿ����¼�ĳ���
    id��������¼Ҫд���ļ��еڶ����У���������һ��������-1���ǲ����ļ�ĩβ
    */
    //ע�⣺�ú���δУ��id�Ƿ񳬹��˵�ǰ�ļ�¼��Ŀ
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
    /*����˵����
    head_size���ļ�ͷ����
    data_size��ÿ����¼�ĳ���
    id������ȡ�ļ�¼���ļ��еĵڼ�����¼*/
    FILE *fp = fopen("table_record", "rb");
    fseek(fp, head_size + (id - 1) * data_size, SEEK_SET);              //�ҵ���id����¼����ʼ��ַ
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
    //˽�й��캯��
    //this->table_name = "table_record";
    FILE *fp = fopen("table_record", "rb+");
    if (fp == NULL) {
        fp = fopen("table_record", "wb+");
    }
    fseek(fp, 0, SEEK_SET);                     //��ָ���ƶ����ļ�ͷ��
    bool flag = false;
    //��ȡ�ļ�ͷ�����в���
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
    ///����һ������
    if (id > this->data_number || (id != -1 && id <= 0)) {                   //�������
        //�����id���ݲ�����condition3��ֱ�ӷ���-1
        return -1;
    }
    record.setId(this->data_number + 1);
    if (id == -1) {
        record.setNextId(-1);
        writeRecord(this->head_size, this->data_size, record, -1);                        //���ú�������¼�������ݿ���
    } else {
        //������¼�Ǹ���ѧ���ĵ�һ�����ļ�¼��Ӧ�ð�������
        Record temp = readRecord(this->head_size, this->data_size, id);
        int nextId = temp.getNextId();
        temp.setNextId(this->data_number + 1);
        writeRecord(this->head_size, this->data_size, temp, id);
        record.setNextId(nextId);
        writeRecord(this->head_size, this->data_size, record, -1);                        //���ú�������¼�������ݿ���
    }
    this->data_number++;
    FILE *fp = fopen("table_record", "rb+");
    fseek(fp, sizeof(int), SEEK_SET);                       //�����ļ�ͷ�ϵļ�¼����
    int number = this->data_number;
    fwrite(&number, sizeof(int), 1, fp);
    fclose(fp);
    return (id == -1) ? (this->data_number) : (id);                     //��������ز�ͬ�ķ���ֵ
}

int TableRecord::update(int id, Record record, vector<int> reviseField) {
    //��id�޸�һ������
    if (id > this->data_number) {
        return -1;
    }
    Record temp = readRecord(this->head_size, this->data_size, id);
    int t[8] = {0};                                                     //�ñ���7���ֶΣ��ֶα�Ŵ�1��ʼ
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
    //��id�޸�һ������
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
    //��id��ѯһ������
    vector<Record> result;
    if (id > this->data_number) {
        return result;
    }
    Record temp = readRecord(this->head_size, this->data_size, id);
    result.push_back(temp);
    return result;
}

vector<int> TableRecord::updateByPerson(int id, Record record, vector<int> reviseField) {
    //�������ߵ�id���������޸�
    /*����˵����
    id:�ý����ߵ��������ļ�¼��id
    reviseField:Ҫ�޸ĵ��ֶ�
    record:Ҫ�ĳɵ�ֵ����record����
    */
    vector<int> result;                             //�ö�̬�������ڴ�ŷ��ؽ�����޸�����Щid�ļ�¼��
    if (id > this->data_number) {
        return result;
    }
    int t[8] = {0};                                                     //�ñ���7���ֶΣ��ֶα�Ŵ�1��ʼ
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 7)
            t[reviseField[i]] = 1;
    }
    while (id != -1) {
        //id!=-1˵������δ��ĩβ
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
    vector<int> result;                             //�ö�̬�������ڴ�ŷ��ؽ�����޸�����Щid�ļ�¼��
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
    //�������ߵ�id���в�ѯ
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
    IndexRecord *instance = IndexRecord::getInstance();                  //������ĵ���ģʽ
    vector<int> v = instance->queryIndex(bookId);                        //��bookId��Ӧ������ȥ��ѯbookId��Ӧ�Ľ��ļ�¼���ڵ��к�
    if ((int) v.size() == 0) {                                    //�������������һ���յĶ�̬���飬�ú���ֱ�ӷ���һ���յĶ�̬����
        return v;
    }
    int t[8] = {0};                                                     //�ñ���7���ֶΣ��ֶα�Ŵ�1��ʼ
    for (int i = 0; i < (int) reviseField.size(); i++) {
        if (reviseField[i] >= 1 && reviseField[i] <= 7)
            t[reviseField[i]] = 1;
    }
    for (int i = 0; i < (int) v.size(); i++) {                            //������̬����v����ÿһ����¼�����޸�
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
        writeRecord(this->head_size, this->data_size, temp, v[i]);      //����¼�޸ĵ��ļ���
    }
    return v;
}

vector<int> TableRecord::updateByBookId(int bookId, Record record) {
    IndexRecord *instance = IndexRecord::getInstance();                  //������ĵ���ģʽ
    vector<int> v = instance->queryIndex(bookId);                        //��bookId��Ӧ������ȥ��ѯbookId��Ӧ�Ľ��ļ�¼���ڵ��к�
    if ((int) v.size() == 0) {                                    //�������������һ���յĶ�̬���飬�ú���ֱ�ӷ���һ���յĶ�̬����
        return v;
    }
    for (int i = 0; i < (int) v.size(); i++) {                            //������̬����v����ÿһ����¼�����޸�
        Record temp = readRecord(this->head_size, this->data_size, v[i]);
        temp.setStId(record.getStId());
        temp.setBoTime(record.getBoTime());
        temp.setNextId(record.getNextId());
        temp.setId(record.getId());
        temp.setBookId(record.getBookId());
        temp.setReTime(record.getReTime());
        temp.setState(record.getState());
        writeRecord(this->head_size, this->data_size, temp, v[i]);      //����¼�޸ĵ��ļ���
    }
    return v;
}

vector<Record> TableRecord::queryByBookId(int bookId) {
    //����id���в�ѯ
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

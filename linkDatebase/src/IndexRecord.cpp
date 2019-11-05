#include "../../linkDatebase/include/IndexRecord.h"
#include <cstdio>
#include "../../linkDatebase/include/Record.h"
using namespace std;
Record readRecord3(int head_size, int data_size, int id) {
    /*����˵����
    head_size���ļ�ͷ����
    data_size��ÿ����¼�ĳ���
    id������ȡ�ļ�¼���ļ��еĵڼ�����¼*/
    FILE * fp = fopen("table_record", "rb");
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

IndexRecord* IndexRecord::getInstance() {
    if(indexRecord == NULL) {
        indexRecord = new IndexRecord;
    }
    return indexRecord;
}

IndexRecord::IndexRecord()
{
    //ctor
}

IndexRecord::~IndexRecord()
{
    //dtor
}

int IndexRecord::addIndex(int id) {
    return 0;
}

vector<int> IndexRecord::queryIndex(int id) {
    FILE *fp = fopen("table_record", "rb");
    int data_size, data_number;
    fread(&data_size, sizeof(int), 1, fp);
    fread(&data_number, sizeof(int), 1, fp);
    vector<int> result;
    for(int i = 1; i <= data_size; i++) {
        Record temp = readRecord3(2*sizeof(int), data_size, i);
        if(temp.getBookId() == id) {
            result.push_back(temp.getId());
        }
    }
    fclose(fp);
    return result;
}
int IndexRecord::init() {
    return 0;
}

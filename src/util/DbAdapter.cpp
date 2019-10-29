#include "util/DbAdapter.h"

DbAdapter::DbAdapter() {

}

DbAdapter::DbAdapter(const string &dbName) : dbName(dbName) {
    this->dbName = dbName;
    this->connect();
}


bool DbAdapter::connect() {
    if (this->db == NULL) {
        db = new DataBase();
        if (this->db->chooseTable(this->dbName) == 0) {
//            printf("���ݿ����ӳɹ� by-̷\n");
            return true;
        }
        cout << this->db->chooseTable(this->dbName) << endl;
//        printf("δ�ҵ���, by-̷\n");
        return false;

    } else {
        printf("���ݿ�����ʧ�� by-̷\n");
    }
    return false;
}

bool DbAdapter::disconnect() {
    if (this->db == NULL) {
//        printf("���ݿ���δ����\n");
        return false;
    } else {
        delete this->db;
//        printf("���ݿ�Ͽ����ӳɹ�\n");
        return true;
    }
}


vector<vector<string>> DbAdapter::searchBySingleField(string field, string value) {
    vector<vector<string>> results;
    vector<ll> id(0);
    this->db->query(field, value, id, results);

    for (int i = 0; i < results.size(); ++i) { // ��idװ�����һ��λ��
        results[i].push_back(to_string(id[i]));
    }
    return results;
}


bool DbAdapter::insert(vector<vector<string>> values, vector<long long> ids) {
    int result = this->db->insert(values, ids);
    //cout << "��������" << result << endl;
    return true;
}

bool DbAdapter::deleteBySingleField(string field, string value) {
    this->db->deleteData(field, value);
    return true;
}

bool DbAdapter::updateBySingleField(string assignField, string assignValue, string changeField, string changeValue) {
    this->db->update(assignField, assignValue, changeField, changeValue);
    return true;
}

DbAdapter::~DbAdapter() {
    this->disconnect();
}

vector<vector<string>> DbAdapter::searchAll() {
    vector<vector<string>> results;
    vector<ll> id(0);
    this->db->queryById(id, results);

    for (int i = 0; i < results.size(); ++i) { // ��idװ�����һ��λ��
        results[i].push_back(to_string(id[i]));
    }
    //cout << "������������" << results.size() << endl;
    return results;
}

vector<vector<string>> DbAdapter::searchBettwenIdA2B(long long startId, long long endId) {
    vector<vector<string>> results;
    vector<ll> id(0);
    this->db->queryById(id, results,startId,endId);

    for (int i = 0; i < results.size(); ++i) { // ��idװ�����һ��λ��
        results[i].push_back(to_string(id[i]));
    }
    //cout << "������������" << results.size() << endl;
    return results;
}


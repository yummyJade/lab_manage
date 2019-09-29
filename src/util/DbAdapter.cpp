//
// Created by Tjm on 2019/9/24.
//

#include "util/DbAdapter.h"

DbAdapter::DbAdapter() {

}

DbAdapter::DbAdapter(const string &dbName) : dbName(dbName) {
    this->dbName = dbName;
    this->connect();
}


bool DbAdapter::connect() {
    if (this->db == NULL) {
        db = DataBase::getInstance();
//        db->getInstance();
        if (this->db->chooseTable(this->dbName) != "δ����") {
            printf("���ݿ����ӳɹ� by-̷\n");
            return true;
        }
        printf("δ�ҵ��� by-̷\n");
        return false;

    } else {
        printf("���ݿ�����ʧ�� by-̷\n");
    }
    return false;
}

bool DbAdapter::disconnect() {
    if (this->db == NULL) {
        printf("���ݿ���δ����");
        return false;
    } else {
        delete this->db;
        printf("���ݿ�Ͽ����ӳɹ�");
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


bool DbAdapter::insert(vector<vector<string>> values) {

    this->db->insert(values);
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
    this->db->showDatas();
    return vector<vector<string>>();
//    return this->db->showDatas();
}

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
//            printf("数据库连接成功 by-谭\n");
            return true;
        }
        cout << this->db->chooseTable(this->dbName) << endl;
//        printf("未找到表, by-谭\n");
        return false;

    } else {
        printf("数据库连接失败 by-谭\n");
    }
    return false;
}

bool DbAdapter::disconnect() {
    if (this->db == NULL) {
//        printf("数据库尚未连接\n");
        return false;
    } else {
        delete this->db;
//        printf("数据库断开连接成功\n");
        return true;
    }
}


vector<vector<string>> DbAdapter::searchBySingleField(string field, string value) {
    vector<vector<string>> results;
    vector<ll> id(0);
    this->db->query(field, value, id, results);

    for (int i = 0; i < results.size(); ++i) { // 把id装进最后一个位置
        results[i].push_back(to_string(id[i]));
    }
    return results;
}


bool DbAdapter::insert(vector<vector<string>> values, vector<long long> ids) {
    int result = this->db->insert(values, ids);
    //cout << "插入结果是" << result << endl;
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

    for (int i = 0; i < results.size(); ++i) { // 把id装进最后一个位置
        results[i].push_back(to_string(id[i]));
    }
    //cout << "检索到的数量" << results.size() << endl;
    return results;
}

vector<vector<string>> DbAdapter::searchBettwenIdA2B(long long startId, long long endId) {
    vector<vector<string>> results;
    vector<ll> id(0);
    this->db->queryById(id, results,startId,endId);

    for (int i = 0; i < results.size(); ++i) { // 把id装进最后一个位置
        results[i].push_back(to_string(id[i]));
    }
    //cout << "检索到的数量" << results.size() << endl;
    return results;
}


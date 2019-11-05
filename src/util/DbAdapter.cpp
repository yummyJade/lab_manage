#include "util/DbAdapter.h"
DataBase* DbAdapter::UserDb=new DataBase();


//DbAdapter::UserDb.chooseTable("User");
DataBase* DbAdapter::BookDb=new DataBase();
//DbAdapter::BookDb->chooseTable("Book");

DbAdapter::DbAdapter() {

}

DbAdapter::DbAdapter(const string &dbName) : dbName(dbName) {
    this->dbName = dbName;
    if(dbName=="User"){
        this->db=DbAdapter::UserDb;
    }else if(dbName=="Book"){
        this->db=DbAdapter::BookDb;
    }else{

        this->connect();
//        cout<<"�������ݿ�ʧ��"<<endl;

    }

//    this->UserDb->chooseTable(dbName);
//    this->connect();
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
    return false;

//    if (this->db == NULL) {
//        return false;
//    } else {
//        delete this->db;
//        return true;
//    }
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

bool DbAdapter::clearTable() {
	this->db->clearTable(this->dbName);
	return true;

}

bool DbAdapter::checkTableExist(std::string tableName) {
    DataBase *db = new DataBase();
    return db->chooseTable(tableName) == 0;
}

vector<vector<string>> DbAdapter::searchFuzzyBySingleField(string field, string value) {
    vector<vector<string>> results;
    vector<ll> id(0);
    this->db->query(field, value, id, results,true);

    for (int i = 0; i < results.size(); ++i) { // ��idװ�����һ��λ��
        results[i].push_back(to_string(id[i]));
    }
    return results;
}

bool DbAdapter::init() {
    if(DbAdapter::UserDb->chooseTable("User")!=0){
        cout<<"��ʼ��ʧ��"<<endl;
    }
    if(DbAdapter::BookDb->chooseTable("Book")!=0){
        cout<<"��ʼ��ʧ��"<<endl;
    }
    cout<<"��ʼ���ɹ�"<<endl;
	return true;
}


#ifndef MYC1_DBADAPTER_H
#define MYC1_DBADAPTER_H

//#include "../../database_v1.0/database.cpp"
#include "../../database/libDB.h"
class DbAdapter {
    DataBase *db = NULL;
    string dbName;

public:
    DbAdapter();


    virtual ~DbAdapter();

    DbAdapter(const string &dbName);

    // 连接数据库
    bool connect();

    // 断开连接
    bool disconnect();

    // 插入数据,这个是强制插入,不帮忙判重
    bool insert(vector<vector<string>> values, vector<long long> ids);

    // 根据单个字段的值删除
    bool deleteBySingleField(string field, string value);

    // 根据单个字段搜索
    vector<vector<string>> searchBySingleField(string field, string value);

    // 修改某个属性值x为x_v的对象的某个属性y的值为y_v
    bool updateBySingleField(string assignField, string assignValue, string changeField, string changeValue);

    // 返回该表的所有内容哟
    vector<vector<string>> searchAll();


};


#endif //MYC1_DBADAPTER_H

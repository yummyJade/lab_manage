#include "index.h"
#include <cstdio>
#include <list>

using namespace std;

ll Index::hash(string value, ll mod) {
    ll len = value.length();
    ll ans = 0;
    for (int i = 0; i < len; i++) {
        ans = (ans + (value[i] > 0 ? value[i] : -value[i]) * 31) % mod;
    }
    return ans;
}

int Index::check() {
    return 0;
}

int Index::bucketInit() {
    bucket.empty();
    bucket.resize(sta->table_col_num[sidx]);
    for (ll field = 0; field < sta->table_col_num[sidx]; field++) {
        if (sta->isHash[sidx][field] == 'T') {
            bucket[field].resize(HASHMOD);
            for (ll hashCode = 0; hashCode < HASHMOD; hashCode++) {
                bucket[field][hashCode].empty();
            }
        } else {
            bucket[field].empty();
        }
    }
    return 0;
}

Index::Index(ll id) {
    sta = Statu::getInstance();
    bucket.clear();
    sidx = id;
    read();
}

int Index::read() {

    //��ʼ��
    bucketInit();
    string name = sta->table_name[sidx] + "_hash";
    string name2 = sta->table_name[sidx] + "_hash" + "_delete";
    char valid, valid2;
    ll data_t, addr;
    FILE *fp = fopen(name.data(), "r");
    FILE *fp2 = fopen(name2.data(), "r");
    //�жϲ����ļ�
    if (fp == NULL) {
        createInsertFile();
    }

    if (fp2 == NULL) {
        createDeleteFile();
    }

    rewind(fp);
    rewind(fp2);

    //��ȡinsert�ļ�
    while ((valid = getc(fp)) != EOF) {
        //��ȡ����
        vector<ll> data(sta->table_col_num[sidx]);
        for (ll i = 0; i < sta->table_col_num[sidx]; i++) {
            if (sta->isHash[sidx][i] == 'T') {
                fread(&data_t, sizeof(ll), 1, fp);
                data[i] = data_t;
            }
        }
        fread(&addr, sizeof(ll), 1, fp);
        //����bucket
        for (ll i = 0; i < sta->table_col_num[sidx]; i++) {
            if (sta->isHash[sidx][i] == 'T') {
                bucket[i][data[i]].push_front(addr);
            }
        }
    }

    //��ȡdelete�ļ�
    while ((valid = getc(fp2)) != EOF) {
        //��ȡ����
        vector<ll> data(sta->table_col_num[sidx]);
        for (ll i = 0; i < sta->table_col_num[sidx]; i++) {
            if (sta->isHash[sidx][i] == 'T') {
                fread(&data_t, sizeof(ll), 1, fp2);
                data[i] = data_t;
            }
        }
        fread(&addr, sizeof(ll), 1, fp2);
        //��bucket��ɾ��
        for (ll i = 0; i < sta->table_col_num[sidx]; i++) {
            if (sta->isHash[sidx][i] == 'T') {
                bucket[i][data[i]].remove(addr);
            }
        }
    }
    fclose(fp);
    fclose(fp2);
    return 0;
}

/*
int Index::save(){
    ll checkCode = check();
    if(checkCode != 0)
        return checkCode;
    string name =  sta->table_name[sidx] + "_hash";
    FILE * fp = fopen(name.data(), "w");
    rewind(fp);

    ll cnum = sta->table_col_num[sidx];
    ll n;
    for(ll field = 0; field < cnum; field++){
        if(sta->isHash[sidx][field] == 'T'){
            for(ll i = 0; i < HASHMOD; i++){
                //��ǰͰ��ָ������
                n = bucket[field][i].size();
                fwrite(&n, sizeof(ll), 1, fp);
                //д��������ÿһ��
                list<ll>::iterator itr = bucket[field][i].begin();
                while(itr != bucket[field][i].end()){
                    ll t = bucket[field][i].size();
                    fwrite(&(*itr), sizeof(ll), 1, fp);
                    itr++;
                }
            }
        }
    }
    fclose(fp);
    return 0;
}
*/

int Index::createInsertFile() {
    string name = sta->table_name[sidx] + "_hash";
    FILE *fp = fopen(name.data(), "w");
    fclose(fp);
    return 0;
}

int Index::createDeleteFile() {
    string name = sta->table_name[sidx] + "_hash" + "_delete";
    FILE *fp = fopen(name.data(), "w");
    fclose(fp);
    return 0;
}

int Index::insert(const vector<vector<string> > &s, const vector<ll> &addr) {
    ll checkCode = check();
    if (checkCode != 0)
        return checkCode;
    ll n = s.size();
    //����Ϊ��
    if (n == 0) return 0;

    ll cnum = sta->table_col_num[sidx];
    if (s[0].size() != cnum) return -1;
    ll hashCode;
    char valid = 'T';
    string name = sta->table_name[sidx] + "_hash";
    FILE *fp = fopen(name.data(), "a");
    for (ll i = 0; i < n; i++) {
        fwrite(&valid, sizeof(char), 1, fp);
        for (ll j = 0; j < cnum; j++) {
            if (sta->isHash[sidx][j] == 'T') {
                hashCode = hash(s[i][j]);
                bucket[j][hashCode].push_front(addr[i]);
                fwrite(&hashCode, sizeof(ll), 1, fp);
            }
        }
        fwrite(&addr[i], sizeof(ll), 1, fp);
    }
    fclose(fp);
    return 0;
}

int Index::deleteData(const vector<vector<string> > &s, const vector<ll> &addr) {
    ll checkCode = check();
    if (checkCode != 0)
        return checkCode;
    ll n = s.size();
    if (n == 0) return 0;
    ll cnum = sta->table_col_num[sidx];
    ll hashCode, t, k;
    char valid = 'T';
    string name = sta->table_name[sidx] + "_hash" + "_delete";
    FILE *fp = fopen(name.data(), "a");
    for (ll i = 0; i < n; i++) {
        fwrite(&valid, sizeof(char), 1, fp);
        for (ll field = 0; field < cnum; field++) {
            if (sta->isHash[sidx][field] == 'T') {
                hashCode = hash(s[i][field]);
                //�˴�Ĭ��ÿ����Ч��ַΨһ
                bucket[field][hashCode].remove(addr[i]);
                fwrite(&hashCode, sizeof(ll), 1, fp);
            }
        }
        fwrite(&addr[i], sizeof(ll), 1, fp);
    }
    fclose(fp);
    return 0;
}

int Index::query(ll idx, string value, list<ll> &addr) {
    ll checkCode = check();
    if (checkCode != 0)
        return checkCode;
    ll hashCode = hash(value);
    addr = bucket[idx][hashCode];
    return 0;
}


int Index::clear() {
    string name = sta->table_name[sidx] + "_hash";
    string name2 = sta->table_name[sidx] + "_hash" + "_delete";
    FILE *fp = fopen(name.data(), "w");
    fclose(fp);
    fp = fopen(name2.data(), "w");
    fclose(fp);
    return 0;
}
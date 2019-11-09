#include "index.h"
#include <cstdio>
#include <list>
#include <windows.h>

using namespace std;

ll Index::hash(string value, ll mod){
    ll len = value.length();
    ll ans = 0;
    for(int i = 0; i < len; i++){
        ans = ( ans + (value[i]>0?value[i]:-value[i])  * 31)%mod;
    }
    return ans;
} 

int Index::check(){
    return 0;
}

int Index::bucketInit(){
    bucket.empty();
    bucket.resize(sta->table_col_num[sidx]);
    for(ll field = 0; field < sta->table_col_num[sidx]; field++){
        if(sta->isHash[sidx][field] == 'T'){
            bucket[field].resize(HASHMOD);
            for(ll hashCode = 0; hashCode < HASHMOD; hashCode++){
                bucket[field][hashCode].empty();
            }
        }else{
            bucket[field].empty();
        }
    }
    return 0;
}

Index::Index(ll id){
    sta = Statu::getInstance();
    bucket.clear();
    sidx = id;
    logFileName =  settings::dataFolder + sta->table_name[sidx] + "_hash";
    read();
}

int Index::writeLog(FILE * fp, char opt, ll field, ll hashCode, ll addr){
    fwrite(&opt, sizeof(char), 1, fp);
    fwrite(&field, sizeof(ll), 1, fp);
    fwrite(&hashCode, sizeof(ll), 1, fp);
    fwrite(&addr, sizeof(ll), 1, fp);
    return 0;
}
int Index::ReadLog(FILE * fp, ll & field, ll & hashCode, ll & addr){
    fread(&field, sizeof(ll), 1, fp);
    fread(&hashCode, sizeof(ll), 1, fp);
    fread(&addr, sizeof(ll), 1, fp);
    return 0;
}

int Index::read(){
    
    //��ʼ��
    bucketInit();
    ll field, addr, hashCode;
    char opt;
    FILE * fp = fopen(logFileName.data(), "rb");
    //�жϲ����ļ�
    if(fp == NULL){
        createLogFile();
        return 0;
    }

    rewind(fp);
    
    while((opt = fgetc(fp))!=EOF){
        //��ȡ����
        //int t = ftell(fp);
        ReadLog(fp, field, hashCode, addr);
        //t = ftell(fp);
        if(opt == 'i'){
            bucket[field][hashCode].push_front(addr);
        }else if(opt == 'd'){
            bucket[field][hashCode].remove(addr);
        }else{
            continue;
        }
    }    

    fclose(fp);
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

int Index::createLogFile(){
    string name =  sta->table_name[sidx] + "_hash";
    FILE* fp = fopen(logFileName.data(), "wb");
    fclose(fp);
    return 0;
}


int Index::insert(const vector< vector<string> > & s, const vector <ll> & addr){
    ll checkCode = check();
    if(checkCode != 0)
        return checkCode;
    ll n = s.size();
    //����Ϊ��
    if(n == 0) return 0;
    
    ll cnum = sta->table_col_num[sidx];
    if(s[0].size() != cnum) return -1;
    ll hashCode;
    
    FILE * fp = fopen(logFileName.data(), "ab");
    for(ll i = 0; i < n; i++){
        for(ll j = 0; j < cnum; j++){
            if(sta->isHash[sidx][j] == 'T'){
                hashCode = hash(s[i][j]);
                bucket[j][hashCode].push_front(addr[i]);
                writeLog(fp, 'i', j, hashCode, addr[i]);
            }
        }
    }
    fclose(fp);
    return 0;
}

int Index::deleteData(const vector< vector<string> > &s, const vector <ll> & addr){
    ll checkCode = check();
    if(checkCode != 0)
        return checkCode;
    ll n = s.size();
    if(n == 0) return 0;
    ll cnum = sta->table_col_num[sidx];
    ll hashCode, t, k;

    FILE * fp = fopen(logFileName.data(), "ab");
    for(ll i = 0; i < n; i++){
        for(ll field = 0; field < cnum; field++){
            if(sta->isHash[sidx][field] == 'T'){
                hashCode = hash(s[i][field]);
                //�˴�Ĭ��ÿ����Ч��ַΨһ
                bucket[field][hashCode].remove(addr[i]);
                writeLog(fp, 'd', field, hashCode, addr[i]);      
            }
        }
    }
    fclose(fp);
    return 0;
}

int Index::query(ll idx, string value, list<ll> & addr){
    ll checkCode = check();
    if(checkCode != 0)
        return checkCode;
    ll hashCode = hash(value);
    addr = bucket[idx][hashCode];
    return 0;
}

int Index::update(ll idx, string value, string value2, ll addr){
    FILE * fp = fopen(logFileName.data(), "ab");

    ll hashCode = hash(value);
    bucket[idx][hashCode].remove(addr);
    writeLog(fp, 'd', idx, hashCode, addr);
    hashCode = hash(value2);
    bucket[idx][hashCode].push_front(addr);
    writeLog(fp, 'i', idx, hashCode, addr);

    fclose(fp);
    return 0;
}

int Index::clear(){
    string name =  sta->table_name[sidx] + "_hash";
    string name2 = sta->table_name[sidx] + "_hash" + "_delete";
    //ɾ������hash
    FILE * fp = fopen((settings::dataFolder + name).data(), "wb");
    fclose(fp);
    DeleteFileA((settings::dataFolder + name).data());
    //ɾ��ɾ��hash�ļ�
    fp = fopen((settings::dataFolder + name2).data(), "wb");
    fclose(fp);
    DeleteFileA((settings::dataFolder + name2).data());

    return 0;
}
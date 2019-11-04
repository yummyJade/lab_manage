#include "../database.h"
#include "statu.h"
#include "index.h"
#include <cstdio>
#include <string>
#include <windows.h>
#include "cmdui.h"
using namespace std;

DataBase::DataBase(){
	sta = Statu::getInstance();
	table_id = -1;
	index.clear();
	for(ll i = 0; i < sta->table_number; i++){
		index.push_back(new Index(i));
	}
	//�����ļ���
	// std::wstring stemp = std::wstring(settings::dataFolder.begin(), settings::dataFolder.end());
	// LPCWSTR sw = stemp.c_str();
	if (CreateDirectory(settings::dataFolder.c_str(), NULL) ||
    ERROR_ALREADY_EXISTS == GetLastError()){
    	//cout << "Yes" << endl;
		//todo: �ļ��д����벻���ڴ����� 
	}

	//�������ļ��ı����Ƿ���ڣ��������򴴽�
	for(ll i = 0; i < sta->table_number; i++){
		FILE * fp = fopen( (settings::dataFolder + sta->table_name[i]).data() , "rb");
		if(fp == NULL){
			fp = fopen((settings::dataFolder + sta->table_name[i]).data(), "wb");
			fclose(fp);
		}
	}
}

DataBase::~DataBase(){
	ll n = index.size();
	for(ll i = 0; i < n; i++){
		delete index[i];
	}
	index.clear();
}

int DataBase::createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique){
	
	ll code = sta->createTable(name, col_name, col_size, isHash, isUnique);
	if(code == 0){
		//���������ݿ��ļ�
		name = settings::dataFolder+name;
		FILE * fp = fopen(name.data(), "w");
		//�����ļ�����ʧ��
		if(fp == NULL) 
			return -301;
		rewind(fp);
		//д���ͷ
		ll nextLocation = -1;
		fwrite(&nextLocation, sizeof(ll), 1, fp);
		fclose(fp);
		index.push_back(new Index(sta->table_number-1));
		return 0;
	}else{
		return code;
	}	 
}

int DataBase::check(){
	if(table_id == -1)
		return -1;
	return 0;
}

void DataBase::showTables(){
	// printf("show\n");
	// printf("�������ֶΣ��ֶγ��ȣ��Ƿ���hash����\n");
	// for(int i = 0; i < sta->table_number; i++){
	// 	printf("������%s �ֶΣ�", sta->table_name[i].data());
	// 	for(int j = 0; j < sta->table_col_num[i]; j++){
	// 		printf("%s %lld %c ", sta->table_col_name[i][j].data(), sta->table_col_size[i][j], sta->isHash[i][j]);
	// 	}
	// 	printf("\n");
	// }
	vector< vector<string> > data;
	
	for(ll i = 0; i < sta->table_number; i++){
		vector<string> t[3];
		data.clear();
		t[0].clear();
		t[0].push_back("������"+sta->table_name[i]);
		for(ll j = 0; j < sta->table_col_num[i]; j++){
			t[0].push_back(sta->table_col_name[i][j]);
		}
		data.push_back(t[0]);

		t[1].clear();
		t[1].push_back("���ݴ�С");
		for(ll j = 0; j < sta->table_col_num[i]; j++){
			t[1].push_back(to_string(sta->table_col_size[i][j]));
		}
		data.push_back(t[1]);

		t[2].clear();
		t[2].push_back("����");
		for(ll j = 0; j < sta->table_col_num[i]; j++){
			char chart[2];
			chart[0] = sta->isHash[i][j];
			t[2].push_back(chart);
		}
		data.push_back(t[2]);
		// CmdUI::drawTable(data);
	}
}


bool DataBase::isChoose(){
	if(table_id == -1){
		return false;
	}
	return true;
}

string DataBase::getTableName(){
	if(table_id == -1)
		return "";
	return sta->table_name[table_id];
}

ll DataBase::getColNum(){
	if(table_id == -1) return 0;
	return sta->table_col_num[table_id];
}

vector<string> DataBase::getColName(){
	if(table_id == -1) return vector<string>(0);
	return sta->table_col_name[table_id];
}

int DataBase::chooseTable(string name){
	ll code = sta->getIdx(name);
	if(code != -1){
		table_id = code;
		return 0;
	}else{
		return code;
	}
}



int DataBase::insert(const vector< vector<string> > & s, vector<ll> & id, string name){
	
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}
	
	//��ʼ������
	id.clear();

	//�õ���������
	ll cnum = sta->table_col_num[table_id]; //����
	char valid = 'T';
	vector <ll> & csize = sta->table_col_size[table_id];
	vector <ll> & cpsize = sta->table_col_pre_size[table_id];
	ll totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //�����ܳ�
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //ÿ�����ݳ���
	vector<ll> addr; addr.clear();
	//���ļ�
	FILE * fp = fopen( (settings::dataFolder + sta->table_name[table_id]).data(), "rb+");
	rewind(fp);

	//��ȡ��ͷ
	ll nextLocation;
	fread(&nextLocation, sizeof(ll), 1, fp);
	ll n = s.size();
	for(int i = 0; i < n; i++){
		//ÿ�ζ�ȡ��ͷ
		if(nextLocation == -1){
			//�����ļ�ĩβ
			fseek(fp, 0, SEEK_END);
			//д���־λ
			fwrite(&valid, sizeof(char), 1, fp);
			
			//д����б�λ
			nextLocation = -1;
			fwrite(&nextLocation, sizeof(ll), 1, fp);

			ll m = sta->table_col_num[table_id];
			
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}
		

		} else{ //�п�����
			//�ӿ�ʼ��ת������
			int temp;
			fseek(fp, nextLocation+sizeof(ll), SEEK_SET);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			//д���־λ
			fwrite(&valid, sizeof(char), 1, fp);
			fflush(fp);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			//��ȡ������nextֵ(�������)
			//ͬʱ��дҪ�ض�λ���������
			// fseek(fp, ftell(fp), SEEK_CUR);
			fread(&nextLocation, sizeof(ll), 1, fp);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			// fseek(fp, ftell(fp)-sizeof(ll), SEEK_CUR);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			ll tempNum = -1;
			fseek(fp, -sizeof(ll), SEEK_CUR);
			fwrite(&tempNum, sizeof(ll), 1, fp);	
			//д������
			ll m = sta->table_col_num[table_id];
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}  
		}

		//�������������fpλ�ü���id
		ll fpLocation = ftell(fp);
		id.push_back((fpLocation-sizeof(ll))/lineSeek);
		//�洢��ʼλ��
		addr.push_back(fpLocation-lineSeek);
	}
	//�����ļ�ͷ
	rewind(fp);
	//�����һ�λ�õ�nextֵд��ͷ��
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);
	index[table_id]->insert(s, addr);
	return 0;
} 

/*
void DataBase::showDatas(string name){
	if(name != "")
		this->chooseTable(name); 
	name = this->table_name; 

	//��������
	char valid;
	ll nextLocation;
	ll idx = 0;
	ll totalLen = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);

	FILE * fp = fopen(this->table_name.data(), "rb+");
	rewind(fp);
	fread(&nextLocation, sizeof(ll), 1, fp);
	
	printf("��ͷ��%lld  id:%lld\n", nextLocation, nextLocation/totalLen);
	

	//��ӡ��ͷ
	printf("��Чλ\t");
	printf("id\t");
	printf("nextLocation\t");
	for(ll i = 0; i < this->table_col_num; i++){
		printf("%s\t", this->table_col_name[i].data());
	} 
	printf("\n");
	//ȷ����ֶγ���
	ll maxLen = 0;
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector <char> s(maxLen);
	string str; 
	//��ȡ����ʾÿһ�� 
	while((valid=fgetc(fp))!=EOF){
		idx++;
		//printf("tell:%d\n", ftell(fp));
		//��ȡ��Чλ
		printf("%c\t", valid); 
		printf("%lld\t", idx);
		//��ȡ����λ
		fread(&nextLocation, sizeof(ll), 1, fp);
		printf("%lld nextId:%lld\t", nextLocation, nextLocation/totalLen);
		//��ȡ����ÿһλ���ݲ���ӡ 
		for(ll i = 0; i < this->table_col_num; i++){
			fread(&s[0], sizeof(char), this->table_col_size[i], fp);
			
			printf("%s\t",  &s[0]);
		}  
		printf("\n");
	} 
	
	fclose(fp);
} 
*/

int DataBase::deleteData(string key, string value, string name){
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}

	//���id
	vector<ll> id(0);
	vector<vector<string> > ans;
	this->query(key, value, id, ans);
	//��ʼ������
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + sta->table_col_pre_size[table_id][sta->table_col_num[table_id]]*sizeof(char);
	char valid = 'F';
	ll nextLocation = 0;
	ll headLocation = 0;
	//���ļ�
	FILE * fp = fopen((settings::dataFolder + sta->table_name[table_id]).data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	fread(&headLocation, sizeof(ll), 1, fp);
	for(ll i = 0; i < n; i++){
		printf("tell%lld id:%lld\n", headLocation, id[i]);
		//�ӿ�ͷ��ת��ָ��λ��
		fseek(fp, len*(id[i]-1)+sizeof(ll), SEEK_SET);
		fwrite(&valid, sizeof(char), 1, fp);
		fwrite(&headLocation, sizeof(ll),1, fp);
		headLocation = (id[i]-1)*len;
	}
	rewind(fp);
	//������һ��ͨ���ļ�ͷ����
	fwrite(&headLocation, sizeof(ll), 1, fp);
	fclose(fp);
}

int DataBase::query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, bool isFuzzy, string name){
	//�ж��Ƿ�ѡ��

	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}
	if(name != "")
		this->chooseTable(name);
	name = sta->table_name[table_id];

	//���ļ�
	FILE * fp = fopen( (settings::dataFolder+name).data(), "rb");
	
	//���������Ĳ�ѯ
	ll idx = sta->fieldToidxMap[table_id]["key"];
	if(isFuzzy == true){
		return traverseQuery(key, value, id, ans, fp, isFuzzy);
	}
	else if(sta->isHash[table_id][idx] == 'T'){
		return indexQuery(key, value, id, ans, fp);
	}else{ 
		return traverseQuery(key, value, id, ans, fp, isFuzzy);
	}
	return 0;
}

int DataBase::queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1, ll id2, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (sta->table_name[table_id] == "δѡ��" || sta->table_name[table_id] == ""))
		return -2;
	//��������
	ans.clear();
	id.clear();
	if(id2 < 0){
		id2 = 0x3f3f3f3f3f3f3f3f;
	}
	if(id1 > id2)
		throw "id��Χ����id1ӦС��id2";
	//��������
	char valid; //��Чλ
	ll nextLocation; //��������λ
	ll idx = 0; //id��¼��
	ll totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //�����ܳ�
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //ÿ�����ݳ���
	//ȷ����ֶγ���
	// todo: �ж��Ƿ���id������ǰ����,�Լ�id�Ϸ���
	ll maxLen = 0;
	for(ll i = 0; i < sta->table_col_num[table_id]; i++){
		maxLen = max(maxLen, sta->table_col_size[table_id][i]);
	}
	vector <char> s(maxLen);
	string str; 
	
	FILE * fp = fopen( (settings::dataFolder + sta->table_name[table_id]).data(), "rb");
	//������ͷ
	fseek(fp, sizeof(ll), SEEK_SET);
	//����ǰ�漸��
	fseek(fp, (id1-1)*lineSeek, SEEK_CUR);
	idx = id1;
	//��ȡ����ʾÿһ�� 
	while( ((valid=fgetc(fp))!=EOF) && idx <= id2){
		if(valid == 'T'){
			//������ͷ
			fseek(fp, sizeof(ll), SEEK_CUR);
			id.push_back(idx);
			vector<string> tempStr(sta->table_col_num[table_id]);
			for(ll i = 0; i < sta->table_col_num[table_id]; i++){
				fread(&s[0], sizeof(char), sta->table_col_size[table_id][i], fp);
				tempStr[i] = &s[0];
			}  
			ans.push_back(tempStr);
		}
		else{
			fseek(fp, lineSeek-sizeof(char), SEEK_CUR);
		}
		idx++;
	}
	fclose(fp);
}

int DataBase::update(string key, string value, string key2, string value2, string name){
	//�ж��Ƿ�ѡ��
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}

	vector<ll> id;
	vector< vector<string> > ans;
	this->query(key, value, id, ans, false, name);
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + sta->table_col_pre_size[table_id][sta->table_col_num[table_id]]*sizeof(char);
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key2, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//���޴��ֶ�
		return -1;
	}
	//�����ļ�����
	FILE * fp = fopen( (settings::dataFolder + sta->table_name[table_id]).data(), "rb+");
	for(int i = 0; i < n; i++){
		//������ͷ
		fseek(fp, sizeof(ll), SEEK_SET);
		//��ת������
		fseek(fp, (id[i]-1) * (totalSeek+sizeof(ll)+sizeof(char)), SEEK_CUR);
		//��ȡ��ǰ��ַ
		ll addr = ftell(fp);
		//������Чλ������λ
		fseek(fp, sizeof(char) + sizeof(ll), SEEK_CUR);
		//������������λ��
		
		fseek(fp, preSeek, SEEK_CUR);
		//д��������
		
		fwrite(value2.data(), sizeof(char), selfLen, fp);
		//fflush(fp);
		//�����������������
		if(sta->isHash[table_id][idx] == 'T'){
			index[table_id]->update(idx, value, value2, addr);
		}
	}
	fclose(fp);
	return 0;
}

bool DataBase::getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek){
	if(sta->fieldToidxMap[table_id].count(key) > 0){
		idx = sta->fieldToidxMap[table_id][key];
	}
	else return false; //�ֶβ�����
	//����ǰ�����ݳ��� 
	selfLen = sta->table_col_size[table_id][idx];
	preSeek = (sta->table_col_pre_size[table_id][idx] - sta->table_col_pre_size[table_id][0])*sizeof(char);
	lastSeek = (sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] - sta->table_col_pre_size[table_id][idx+1])*sizeof(char);
	totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //�����ܳ�
	return true;
}

int DataBase::clear(){
	//todo: ��չ��̴���׽
	ll num = sta->table_name.size();
	for(ll i = 0; i < num; i++){
		index[i]->clear();
		DeleteFileA((settings::dataFolder+sta->table_name[i]).data());
	}
	sta->clear();
	return 0;
}

int DataBase::clearTable(string name){
	ll checkCode = check();
	if(checkCode != 0)
		return checkCode;
	if(name == "") name = sta->table_name[table_id];
	ll nextLocation = -1;
	FILE * fp = fopen((settings::dataFolder + name).data(), "w");
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);

	//�������
	index[table_id]->clear();
	return 0;
}

int DataBase::traverseQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp, bool isFuzzy){
	id.clear();
	ans.clear();
	//��ȡ��������
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//���޴��ֶ�
		return -1;
	}
	//�������ݳ���
	ll lineSeek = totalSeek + sizeof(ll) + sizeof(char);
	//��ʼ������
	ll lineNum = 0; //��¼��ǰ�кţ�����id
	const ll & maxLen = sta->maxLen[table_id];
	//��ʱ���� 
	vector<char> s(maxLen+10);  
	string str; 
	char valid;
	
	vector <ll> & csize = sta->table_col_size[table_id];
	
	rewind(fp);
	fseek(fp, sizeof(ll), SEEK_SET);
	while((valid=fgetc(fp))!=EOF){
		lineNum++;
		//����������Ч 
		if(valid == 'T'){
			//��������λ
			fseek(fp, sizeof(ll), SEEK_CUR);
			//����ǰ�����ݿ�
			fseek(fp, preSeek, SEEK_CUR);
			//��ȡ���ݲ�����string
			fread(&s[0], sizeof(char), selfLen, fp);
			str = &s[0];
			// ������ȷ
			if( (!isFuzzy && str == value) || (isFuzzy &&  (str.find(value) != str.npos)) ){
				//�ҵ���ǰ��Ծ 
				fseek(fp, -(preSeek + selfLen*sizeof(char)), SEEK_CUR);
				//��ȡ��������
				vector<string> line(sta->table_col_num[table_id]); //һ�н�� 
				for(ll i = 0; i < sta->table_col_num[table_id]; i++){
					fread(&s[0], sizeof(char), csize[i], fp);
					line[i] = &s[0];
				}
				//vector�Ŀ�������� 
				ans.push_back(line); 
				id.push_back(lineNum);
			}
			else{
				//�������沿��
				fseek(fp, lastSeek, SEEK_CUR);
			}
		}
		else{
			fseek(fp, sizeof(ll), SEEK_CUR);
			fseek(fp, totalSeek, SEEK_CUR);
		}
	}
	return 0;
}

int DataBase::indexQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp){
	id.clear();
	ans.clear();
	//��ȡ��������
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//���޴��ֶ�
		return -1;
	}
	//�������ݳ���
	ll lineSeek = totalSeek + sizeof(ll) + sizeof(char);
	//��ʼ������
	ll lineNum = 0; //��¼��ǰ�кţ�����id
	const ll & maxLen = sta->maxLen[table_id];
	//��ʱ���� 
	vector<char> s(maxLen+10);  
	string str; 
	char valid;
	
	vector <ll> & csize = sta->table_col_size[table_id];
	list <ll> addr;
	index[table_id]->query(idx, value, addr);
	list<ll>::iterator itr;
	int  t = addr.size();
	for(itr = addr.begin(); itr != addr.end(); itr++){
		fseek(fp, *itr, SEEK_SET);
		valid = fgetc(fp);
		if(valid == 'T'){
			fseek(fp, sizeof(ll), SEEK_CUR);
			//����ǰ�����ݿ�
			fseek(fp, preSeek, SEEK_CUR);
			//��ȡ���ݲ�����string
			fread(&s[0], sizeof(char), selfLen, fp);
			str = &s[0];
			// ������ȷ
			if(str == value){
				// printf("%s������ͬ", str.data());
				//�ҵ���ǰ��Ծ 
				fseek(fp, -(preSeek + selfLen*sizeof(char)), SEEK_CUR);
				//��ȡ��������
				vector<string> line(sta->table_col_num[table_id]); //һ�н�� 
				
				for(ll i = 0; i < sta->table_col_num[table_id]; i++){
					fread(&s[0], sizeof(char), csize[i], fp);
					line[i] = &s[0];
				}
				//vector�Ŀ�������� 
				ans.push_back(line); 
				id.push_back((*itr - sizeof(ll))/lineSeek + 1);
			}
		}
	}
}
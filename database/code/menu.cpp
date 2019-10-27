#include "../database.h"
#include <time.h>
#include <string>
#include <windows.h>
#include "cmdui.h"
using namespace std;

int main(){
	
	//todo����һ����������? 
    //windows �����������Ҫ�л��ַ���
    //system("chcp 65001"); //�л��ַ�??
    system("cls");
	DataBase* d = new DataBase();
	ll opt = 1;
	string chooseTableName = "δѡ��";
	
	while(opt){
		chooseTableName = d->getTableName();
		if(chooseTableName == "") chooseTableName = "δѡ��";
		system("cls");
		printf("\
��ѡ�б�%s\n\
----------���ݿ����----------\n\
0.�˳�\n\
1.������\n\
*2.ɾ����\n\
3.ѡ���\n\
4.��ʾ���б�\n\
5.������ݿ�\n\
----------�������----------\n\
6.��������\n\
*7.��ʾ��ǰ��������\n\
8.���ݾ������ֲ�ѯֵ��where��\n\
9.ɾ��\n\
10.��յ�ǰ��\n\
11.��������\n\
12.����id��Χѡ������\n\
13.���Բ�������\n\
----------�����������-------\n\
*100.����\n\
*101.��֤����\n", chooseTableName.data());
		//�������� 
		vector <string> col_name;
		vector <ll> col_size;
		vector <ll> id(0);
		vector <char> isHash;
		vector < vector<string> > insertData(0);
		vector < vector<string> > queryData;
		char s[1000];
		wchar_t sw[1000];
		string name, name2, str, str2;
		char temp_c;
		ll num, num2;
		int code;
		CmdUI cmdui;
		//�������� 
		
		//��ɫ����
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_INTENSITY|BACKGROUND_GREEN);
		scanf("%d", &opt);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		clock_t optStartTime = clock();
		if(opt ==  1){  //����??
			printf("�������:");
			scanf("%s", s);
			name = s;
			
			printf("��������?:");
			scanf("%lld", &num); 
			
			col_name.resize(num);
			col_size.resize(num);
			isHash.clear();
			isHash.resize(num);
			printf("�������������\n");  
			for(ll i = 0; i < num; i++){
				
				scanf("%s", s);
				col_name[i] = s;
				
			}
			printf("�����������󳤶�\n");
			for(ll i = 0; i < num; i++){
				
				scanf("%lld", &col_size[i]);
			}
			printf("�������ÿ���ֶ��Ƿ���hash��������������T������������F\n");
			for(ll i = 0; i < num; i++){
				ll hashTemp; 
				scanf("%s", s);
				isHash[i] = s[0]; 
			}
			optStartTime = clock();
			printf("��ʼ����......\n");
			d->createTable(name, col_name, col_size, isHash);
			printf("�����ɹ�\n");
		} else if(opt == 2){
			// printf("����ϣ��ɾ���ı�?:");
			// scanf("%s", s);
			// name = s;
			// printf("��ʼɾ��?......\n");
			// d->deleteTable(name);
			// printf("ɾ���ɹ�\n");
		}else if(opt == 3){
			printf("������ѡ��ı�:");
			scanf("%s", s);
			name = s;
			optStartTime = clock();
			code = d->chooseTable(name);
			if(code == -1){
				printf("�����ڸñ�\n");
			}else{
				printf("ѡ��\n");
			}
		}else if(opt == 4){
				d->showTables();
		}else if(opt == 5){
				d->clear();
		}else if(opt == 6){
			if(!d->isChoose()){
				printf("��ѡ��һ�ű�\n");
				getchar();
				getchar();
				continue;
			}
			printf("����ϣ���������������?:");
			scanf("%lld", &num);
			num2 = d->getColNum();
			
			for(ll i = 0; i < num; i++){
				printf("����� %lld �����ݵ� %lld ������\n", i+1, num2);
				vector<string> line(num2);
				for(ll j = 0; j < num2; j++){
					scanf("%s", s);
					line[j] = s;
					//printf("%s\n", s);
				}
				insertData.push_back(line);
			} 
			optStartTime = clock();
			code = d->insert(insertData, id);
			if(code == -1){
				printf("δѡ�б�\n");
				continue;
			}
			printf("����Ԫ��id����\n");

			num = id.size();
			vector< vector<string> > temp(num);
			for(int i = 0; i < num; i++){
				temp[i].push_back(to_string(id[i]));
			}
			cmdui.drawTable(temp);
		}else if(opt == 7){
			 	// if(d->getTableName() == ""){
				// 	printf("��ѡ��һ�ű�\n");
				// 	break;
				// }
				// d->showDatas();
		}else if(opt == 8){
			if(!d->isChoose()){
				printf("��ѡ��һ�ű�\n");
				getchar();
				getchar();
				continue;
			}
			printf("�������ֶ���?");
			scanf("%s", s);
			name = s;
			printf("������ֵ��");
			scanf("%s", s);
			name2 = s; 
			//��ȡ���� 
			col_name = d->getColName();
			// printf("id\t");
			// for(ll i = 0; i < col_name.size(); i++){
			// 	printf("%s\t", col_name[i].data());
			// }
			// printf("\n"); 
			// //��ȡ���� 
			optStartTime = clock();
			d->query(name, name2, id, queryData);
			// for(ll i = 0; i < queryData.size(); i++){
			// 	printf("%lld\t", id[i]);
			// 	for(ll j = 0; j < queryData[0]. size(); j++){
			// 		printf("%s\t", queryData[i][j].data());
			// 	}
			// 	printf("\n");
			// }
			cmdui.drawTable(queryData, col_name);
		}else if(opt == 9){
			if(!d->isChoose()){
				printf("��ѡ��һ�ű�\n");
				getchar();
				getchar();
				continue;
			}
			printf("�������ֶ���?");
			scanf("%s", s);
			name = s;
			printf("������ֵ��");
			scanf("%s", s);
			name2 = s;
			//��ȡ���� 
			optStartTime = clock();
			d->deleteData(name, name2);
			break;
		}else if(opt == 10){
			if(!d->isChoose()){
				printf("��ѡ��һ�ű�\n");
				getchar();
				getchar();
				continue;
			}
			d->clearTable();
		}else if(opt == 11){
			if(!d->isChoose()){
				printf("��ѡ��һ�ű�\n");
				getchar();
				getchar();
				continue;
			}
			printf("�������ѯ�ֶ���\n");
			scanf("%s", s);
			name = s;
			printf("�������ѯֵ\n");
			scanf("%s", s);
			name2 = s;
			printf("�������޸��ֶ���\n");
			scanf("%s", s);
			str = s;
			printf("�������޸ĺ�ֵ\n");
			scanf("%s", s);
			str2 = s;
			optStartTime = clock();
			d->update(name, name2, str, str2);
		}else if(opt == 12){
			if(!d->isChoose()){
				printf("��ѡ��һ�ű�\n");
				getchar();
				getchar();
				continue;
			}
			printf("��������ʼid\n");
			scanf("%lld", &num);
			printf("��������ֹid, ������-1��ΪĬ��ֵ\n");
			scanf("%lld", &num2);
			//��ȡ���� 
			col_name = d->getColName(); 
			optStartTime = clock();
			d->queryById(id, queryData, num, num2);
			CmdUI::drawTable(queryData, col_name);
		}else if(opt == 13){
			d->clear();
			//��������
			const ll insertNumOnce = 100;
			//����׼��
			string name;
			vector<string> col_name;
			vector<ll> size;
			vector<char> isHash;
			vector<char> isUnique;
			vector< vector< string> > data;
			vector<ll> addr; addr.clear();
			col_name.clear();
			size.clear();
			isHash.clear();
			isUnique.clear();
			data.clear();
			//���ݶ�ȡ
			vector<char> str(10000);
			ll n, m;
			FILE* fp = fopen("test_data.txt", "r");
			rewind(fp);
			fscanf(fp, "%s %lld", &str[0], &n);
			name = &str[0];
			col_name.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%s", &str[0]);
				col_name[i] = &str[0];
			}

			size.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%lld", &size[i]);
			}

			isHash.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%s", &str[0]);
				isHash[i] = str[0];
			}

			isUnique.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%s", &str[0]);
				isUnique[i] = str[0];
			}

			d->createTable(name, col_name, size, isHash, isUnique);
			d->chooseTable(name);
			fscanf(fp, "%lld", &m);
			data.resize(insertNumOnce);
			optStartTime = clock();
			for(ll I = 0; I < m/insertNumOnce; I++){
				for(ll i = 0; i < insertNumOnce; i++){
					data[i].resize(n);
					for(ll j = 0; j < n; j++){
						fscanf(fp, "%s", &str[0]);
						data[i][j] = &str[0];
					}
					addr.push_back(i);
				}
				d->insert(data, id);
			}
			for(ll i = 0; i < m % insertNumOnce; i++){
				data[i].resize(n);
				for(ll j = 0; j < n; j++){
					fscanf(fp, "%s", &str[0]);
					data[i][j] = &str[0];
				}
				addr.push_back(i);
			}
			d->insert(data, id);
			fclose(fp);
		}
		clock_t optEndTime = clock();
		cout << endl << endl;
		for(int i = 0; i < 30; i++) cout << "=";
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);//������ɫ���
		cout << "���β���ִ�к�ʱ ";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		
		double optDuringTime = optEndTime - optStartTime;
		cout << optDuringTime/1000;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);//������ɫ���
		cout << " ��" << endl;
		
		for(int i = 0; i < 30; i++) cout << "=";
		cout << endl;
		cout << "�����������" << endl;
		getchar();
		getchar();
	} 
} 
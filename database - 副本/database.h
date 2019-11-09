/*
 * @Date: 2019-10-06 19:21:17
 * @LastEditors: Phoenix
 * @LastEditTime: 2019-10-29 10:54:09
 */

#ifndef DATABASE_H
#define DATABASE_H
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include "code/statu.h"
#include "code/index.h"
#define ll long long

using namespace std;
class DataBase{	
private:
	//��ǰ��id
	ll table_id;
	Statu * sta;
	vector<Index *> index;
	

	/*************************************************
	Function: check
	Description: ͨ��׼��״̬��麯��
	Return: ״̬�� int
	0 ��ȷ
	-1 δѡ��
	Other: 
	*************************************************/ 
	int check();

	/*************************************************
	Function: getKeyLocation
	Description: ��ȡ��Ӧ�ֶ��������λ�ã��ڼ����ֶΣ��Լ�ǰ��������Ϣ
	Input: 
		key: string ����ѯ���ֶ���
	Output: 
		idx: & ll �ֶμ���λ�ã���0��ʼ�����ڼ����ֶ�
		selfLen: &ll ������
		preSeek: &ll ���ֶ�ǰ�沿�����ݳ��ȣ���������������λ����Чλ��
		lastSeek: &ll ���ֶκ��沿�����ݳ���
		totalSeek: && ���ֶ������ݳ��ȣ���������������λ����Чλ��
	Return: bool �Ƿ�ɹ� 
	Other: 
	*************************************************/ 
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek);
	/**
  * @Date: 2019-10-29 10:34:44
  * @describe: ��
  * @input: ��
  * @output: ��
  * @return: ��
  */
	int traverseQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp, bool isFoozy = false);
	/**
  * @description: 
  * @param {type} 
  * @return: 
  */
	int indexQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp);
public:
	/*************************************************
	Function: DataBase
	Calls: Statu::getInstance;
	Description: ���캯��
	*************************************************/ 
	DataBase();
	
	~DataBase();
	/*************************************************
	Function: createTable
	Description: ����һ�ű��������ñ����������
	Calls: 
	Input: 
	Output: 
	Return: int ״̬�� 
	0 �����ɹ�
	-1
	-1 ���Ѵ���
		-200:
		 	-201: size��С����ȷ
			-202: isHash��С����ȷ
			-203: isUnique��С����ȷ
		-300
			-301�������ļ���ʧ��
	Other: 
	*************************************************/
	int createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique = vector<char>(0));
	
	//void showTables();
	
	/*************************************************
	Function: chooseTable
	Description: ѡ���
	Calls: 
	Input: 
	Output: 
	Return: int ״̬�� 
	0 ѡ��
	-1 ������
	Other: 
	*************************************************/
	int chooseTable(string name);

    /*************************************************
	Function: insert
	Description: ��������
	Input: 
		s: ��ά���飬���������
		id: �����Ľ��id
	Optional:
		name: ���±���
	Return: int ״̬�� 
	0 ����ɹ�
	-1 ������
	Other: 
	*************************************************/
	int insert(const vector< vector<string> > & s, vector<ll> & id, string name = "");
	
	// void showDatas(string name = "");
	
	//ɾ
	int deleteData(string key, string value, string name = "");

	/*************************************************
	Function: query
	Description: ѡ���
	Calls: getKeyLocation
	Input: 
	Output: 
	Return: int ״̬�� 
	0 ѡ��
	-1 ������
	Other: 
	*************************************************/
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, bool isFuzzy = false, string name = "");


	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = "");


	/*************************************************
	Function: update
	Description: ��������
	Calls: query
	Input: 
		key: ��ѯ���ֶ���
		value�� ��ѯ�ֶζ�Ӧֵ
		key2: �������ֶ���
		value2�� �����õ��ֶζ�Ӧ����ֵ
	Optional:
		name: ִ�еĶ�Ӧ�ı�����Ĭ��ִ�е�ǰѡ�б�
	Return: int ״̬�� 
	0 ѡ��
	-1 ������
	Other: 
	*************************************************/
	int update(string key, string value, string key2, string value2, string name = "");
	
	

	/*************************************************
	Function: clear
	Description: ������ݿ�
	Input: 
	Return: int ״̬�� 
	0 ��ճɹ� 
	*************************************************/
	int clear();

	/*************************************************
	Function: clear
	Description: ���һ�ű�����������
	Optional: 
		name: string ѡ�еı�����Ĭ�ϵ�ǰѡ�б���
	Return: int ״̬�� 
	0 ��ճɹ�
	-1 δѡ�б�
	*************************************************/
	int clearTable(string name="");


	bool isChoose();
	//get��������
	string getTableName();

    ll getColNum();
	
	vector<string> getColName();

	//�����÷���
	void showTables();
};

#endif
#ifndef INDEX_H
#define INDEX_H
#define ll long long
#include <vector>
#include <string>
#include <list>
#include "statu.h"
using namespace std;

//����index
class Index{
    private:
       //ָ���ϣľͰ
	   //��һά��ÿ���ֶ� �ڶ�ά��ÿ��Ͱ ����ά��ÿ��ֵ
	   //hashӳ�������ļ���ͷ��ʵ��ַ
	   vector< vector< list<ll> > > bucket;

		static const ll HASHMOD = 1000;
		Statu * sta;
		ll sidx;
		string logFileName;

		//todo: ����һ�� isreadλ�ñ�֤��index�Ƿ�read����create��

		/*************************************************
		Function: hash
		Description: hashӳ�亯��
		Calls: // �����������õĺ����嵥
		Input: 
			value: string ��Ҫ����hash��ֵ
		Output: ��
		Return: long long hash��Ľ��
		Optional: 
			mod: long long ��ϣ�õ�ȡģ��ֵ������hash��ķ�Χ��Ĭ��ȡ����HASHMOD
		Other: ���иò��������ոñ����ִ�������������
		*************************************************/ 
		ll hash(string value, ll mod = Index::HASHMOD);

		/*************************************************
		Function: check
		Description: ����������⺯��
		Calls: // �����������õĺ����嵥
		Return: int ״̬��
		-1 �ö���δread��create ���ڿ�״̬
		*************************************************/ 
		int check();

		/*************************************************
		Function: bucketInit
		Description: ����isHash��Ҫ��ʼ��bucket����
		Calls: // �����������õĺ����嵥
		Return: int ״̬��
		0 �ɹ�
		*************************************************/ 
		int bucketInit();

		int writeLog(FILE * fp, char opt, ll field, ll hashCode, ll addr);
		int ReadLog(FILE * fp, ll & field, ll & hashCode, ll & addr);

    public:
		
		/*************************************************
		Function: Index
		Description: ���캯��
		Calls: // �����������õĺ����嵥
		Input: 
			id: ���������ı�
		Output: ��
		Return: һ��indexʵ��
		Other: �˴�δ����id��Ӧ���Ƿ���ڵ��жϣ�Ӧע��
		*************************************************/ 
        Index(ll id);

		/*************************************************
		Function: createInsertFile
		Description: ����һ����������ļ��Լ�����ĳ�ʼ��׼��
		Calls: // �����������õĺ����嵥
		Input: 
			name: string ���������ı������
			isHahs: const vector<char> & �Ƿ����hash��ÿһ��ı�־���飬ע��˴�T Ϊ����hash�� FΪ������hash
		Output: ��
		Return: int ״̬��
		0:�ɹ�
		Other: ���иò��������ոñ����ִ�������������,����ֱ��ѡ�е�ǰ�����ı���������
		*************************************************/ 
        int createLogFile();

		int read();

		//int save();
		/*************************************************
		Function: choose
		Description: ѡȡһ�����hash����
		Calls: // �����������õĺ����嵥
		Input: 
			name: string ѡȡ�ı������
		Output: ��
		Return: int ״̬��
		Other: ��
		*************************************************/ 
        int choose(string name, ll cnum);

		int insert(const vector< vector<string> > & s, const vector <ll> & addr);

		/*************************************************
		Function: delete
		Description: ɾ����ά���ݱ��ж�Ӧ�����ݵ�ַ
		Calls: // �����������õĺ����嵥
		Input: 
			name: string ѡȡ�ı������
		Output: ��
		Return: int ״̬��
		Other: Ĭ��ÿ����Ч��ַΨһ
		*************************************************/
		int deleteData(const vector< vector<string> > &s, const vector <ll> & addr);

		/*************************************************
		Function: delete8
		Description: ָ���ֶε�ָ��ֵ���ܴ洢λ�õĵ�ַ������Ͱ
		Calls: // �����������õĺ����嵥
		Input: 
			idx: ll �ֶ�λ��
			value: string ֵ
			addr: ll ��ַ����
		Output: ��
		Return: int ״̬��
		Other: 
		*************************************************/
		int query(ll idx, string value, list<ll> & addr);

		int update(ll idx, string value, string value2, ll addr);

		int clear();

};
#endif
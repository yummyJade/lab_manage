#ifndef STATU_H
#define STATU_H
#include <vector>
#include <string>
#include "settings.cpp"
#define ll long long
using namespace std;

//���Խ������������ֱ�Ӵ����ڴ�֮��


class Statu{
private:
	
	static Statu * instance;
	Statu();

	/*************************************************
	Function: save
	Description: ���浱ǰ���б�Ļ�������
	Calls: // �����������õĺ����嵥
	Input: 
	Output: ��
	Return: int ״̬��
	Other: ��
	*************************************************/  
	int save();
	/*************************************************
	Function: read
	Description: ��ȡ��ǰ�������л�������
	Calls:
	Input: 
	Output: ��
	Return: int ״̬��
	Other: ��
	*************************************************/  
	int read();

	/*************************************************
	Function: isNameRepeat
	Description: �жϱ����Ƿ��ظ�
	Calls: // �����������õĺ����嵥
	Input: 
		name�� string �ж��õı���
	Output: ��
	Return: bool �Ƿ����ظ������ظ�Ϊtrue ����Ϊfalse
	Other: ��
	*************************************************/  
	bool isNameRepeat(string name);
public:
	ll table_number; //��ǰ������
	
	vector<string> table_name;
	vector<ll> table_col_num; 
	vector< vector<string> > table_col_name; //����
	vector< vector<ll> > table_col_size; // ÿ����ÿ�г���
	vector< vector<ll> > table_col_pre_size; //ÿ����ÿ�б�ǰ׺��
	vector< vector<char> > isHash;
	vector< vector<char> > isUnique;

	static Statu * getInstance();
	

	/*************************************************
	Function: createTable
	Description: ������
	Calls:
	Input: 
	Output: ��
	Return: int ״̬�� 0�ɹ�
		-1 ���Ѵ���
		-200:
		 	-201: size��С����ȷ
			-202: isHash��С����ȷ
			-203: isUnique��С����ȷ
	Other: ��
	*************************************************/
	int createTable(string name, vector<string> col_name, vector<ll> size, vector<char> isHash, vector<char> isUnique);

	/*************************************************
	Function: getIdx
	Description: ��ñ�����λ��
	Calls:
	Input: 
	Output: ��
	Return: int ������λ�ã���Ϊ
		-1 �������
	Other: ��
	*************************************************/
	int getIdx(string name);

	/*************************************************
	Function: clear
	Description: �����������
	Calls:
	Input: 
	Output: ��
	Return: ״̬��
	Other: ��
	*************************************************/
	int clear();
};


#endif
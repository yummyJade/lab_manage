#ifndef MD5H
#define MD5H

#include <math.h>
#include <Windows.h>
#include <stdio.h>

void ROL(unsigned int &s, unsigned short cx); //32λ��ѭ������ʵ�ֺ���
void ltob(unsigned int &i); //B\L��ת������UINT����
unsigned int *MD5(const char *mStr); //�ӿں�������ִ��������䣬����MD5ʱ���ô˺���
string calcMD5(string rawPassword);

#endif
#pragma once

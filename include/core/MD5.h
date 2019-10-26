#ifndef MD5H
#define MD5H

#include <math.h>
#include <Windows.h>
#include <stdio.h>

void ROL(unsigned int &s, unsigned short cx); //32位数循环左移实现函数
void ltob(unsigned int &i); //B\L互转，接受UINT类型
unsigned int *MD5(const char *mStr); //接口函数，并执行数据填充，计算MD5时调用此函数
string calcMD5(string rawPassword);

#endif
#pragma once

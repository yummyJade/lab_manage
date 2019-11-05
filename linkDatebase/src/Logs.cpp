#include "../../linkDatebase/include/Logs.h"
#include <cstring>
#include <stdlib.h>
#include <iostream>
Logs::Logs(char* tableName)
{
    char *fillS = "Log";
    char* table_name = NULL;
    string temp = string(tableName) + string(fillS);
    table_name = (char*)temp.c_str();
    //cout << table_name << endl;
    FILE* fp = fopen(table_name, "rb+");
    if(fp == NULL) {
        fp = fopen(table_name, "wb+");
    }
    fseek(fp, 0, SEEK_SET);
    if(fread(&this->lastStartPos, sizeof(long long), 1, fp) == 0) {
        this->lastStartPos = -1;
        fseek(fp, 0, SEEK_SET);
        fwrite(&this->lastStartPos, sizeof(long long), 1, fp);
    }
    this->table_name = table_name;
    fclose(fp);
    this->fp = fopen(table_name, "rb+");
    //ctor
}

Logs::~Logs()
{
    fclose(this->fp);
    //dtor
}

void Logs::commitT() {
    char c = 'C';
    fseek(this->fp, 0, SEEK_END);
    fwrite(&c, sizeof(char), 1, this->fp);
    fseek(this->fp, 0, SEEK_SET);
    this->lastStartPos = -1;
    fwrite(&this->lastStartPos, sizeof(long long), 1, this->fp);
    fflush(this->fp);
}

void Logs::startT() {
    char c = 'S';
    fseek(this->fp, 0, SEEK_END);
    this->lastStartPos = ftell(this->fp);
    //cout << this->lastStartPos << endl;
    //fseek(fp, 0, SEEK_END);
    fwrite(&c, sizeof(char), 1, this->fp);
    fseek(this->fp, 0, SEEK_SET);
    fwrite(&this->lastStartPos, sizeof(long long), 1, this->fp);
}

void Logs::endT() {
    char c = 'E';
    fseek(fp, 0, SEEK_END);
    fwrite(&c, sizeof(char), 1, this->fp);
    fflush(this->fp);
}

void Logs::insertL(vector<string> v, char type) {
    fseek(this->fp, 0, SEEK_END);
    for(int i = 0; i < (int)v.size(); i++) {
        int temp = v[i].length();
        if(type != 'I' && type != 'U') {
            return;
        }
        int l = v[i].length();
        //cout << "??????" << v[i].data() << "?????" << l << endl;
        fwrite(&type, sizeof(char), 1, this->fp);
        fwrite(&l, sizeof(int), 1, this->fp);
        fwrite(v[i].data(), sizeof(char), temp, this->fp);
    }
}

vector<string> Logs::init() {
    vector<string> v;
    fseek(this->fp, 0, SEEK_SET);
    fread(&this->lastStartPos, sizeof(long long), 1, this->fp);
    if(this->lastStartPos == -1) {
        return v;
    }
    fseek(this->fp, this->lastStartPos, SEEK_SET);
    char flag;
    fread(&flag, sizeof(char), 1, this->fp);                //???¦Ë?????S
    fread(&flag, sizeof(char), 1, this->fp);
    while(flag == 'I' || flag == 'U') {
        //cout << flag << endl;
        int length;
        fread(&length, sizeof(int), 1, this->fp);
        //printf("length=%d\n", length);
        char c[length+1];
        fread(&c, sizeof(char), length, this->fp);
        c[length] = '\0';
        //printf("string length = %d\n", s.length());
        v.push_back(c);
        fread(&flag, sizeof(char), 1, this->fp);
        if(feof(this->fp)) {
            break;
        }
    }
    char symbol;
    fread(&symbol, sizeof(char), 1, this->fp);
    if(flag == 'E' && symbol == 'C') {
        //????????????Commit??
        v.clear();
        return v;
    } else if(flag == 'E') {
        //????????????End???????¦ÄCommit
        return v;
    } else {
        //?????????¦ÄEnd
        v.clear();
        return v;
    }
}

void Logs::finishAll() {
    this->lastStartPos = -1;
    fseek(this->fp, 0, SEEK_SET);
    fwrite(&this->lastStartPos, sizeof(long long), 1, this->fp);
}

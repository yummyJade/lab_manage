#include <iostream>
#include <cstdio>
#include <windows.h>
#include <string>
#include <vector>
using namespace std;

class CmdUI{
    private:
        //vector<int> width;
        void static drawSplitLine(const vector<int> & width);
        void static drawDataLine(const vector<string> & data, const vector<int> & width);
    public:
        /*
        ����һ�����ݱ��
        */
        void static drawTable(const vector< vector<string> > & data);

        void static drawTable(const vector< vector<string> > & data, const vector<string> & head);
};
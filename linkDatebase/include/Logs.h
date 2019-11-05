#ifndef LOGS_H
#define LOGS_H
#include <cstdio>
#include <vector>
#include <string>
using namespace std;
class Logs
{
    public:
        Logs(char* table_name);
        virtual ~Logs();
        void insertL(vector<string> v, char type);
        void startT();
        void endT();
        void commitT();
        vector<string> init();
        void finishAll();

    protected:

    private:
        char* table_name;
        FILE* fp;
        long long lastStartPos;
};

#endif // LOGS_H

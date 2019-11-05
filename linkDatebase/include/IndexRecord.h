#ifndef INDEXRECORD_H
#define INDEXRECORD_H
#include <vector>
using namespace std;
class IndexRecord
{
    public:
        static IndexRecord* indexRecord;
        static IndexRecord* getInstance();
        virtual ~IndexRecord();
        int addIndex(int id);
        vector<int> queryIndex(int id);
        int init();

    protected:

    private:
        IndexRecord();
};

#endif // INDEXRECORD_H

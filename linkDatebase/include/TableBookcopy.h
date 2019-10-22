#ifndef TABLEBOOKCOPY_H
#define TABLEBOOKCOPY_H

#include <cstdio>
#include <vector>

#include "Bookcopy.h"

using namespace std;

class TableBookcopy {
public:
    virtual ~TableBookcopy();

    static TableBookcopy *tableBookcopy;

    static TableBookcopy *getInstance();

    int insertData(int id, Bookcopy bookcopy, int num);

    vector<Bookcopy> query(int id);

    int update(int id, Bookcopy bookcopy, vector<int> reviseField);

    int update(int id, Bookcopy bookcopy);

    vector<int> updateByBookId(int id, Bookcopy bookcopy, vector<int> reviseField);

    vector<int> updateByBookId(int id, Bookcopy bookcopy);

    vector<Bookcopy> queryByBookId(int id);

protected:

private:
    char *table_name;
    int head_size;
    int data_size;
    int data_number;

    TableBookcopy();
};

#endif // TABLEBOOKCOPY_H

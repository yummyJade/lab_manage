#ifndef BOOKCOPY_H
#define BOOKCOPY_H
#include <string>
using namespace std;

class Bookcopy {
public:
    Bookcopy();

    virtual ~Bookcopy();

    void setId(int id);

    int getId();

    /*void setBookId(int bookId);
    int getBookId();*/

    void setState(int state);

    int getState();

    void setReTime(int reTime);

    int getReTime();

    void setPosition(string position);

    string getPosition();

    void setIsbn(string isbn);

    string getIsbn();

    void setNextId(int nextId);

    int getNextId();

    void show();

private:
    int id;
    //int bookId;
    int state;
    int reTime;
    int nextId;
    string isbn;
    string position;
};

#endif // BOOKCOPY_H

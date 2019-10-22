#ifndef RECORD_H
#define RECORD_H

class Record {
public:
    Record();

    virtual ~Record();

    void setStId(long long stId);

    int getStId();

    void setBoTime(long long boTime);

    long long getBoTime();

    void setId(int id);

    int getId();

    void setBookId(int bookId);

    int getBookId();

    void setNextId(int nextId);

    int getNextId();

    void setReTime(int reTime);

    int getReTime();

    void setState(int state);

    int getState();

    void show();

private:
    long long stId;
    long long boTime;
    int id;
    int bookId;
    int nextId;
    int reTime;
    int state;
};

#endif // RECORD_H

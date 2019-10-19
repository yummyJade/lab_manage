#ifndef BOOKCOPY_H
#define BOOKCOPY_H

class Bookcopy {
private:
    int id;
    char isbn[15];
    int state;
    char *position;
    int reTime;
    int nextId;

public:
    Bookcopy();

    virtual ~Bookcopy();

    void setId(int id);

    int getId();


    const char *getIsbn() const {
        return isbn;
    }

    void setIsbn(std::string isbn);

    void setState(int state);

    int getState();

    void setReTime(int reTime);

    int getReTime();

    void setPosition(std::string position);

    char *getPosition();

    void setNextId(int nextId);

    int getNextId();


};

void Bookcopy::setIsbn(std::string isbn) {
    strcpy(this->isbn, isbn.c_str());
}

void Bookcopy::setPosition(std::string position) {
    strcpy(this->position, position.c_str());
}

#endif // BOOKCOPY_H

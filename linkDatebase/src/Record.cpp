#include "../../linkDatebase/include/Record.h"
#include <iostream>

using namespace std;

Record::Record() {
    //ctor
}

Record::~Record() {
    //dtor
}

void Record::setStId(long long stId) {
    this->stId = stId;
}

int Record::getStId() {
    return stId;
}

void Record::setBoTime(long long boTime) {
    this->boTime = boTime;
}

long long Record::getBoTime() {
    return boTime;
}

void Record::setId(int id) {
    this->id = id;
}

int Record::getId() {
    return id;
}

void Record::setBookId(int bookId) {
    this->bookId = bookId;
}

int Record::getBookId() {
    return bookId;
}

void Record::setNextId(int nextId) {
    this->nextId = nextId;
}

int Record::getNextId() {
    return nextId;
}

void Record::setReTime(int reTime) {
    this->reTime = reTime;
}

int Record::getReTime() {
    return reTime;
}

void Record::setState(int state) {
    this->state = state;
}

int Record::getState() {
    return state;
}

void Record::show() {
    cout << "BookId" << ": " << this->bookId << "  ";
    cout << "BoTime" << ": " << this->boTime << "  ";
    cout << "Id" << ": " << this->id << "  ";
    cout << "NextId" << ": " << this->nextId << "  ";
    cout << "ReTime" << ": " << this->reTime << "  ";
    cout << "State" << ": " << this->state << "  ";
    cout << "StId" << ": " << this->stId << "  ";
    cout << endl;
}

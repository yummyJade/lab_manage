#include "../../linkDatebase/include/Bookcopy.h"
#include <cstdio>
#include <iostream>
#include <cstring>
Bookcopy::Bookcopy()
{
	//ctor
}

Bookcopy::~Bookcopy()
{
	//dtor
}

void Bookcopy::setId(int id) {
	this->id = id;
}
int Bookcopy::getId() {
	return id;
}

/*void Bookcopy::setBookId(int bookId) {
	this->bookId = bookId;
}
int Bookcopy::getBookId() {
	return bookId;
}*/

void Bookcopy::setState(int state) {
	this->state = state;
}
int Bookcopy::getState() {
	return state;
}

void Bookcopy::setReTime(int reTime) {
	this->reTime = reTime;
}
int Bookcopy::getReTime() {
	return reTime;
}

void Bookcopy::setPosition(string position) {
    //printf("xiezheyong:%s\n", position);
	this->position = position;
	//printf("xiezheyong2:%s\n", this->position);
	//printf("xiezhengyong3:%s\n", this->getPosition());
}
string Bookcopy::getPosition() {
	return position;
}

void Bookcopy::setIsbn(string isbn) {
	this->isbn = isbn;
}
string Bookcopy::getIsbn() {
	return isbn;
}

void Bookcopy::setNextId(int nextId) {
	this->nextId = nextId;
}
int Bookcopy::getNextId() {
	return nextId;
}

void Bookcopy::show() {
    /*cout << "show" << endl;
    cout << this->getIsbn();
    cout << this->getPosition() << endl;
    cout << "showend" << endl;*/
    printf("%d %d %d %d ", this->getId(), this->getState(), this->getReTime(), this->getNextId());
    cout << this->getIsbn() << " " << this->getPosition() << endl;
}

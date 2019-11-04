#ifndef MYC1_INPUT_H
#define MYC1_INPUT_H

class Input {
public:
    static int getInt(bool needPositive=false);

	static long long getLongLong();

	static std::string getAssignMaxLengthStr(int maxLength) ;

	static char getChar();


};



#endif //MYC1_INPUT_H

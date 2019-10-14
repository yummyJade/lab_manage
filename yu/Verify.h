//
// Created by THINKPAD on 2019/10/14.
//

#ifndef MYC1_VERIFY_H
#define MYC1_VERIFY_H


class Verify {
public:
    Verify();
    int convertDigtal(char inputStr[]);
    bool IsDigital(char input);     //验证是否为一个合法的整数
    bool optionExist(int choose, int sum);     //判断一个选项是否存在
    bool optionExist(int choose, int optionArr[]);      //同上
};


#endif //MYC1_VERIFY_H

#ifndef MYC1_SIMPLESTRING_H
#define MYC1_SIMPLESTRING_H

#include <string>

class SimpleString {
public:
    /**
     * 判断字符串是否为数字
     * @param str
     * @return
     */
    static bool isDigital(std::string str);


    /**
     * 返回修正后的路径 将斜杠修复为合法的
     * @param str
     * @return
     */
    static std::string fixPath(std::string str);

    static bool isLegalPath(std::string str);
};




#endif //MYC1_SIMPLESTRING_H

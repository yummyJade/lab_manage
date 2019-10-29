#ifndef MYC1_SIMPLESTRING_H
#define MYC1_SIMPLESTRING_H

#include <string>

/**
 * 一个字符串处理的类
 *
 */
class SimpleString {
public:
    /**
     * 判断字符串是否为数字
     * @param str
     * @return
     */
    static bool isDigital(std::string str);
    
    //------------------------------------------------
    //---下面这些主要用于路径处理------------------------

    /**
     * 返回修正后的路径 将斜杠修复为合法的
     * @param str
     * @return
     */
    static std::string fixPath(std::string str);

    /**
     * 判断路径的后缀是否合法(txt,csv)
     * @param str
     * @return
     */
    static bool isLegalPath(std::string str);


    /**
     * 从控制台读取一个路径并以字符串形式返回,要求该路径必须以csv或txt结尾
     * @return
     */
    static std::string readPathFromCmd();

    /**
     *
     * @param path
     * @return
     */
    static std::ifstream readFinFromPath(std::string path);
};




#endif //MYC1_SIMPLESTRING_H

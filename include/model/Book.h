#pragma once

#include "core/SimpleTime.h"


class Book
{

private:
    char type;        //书所属类别A--V，期刊为0
    bool isLend = true;        //是否可借
    int price;        //书本价格,单位分
	int id;
    std::string name;        //书名
    std::string author;        //作者
    std::string isbn;		//ISBN
    std::string press;       // 出版社

public:
	Book();
	~Book();

    //打印书本类型
    std::string printType();
    std::string printIsLend();

	void setBookInfo(std::string namet,std::string authort,std::string isbnt,char typet,double pricet);

    //打印书本基本信息
	void printBookInfo() ;


    /**
     *
     * @param assignInfo 搜索的内容
     * @param type 指定搜索的类型 1-name;2-author
     * @return book数组
     */
    Book *searchBooksByAssignInfo(std::string assignInfo, int type);


	
};


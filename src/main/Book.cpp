#include "Book.h"



Book::Book()
{
}


Book::~Book()
{
}

string Book::printType() {
    string temp;
    switch (type)
    {
        case 'A':temp = "马克思主义、列宁主义、毛泽东思想、邓小平理论"; break;
        case 'B':temp = "哲学、宗教"; break;
        case 'C':temp = "社会科学总论"; break;
        case 'D':temp = "政治、法律"; break;
        case 'E':temp = "军事"; break;
        case 'F':temp = "经济"; break;
        case 'G':temp = "文化、科学、教育、体育"; break;
        case 'H':temp = "语言、文字"; break;
        case 'I':temp = "文学"; break;
        case 'J':temp = "艺术"; break;
        case 'K':temp = "历史、地理"; break;
        case 'N':temp = "自然科学总论"; break;
        case 'O':temp = "数理科学和化学"; break;
        case 'P':temp = "天文学、地球科学"; break;
        case 'Q':temp = "生物科学"; break;
        case 'R':temp = "医药、卫生"; break;
        case 'S':temp = "农业科学"; break;
        case 'T':temp = "工业技术"; break;
        case 'U':temp = "交通运输"; break;
        case 'V':temp = "航空、航天"; break;
        case 'X':temp = "环境科学、安全科学"; break;
        case 'Z':temp = "综合性图书"; break;

        default:temp = "期刊杂志";
            break;
    }
    return temp;
}

string Book::printIsLend(){
    if (isLend) {
        return "可借";
    }
    else if (!isLend) {
        return "不可借";
    }
}

void Book::printBookInfo(){
    cout << id << name << author << isbn << type;
}

void Book::setBookInfo(string namet,string authort,string isbnt,char typet,double pricet) {
    name = namet;
    author = authort;
    isbn = isbnt;
    type = typet;
    price = pricet;
}
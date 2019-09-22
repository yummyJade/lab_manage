#include "../../include/model/User.h"



User::User()
{
}

User::User(int idt, int jobNumt, int maxLendt, int maxTimet, string pwdt, status typet) {
    id = idt;
    jobNum = jobNumt;
    maxLend = maxLendt;
    maxTime = maxTimet;
    pwd = pwdt;

    switch (typet) {
        default:
            break;
    }
}

User::~User()
{
}

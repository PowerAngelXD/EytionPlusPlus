#pragma once
#include <iostream>

namespace excphandling{
    struct excpType{
        int Null = -1;
        int StringError = 0;
        int IntError = 1;
        int DeciError = 2;
        int CharError = 3;
        int BoolError = 4;
    };

    class Excp{
    public:
        excpType eType;
        std::string excpTitle = "Null Excption";
        std::string excpContent = "Null Content";

        Excp(std::string content, std::string type);
    };
}
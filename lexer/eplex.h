/**
 * @file epplex.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief eytion plus plus lexer
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../eppack/error/epperr.h"

namespace epplex{
    struct Token {
        std::string content;
        std::string type;
        int line=1;
        int column=0;

        std::string format();
        std::string simply_format();
    };

    class Lexer{
        std::istream& input;
        int line=2;
        int column=1;
        Token Start();
        Token Num();
        Token Oct();
        Token Hex();
        Token OctHex();
        void Comment();
        Token Identifier();
        Token Sign();
        Token String();
        char get();
        void put(char ch);
    public:
        Lexer(std::istream& input);
        std::vector<Token> getTokenGroup();
    };
};
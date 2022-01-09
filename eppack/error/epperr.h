/**
 * @file east.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief Custom exception file
 * @date 2022-01-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <iostream>
#include <exception>
#include <string>

namespace epperr{
    class Epperr : public std::exception{
        std::string message;
    public:
        Epperr(std::string title, std::string content, int line, int col);
        ~Epperr();
        virtual const char* what();
    };
}
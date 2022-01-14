/**
 * @file epp.cpp
 * @author PowerAngelXd (692732163@qq.com)
 * @brief main file
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) PowerAngelXd
 * 
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "lexer/eplex.h"
#include "parser/parser.h"
#include "eppack/error/epperr.h"

int main(){
    std::wcout<<L"Eytion++ ["<<__DATE__<<"]\nCopyright (c) PowerAngelXd\nNow version: 0.0.4"<<std::endl;
    std::string cmd;
    int code = 10;
    while(true){
        try{
            std::cout<<">> ";
            std::getline(std::cin, cmd);
            if(cmd == "quit") code = 0;
            else {
                std::stringstream ss(cmd);
                epplex::Lexer lexer(ss);
                //std::cout<<ss.str()<<std::endl;
                auto tokens = lexer.getTokenGroup();
                //for(auto token:tokens){std::cout<<token.format()<<std::endl;}
                east::astParser ast(tokens);
                east::StatNode* node = ast.gen_statNode();
                //std::cout<<node->to_string()<<std::endl;
                parser::Parser p(*node);
                p.parse();
                std::cout<<std::endl;
                code = 1;
            }
            if(code == 0)break;
        }
        catch(char const* e){
            std::cout<<"Eytion++ Error:\n"<<e<<std::endl;
        }
        catch(epperr::Epperr eppe){
            std::cout<<eppe.what()<<std::endl;
        }
    }
    return 0;
}
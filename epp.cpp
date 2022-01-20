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
#include <cstring>
#include <getopt.h>
#include "lexer/eplex.h"
#include "parser/parser.h"
#include "eppack/error/epperr.h"
parser::Parser p;
inline void epp_cli(){
    int code = 10;
    std::string cmd;
    std::wcout<<L"Eytion++ ["<<__DATE__<<"]\nCopyright (c) PowerAngelXd\nNow version: 0.0.9\nyou can type 'help' to get 'Eytion++Cli & Eytion++Grammar' help document"<<std::endl;
    while(true){
        if(code == 0) break;
        try{
            std::cout<<">> ";
            std::getline(std::cin, cmd);
            if(cmd == "quit") code = 0;
            else if(cmd == "envc"){
                parser::Parser _p;
                p = _p;
            }
            else if(cmd == "help"){
                std::string help = "Eytion++ Cli Help Dcoument\n"
                                   "Commands:\n"
                                   "quit               Exit the Eytion++ Cli\n"
                                   "envc               Reset the Eytion++Runtime Environment\n"
                                   "help               View Eytion++ HelpDocument\n"
                                   "run <file>         Run a epp-file named 'file'\n"
                                   "view <file>        View a file named 'file'\n"
                                   "-------------------------------------------------\n"
                                   "Eytion++ Grammar:\n"
                                   "addop ::= '+'|'-'\n"
                                   "mulop ::= '/'|'*'|'%'\n"
                                   "primexpr ::= number|identifier|string|char|boolconst|typeof|input\n"
                                   "mulexpr ::= primexpr (mulop primexpr)*\n"
                                   "addexpr ::= addexpr (addop addexpr)*\n"
                                   "expr ::= addexpr | boolexpr | listexpr\n"
                                   "typeof ::= 'typeof' '(' expr ')'\n"
                                   "input ::= 'input' '(' string ')'\n"
                                   "\n"
                                   "outstmt ::= 'out' expr ';'\n"
                                   "vorcstmt ::= 'var'|'const' identifier '=' expr ';'\n"
                                   "assignstmt ::= identifier '=' expr ';'\n"
                                   "deletestmt ::= 'delete' identifier ';'\n"
                                   "blockstmt ::= '{' (stmt)* '}'\n";
                std::cout<<help<<std::endl;
            }
            else if(cmd.find("run") == 0){
                std::string cut;
                cut.assign(cmd.begin() + 4, cmd.end());
                std::ifstream file(cut);
                std::size_t index = cut.find(".epp", cut.size() - ((std::string)".epp").size()); // file suffix check
                if(index == std::string::npos) throw "The file must be in \".epp\" format";
                if(file.fail()) throw "The specified file could not be found";
                else{
                    epplex::Lexer lexer(file);
                    //std::cout<<ss.str()<<std::endl;
                    auto tokens = lexer.getTokenGroup();
                    //for(auto token:tokens){std::cout<<token.format()<<std::endl;}
                    east::astParser ast(tokens);
                    east::StatNode* node = ast.gen_statNode();
                    //std::cout<<node->to_string()<<std::endl;
                    p.stat = *node;
                    p.parse();
                    std::cout<<std::endl;
                    code = 1;
                }
            }
            else if(cmd.find("view") == 0){
                std::string cut;
                cut.assign(cmd.begin() + 5, cmd.end());
                std::ifstream file(cut);
                std::istreambuf_iterator<char> begin(file);
                std::istreambuf_iterator<char> end;
                std::string str(begin, end);
                std::cout<<std::endl<<"view file: "<<cut<<"\n"<<str<<std::endl;
            }
            else {
                std::stringstream ss(cmd);
                epplex::Lexer lexer(ss);
                //std::cout<<ss.str()<<std::endl;
                auto tokens = lexer.getTokenGroup();
                //for(auto token:tokens){std::cout<<token.format()<<std::endl;}
                east::astParser ast(tokens);
                if(east::ExprNode::is_it(ast)){
                    east::ExprNode* repl_node = ast.gen_exprNode();
                    cenv::Calculation calc(p.sset);
                    if(repl_node->addexpr != nullptr){
                        cvisitor::visitor v;
                        v.visitAddExpr(repl_node->addexpr);
                        calc.ins = v.ins; calc.constpool = v.constpool;
                        calc.run();
                    }
                    else if(repl_node->boolexpr != nullptr){
                        cvisitor::visitor v;
                        v.visitBoolExpr(repl_node->boolexpr);
                        calc.ins = v.ins; calc.constpool = v.constpool;
                        calc.run();
                    }
                    else if(repl_node->listexpr != nullptr){
                        cvisitor::visitor v;
                        v.visitListExpr(repl_node->listexpr);
                        calc.ins = v.ins; calc.constpool = v.constpool;
                        calc.run();
                    }
                    if(calc.result[0].first == "__STRING__")
                        std::cout<<calc.constpool[calc.result[0].second];
                    else
                        std::cout<<calc.result[0].second;
                }
                else{
                    east::StatNode* node = ast.gen_statNode();
                    p.stat = *node;
                    p.parse();
                }
                //std::cout<<node->to_string()<<std::endl;
                std::cout<<std::endl;
                code = 1;
            }
        }
        catch(char const* e){
            std::cout<<"Eytion++ Error:  "<<e<<std::endl;
        }
        catch(epperr::Epperr eppe){
            std::cout<<eppe.what()<<std::endl;
        }
    }
}

int main(int argc, char *argv[]){
    std::string cmd;
    if(argc >= 2){
        if(strcmp(argv[1], "-v")==0 || strcmp(argv[1], "-version")==0) std::cout<<"version => debug-0.0.9"<<std::endl;
        else if(strcmp(argv[1], "-r")==0 || strcmp(argv[1], "-run")==0){
            std::ifstream file(argv[2]);
            std::size_t index = ((std::string)argv[2]).find(".epp", ((std::string)argv[2]).size() - ((std::string)".epp").size()); // file suffix check
            if(index == std::string::npos) {std::cout<<"error:  The file must be in \".epp\" format"<<std::endl; goto end;}
            if(file.fail()) {std::cout<<"error:  The specified file could not be found"<<std::endl; goto end;}
            else{
                epplex::Lexer lexer(file);
                //std::cout<<ss.str()<<std::endl;
                auto tokens = lexer.getTokenGroup();
                //for(auto token:tokens){std::cout<<token.format()<<std::endl;}
                east::astParser ast(tokens);
                east::StatNode* node = ast.gen_statNode();
                //std::cout<<node->to_string()<<std::endl;
                p.stat = *node;
                p.parse();
                std::cout<<std::endl;
            }
        }
        else if(strcmp(argv[1], "-cli") == 0) epp_cli();
        else if(strcmp(argv[1], "-?")==0 || strcmp(argv[1], "-h")==0 || strcmp(argv[1], "-help")==0){
            std::string help = "Eytion++ Options Helper"
                               "Options:\n"
                               "-v, -version          Output the version of Eytion++ on the screen\n"
                               "-?, -h, -help         View Eytion++ options help\n"
                               "-r, -run <file>       Run a file named 'file'\n"
                               "-cli                  Run Eytion++ cli";
            std::cout<<help<<std::endl;
        }
    }
    else std::cout<<"parameter '"<<argv[1]<<"' that does not exist"<<std::endl;
    end:
    return 0;
}
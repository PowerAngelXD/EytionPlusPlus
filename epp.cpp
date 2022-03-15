/**
 * @file epp.cpp
 * @author PowerAngelXd (692732163@qq.com)
 * @brief main file
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) PowerAngelXd
 * 
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include "lexer/eplex.h"
#include "parser/parser.h"
#include "eppack/error/epperr.h"
#include "eppack/tools/configloader.h"
//#define EPP_DEBUG

#ifdef _WIN32
void getAllFile(std::string path, std::vector<std::string>& files){
    long hFile = 0;
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){
        do{
            files.push_back(p.assign(path).append("\\").append(fileinfo.name));
        } while (_findnext(hFile, &fileinfo) == 0);

        _findclose(hFile);
    }
}
#endif

parser::Parser p;
configloader::Loader loader("resource/config/common.ecfg");
#ifdef EPP_DEBUG
int main(){
#else
inline void epp_cli(){
#endif
    int code = 10;
    std::string cmd;
    char work_path[256];
    std::wcout<<L"Eytion++ [ Build Time:"<<__DATE__<<" "<<__TIME__<<"]\nCopyright (c) PowerAngelXd\nNow version: 0.2.2\nyou can type 'help' to get 'Eytion++Cli & Eytion++Grammar' help document\n"<<std::endl;
    while(true){
        if(code == 0) break;
        try{
            getcwd(work_path, 256); 
            if(loader.getData().cli_nameshow == true){
                std::cout<<"[Epp Cli]";
            }
            if(loader.getData().cli_workspace == true){
                std::cout<<work_path;
            }
            std::cout<<"> ";
            std::getline(std::cin, cmd);
            if(cmd == "exit" || cmd == ":e") {
                if(loader.getData().cli_exittip == true){
                    std::cout<<"Do you really want to quit this program? (you can turn off the 'cli-exittip' option in the configuration file and don't see this prompt again)"<<std::endl;
                    std::cout<<"yes[y/other keys]   no[n/N]"<<std::endl;
                    std::cout<<"> ";
                    char k = getchar();
                    if(k == 'n' || k == 'N');
                    else code = 0;
                }
                else code = 0;
            }
            else if(cmd == "ls"){
                #ifdef _WIN32
                std::vector<std::string> files;
                std::string crt_path;
                std::cout<<"================================================="<<std::endl;
                for(int i = 0; i < strlen(work_path); i++){
                    crt_path.push_back(work_path[i]);
                }
                getAllFile(crt_path, files);
                for(auto file: files) std::cout<<file<<std::endl;
                std::cout<<"================================================="<<std::endl;
                #elif __linux__
                system("ls");
                #endif

            }
            else if(cmd == "update"){
                system("python resource/scripts/updater.py");
            }
            else if(cmd == "envc"){
                parser::Parser _p;
                p = _p;
            }
            else if(cmd == "redcfg"){
                loader = configloader::Loader("resource/config/common.ecfg");
                std::cout<<"\033[42;37mThe configuration file was successfully reloaded!"<<"\033[0m"<<std::endl;
            }
            else if(cmd == "help"){
                std::string help = "Eytion++ Cli Help Dcoument\n"
                                   "Commands:\n"
                                   "exit/:e            Exit the Eytion++ Cli\n"
                                   "envc               Reset the Eytion++Runtime Environment\n"
                                   "help               View Eytion++ HelpDocument\n"
                                   "run <file>         Run a epp-file named 'file'\n"
                                   "view <file>        View a file named 'file'\n"
                                   "-------------------------------------------------\n"
                                   "Eytion++ Grammar:\n"
                                   "addop ::= '+'|'-'\n"
                                   "mulop ::= '/'|'*'|'%'\n"
                                   "siadexpr ::= (identifier) '++' | '--' (identifier)\n"
                                   "primexpr ::= number|identifier|string|char|boolconst|typeof|input|siadexpr\n"
                                   "mulexpr ::= primexpr (mulop primexpr)*\n"
                                   "addexpr ::= addexpr (addop addexpr)*\n"
                                   "expr ::= addexpr | boolexpr | listexpr\n"
                                   "typeof ::= 'typeof' '(' expr ')'\n"
                                   "input ::= 'input' '(' string ')'\n"
                                   "\n"
                                   "stmt ::= outstmt|vorcstmt|assignstmt|deletestmt|blockstmt|ifstmt|whilestmt|repeatstmt|breakstmt\n"
                                   "outstmt ::= 'out' expr ';'\n"
                                   "vorcstmt ::= 'var'|'const' identifier (':' type) '=' expr ';'\n"
                                   "assignstmt ::= identifier '=' expr ';'\n"
                                   "deletestmt ::= 'delete' identifier ';'\n"
                                   "blockstmt ::= '{' (stmt)* '}'\n"
                                   "ifstmt ::= 'if' '(' boolexpr ')' blockstmt|stmt\n"
                                   "elsestmt ::= 'else' blockstmt\n"
                                   "whilestmt ::= 'while' '(' boolexpr ')' blockstmt|stmt\n"
                                   "repeatstmt ::= 'repeat' '(' addexpr ')' blockstmt|stmt\n"
                                   "breakstmt ::= 'break' ';'\n"
                                   "for_each_stmt ::= 'for_each' '(' 'var' identifier ':' identifier ')' blockstmt";
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
                parser::Parser new_p;
                p = new_p;
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
            else if(cmd.find("cd") == 0){
                std::string cut;
                cut.assign(cmd.begin() + 3, cmd.end());
                chdir(cut.c_str());
            }
            else {
                std::ofstream tfile("debug/tokens/tokens.edebug");
                std::stringstream ss(cmd);
                epplex::Lexer lexer(ss);
                //std::cout<<ss.str()<<std::endl;
                auto tokens = lexer.getTokenGroup();
                std::vector<std::string> tg_msg;
                for(auto token:tokens){tg_msg.push_back(token.format());}
                for(auto msg:tg_msg){tfile<<msg<<std::endl;}
                tfile.close();
                if(loader.getData().debug_echotokeng == true) {
                    std::cout<<"DebugMode: TokenGroups\n";
                    for(auto token:tokens){std::cout<<token.format()<<",";}
                    std::cout<<std::endl;
                }
                east::astParser ast(tokens);
                if(east::ValExprNode::is_it(ast) && !east::AssignStmtNode::is_it(ast)){
                    east::ValExprNode* repl_node = ast.gen_valExprNode();
                    if(loader.getData().debug_echoast == true){
                        std::cout<<"DebugMode: Ast\n";
                        std::cout<<repl_node->to_string()<<std::endl;
                    }
                    cenv::Calculation calc(p.sset);
                    cvisitor::visitor v;
                    //std::cout<<repl_node->to_string()<<std::endl;
                    if(repl_node->addexpr != nullptr)
                        v.visitAddExpr(repl_node->addexpr);
                    else if(repl_node->boolexpr != nullptr)
                        v.visitBoolExpr(repl_node->boolexpr);
                    else if(repl_node->listexpr != nullptr)
                        v.visitListExpr(repl_node->listexpr);
                    calc.ins = v.ins; calc.constpool = v.constpool;
                    calc.run();
                    p.sset = calc.sset;
                    if (calc.isArray()){
                        if (calc.result[0].first == "__STRING__"){
                            for (int i = 0; i < calc.result.size(); i++){
                                std::cout << calc.constpool[calc.result[i].second];
                            }
                        }
                        else if (calc.result[0].first == "__CHAR__"){
                            for (int i = 0; i < calc.result.size(); i++){
                                std::cout << calc.constpool[calc.result[i].second];
                            }
                        }
                        else{
                            for (int i = 0; i < calc.result.size(); i++){
                                std::cout << calc.result[i].second;
                            }
                        }
                    }
                    else{
                        if (calc.result[0].first == "__STRING__")
                            std::cout << calc.constpool[calc.result[0].second];
                        else if (calc.result[0].first == "__CHAR__")
                            std::cout << calc.constpool[calc.result[0].second];
                        else
                            std::cout << calc.result[0].second;
                    }
                    std::cout<<std::endl;
                }
                else{
                    east::StatNode* node = ast.gen_statNode();
                    if(loader.getData().debug_echoast == true){
                        std::cout<<"DebugMode: Ast\n";
                        std::cout<<node->to_string()<<std::endl;
                    }
                    std::ofstream afile("debug/ast/ast.edebug");
                    afile<<node->to_string()<<std::endl;
                    afile.close();
                    p.stat = *node;
                    p.parse();
                }
                std::cout<<std::endl;
                code = 1;
            }
        }
        catch(char const* e){
            std::cout<<"Eytion++ Error:  "<<e<<std::endl;
        }
        catch(std::string e){
            std::cout<<"Eytion++ Error:  "<<e<<std::endl;
        }
        catch(epperr::Epperr eppe){
            if(loader.getData().cli_errorhighlight == true) std::cout<<"\033[41;37m";
            std::cout<<eppe.what()<<"\033[0m"<<std::endl;
        }
        catch(epperr::EppClierr eppce){
            if(loader.getData().cli_errorhighlight == true) std::cout<<"\033[41;37m";
            std::cout<<eppce.what()<<"\033[0m"<<std::endl;
        }
    }
}

#ifndef EPP_DEBUG
int main(int argc, char *argv[]){
    std::string cmd;
    if(argc >= 2){
        if(strcmp(argv[1], "-v")==0 || strcmp(argv[1], "-version")==0) std::cout<<"version => dev-0.2.2"<<std::endl;
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
    else{
        std::cout<<"If you want to run EytionCli you can type './e++ -cli'"<<std::endl;
        std::cout<<"If you want to run the \".epp\" file, you can type \"./e++ -r (filename)\""<<std::endl;
    }
    end:
    return 0;
}
#endif
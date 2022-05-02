/**
 * @file parser.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief parser file
 * @date 2022-01-012
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "../eppack/calc/calc.h"
#include "../eppack/calc/calcvisitor.h"
#include "../eppack/excphg/excphg.h"

namespace parser{
    class Parser;
    std::string getTypestring(std::string type);
    cenv::Calculation getCalc(east::ValExprNode node, var::ScopeSet sset);    // Used to get the calculated value
    efunc::Efunction getCalc(east::FuncDefineExprNode node, var::ScopeSet sset);
    cenv::Calculation getCalc(east::AssignExprNode node, var::ScopeSet sset);
    cenv::Calculation getCalc(east::BoolExprNode node, var::ScopeSet sset);   // Used to get the calculated value(just boolean)

    namespace funcParser{
        cenv::Calculation call(Parser p, east::FuncCallExprNode* node);
    }

    /**
     * @brief Used to parse each stmt
     */
    class Parser{
        int _if_control = -1;
    public:
        east::StatNode stat;
        var::ScopeSet sset;
        var::ScopeSet user_sset; // User defined sset

        void parse_OutStmt(east::OutStmtNode* stmt);      // Migration complete
        void parse_VorcStmt(east::VorcStmtNode* stmt);      // Migration complete
        void parse_ExprStmt(east::ExprStmtNode* stmt);      // Migration complete
        void parse_DeleteStmt(east::DeleteStmtNode* stmt);      // Migration complete
        void parse_BlockStmt(east::BlockStmtNode* stmt);
        void parse_IfStmt(east::IfStmtNode* stmt);
        void parse_ElseIfStmt(east::ElseifStmtNode* stmt);
        void parse_ElseStmt(east::ElseStmtNode* stmt);
        void parse_BreakStmt(east::BreakStmtNode* stmt);
        void parse_WhileStmt(east::WhileStmtNode* stmt);
        void parse_RepeatStmt(east::RepeatStmtNode* stmt);
        void parse_ForEachStmt(east::ForEachStmtNode* stmt);
        void parse_ForStmt(east::ForStmtNode* stmt);     // Migration complete

        cenv::Calculation parse();
    };
}
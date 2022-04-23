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
    cenv::Calculation getCalc(east::ValExprNode node, var::ScopeSet sset);    // Used to get the calculated value
    cenv::Calculation getCalc(east::AssignExprNode node, var::ScopeSet sset);
    cenv::Calculation getCalc(east::BoolExprNode node, var::ScopeSet sset);   // Used to get the calculated value(just boolean)
    class BifParser{
        public:
            east::BifInstanceNode* bif;
            var::ScopeSet _sset;
            var::ScopeSet _user_sset; // User defined sset

            BifParser(east::BifInstanceNode* bif, var::ScopeSet sset);
            cenv::Calculation bif_Sytem();
        };
    class Parser{
        int _if_control = -1;
    public:
        east::StatNode stat;
        var::ScopeSet sset;
        var::ScopeSet user_sset; // User defined sset

        void parse_OutStmt(east::OutStmtNode* stmt);
        void parse_VorcStmt(east::VorcStmtNode* stmt);
        void parse_ExprStmt(east::ExprStmtNode* stmt);
        void parse_DeleteStmt(east::DeleteStmtNode* stmt);
        void parse_BlockStmt(east::BlockStmtNode* stmt);
        void parse_IfStmt(east::IfStmtNode* stmt);
        void parse_ElseIfStmt(east::ElseifStmtNode* stmt);
        void parse_ElseStmt(east::ElseStmtNode* stmt);
        void parse_BreakStmt(east::BreakStmtNode* stmt);
        void parse_WhileStmt(east::WhileStmtNode* stmt);
        void parse_RepeatStmt(east::RepeatStmtNode* stmt);
        void parse_ForEachStmt(east::ForEachStmtNode* stmt);
        void parse_ForStmt(east::ForStmtNode* stmt);

        void parse();
    };
}
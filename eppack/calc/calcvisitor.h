/**
 * @file calcvisitor.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief Calculation
 * @date 2022-01-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../../parser/east.h"
#include "calc.h"

namespace cvisitor{
    class visitor{
    public:
        east::ValExprNode* expr;
        std::vector<cenv::calc_ins> ins;
        std::vector<std::string> constpool;

        void visitNumber(epplex::Token* token);
        void visitString(epplex::Token* token);
        void visitAssign(epplex::Token* token);
        void visitChar(epplex::Token* token);
        void visitWholeExprNode(east::WholeExprNode* node);
        void visitAssignExpr(east::AssignExprNode* node);
        void visitIden(east::IdentifierNode* node);
        void visitSiad(east::SelfIaDExprNode* node);
        void visitTpof(east::TypeOfExprNode* node);
        void visitInput(east::InputExprNode* node);
        void visitTypeTo(east::TypeToExprNode* node);
        void visitLen(east::LenExprNode* node);
        void visitAddOp(east::AddOpNode* node);
        void visitMulOp(east::MulOpNode* node);
        void visitCmpOp(east::CmpOpNode* node);
        void visitBoolOp(east::BoolOpNode* node);
        void visitCmpExpr(east::CmpExprNode* node);
        void visitBoolExpr(east::BoolExprNode* node);
        void visitPrimExpr(east::PrimExprNode* node);
        void visitMulExpr(east::MulExprNode* node);
        void visitAddExpr(east::AddExprNode* node);
        void visitListExpr(east::ListExprNode* node);
    };
}
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
        east::ExprNode* expr;
        std::vector<cenv::calc_ins> ins;

        void visitNumber(epplex::Token* token);
        void visitString(epplex::Token* token);
        void visitChar(epplex::Token* token);
        void visitIden(east::IdentifierNode* node);
        void visitAddOp(east::AddOpNode* node);
        void visitMulOp(east::MulOpNode* node);
        void visitPrimExpr(east::PrimExprNode* node);
        void visitMulExpr(east::MulExprNode* node);
        void visitAddExpr(east::AddExprNode* node);
    };
}
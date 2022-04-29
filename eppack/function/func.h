/**
 * @file func.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief function file
 * @date 2022-01-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include "../../parser/east.h"

namespace efunc{
    class Efunction{
    public:
        std::string ret_type = "__NULL__";
        std::vector<east::FuncDefineExprNode::Para*> nor_para;
        std::vector<east::ValExprNode*> act_para;
        east::BlockStmtNode* body;
    };
}
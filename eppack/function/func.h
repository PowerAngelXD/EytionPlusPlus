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
        std::string name = "__FUNC__";
        std::string ret_type = "__NULL__";
        std::vector<std::string> nor_para;
        std::vector<std::string> norp_type;
        std::vector<east::ExprNode> act_para;
        east::BlockStmtNode body;
    };
}
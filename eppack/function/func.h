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
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "../../parser/east.h"
#include "../../parser/parser.h"

namespace efunc{
    class Efunction{
    public:
        std::string name = "__FUNC__";
        std::vector<std::string> nor_para;
        std::vector<east::ExprNode> act_para;
        east::BlockStmtNode body;
        scope::ScopeSet func_sset;
        std::vector<cenv::calc_unit> ret;

        void exec();
    };
}
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
    class Parser{
        int _if_control = -1;
    public:
        east::StatNode stat;
        var::ScopeSet sset;
        var::ScopeSet user_sset; // User defined sset

        void parse();
    };
}
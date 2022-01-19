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
#include "east.h"
#include "../eppack/calc/calc.h"
#include "../eppack/calc/calcvisitor.h"

namespace parser{
    class Parser{
    public:
        east::StatNode stat;
        scope::ScopeSet sset;

        void parse();
    };
}
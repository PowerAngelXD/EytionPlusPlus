/**
 * @file calc.h
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
#include "../error/epperr.h"
#include "../var/var.h"

namespace cenv{
    typedef std::string calc_type; // __INT__ __DECI__ __STRING__ __CHAR__ __BOOL__ __NULL__
    typedef std::string calc_instr; // __PUSH__ __PUSHA__ __TPOF__ __INPUT__
    typedef std::pair<calc_type, float> calc_unit;

    struct calc_ins{
        calc_instr instr = "__NULL__"; // symbol of calculation
        calc_unit unit;
        std::string para = "__NULL__";
        int line, column;
    };

    class Calculation{
        bool is_array = false;
        scope::ScopeSet sset;
    public:
        Calculation(scope::ScopeSet sset_);

        std::vector<calc_ins> ins;
        std::vector<calc_unit> env;
        std::vector<std::string> constpool;
        int env_top = 0;
        std::vector<calc_unit> result;

        void push(calc_unit cu);
        calc_unit pop();
        void run();
        bool isArray();
    };
}
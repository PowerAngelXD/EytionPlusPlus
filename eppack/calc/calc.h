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

namespace cenv{
    typedef std::string calc_type; // __INT__ __DECI__ __STRING__ __CHAR__ __BOOL__ __NULL__
    typedef std::pair<calc_type, float> calc_unit;

    struct calc_ins{
        std::string sym = "__NULL__"; // symbol of calculation
        calc_unit unit;
        std::string para = "__NULL__";
        int line, column;
    };

    class Calculation{
    public:
        std::vector<calc_ins> ins;
        std::vector<calc_unit> env;
        std::vector<std::string> constpool;
        int env_top = 0;
        calc_unit result = calc_unit("__NULL__", 0.0);

        void push(calc_unit cu);
        calc_unit pop();
        void run();
    };
}
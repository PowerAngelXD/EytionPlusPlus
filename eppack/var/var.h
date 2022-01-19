/**
 * @file var.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief variable file
 * @date 2022-01-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <iostream>
#include <vector>
#include <type_traits>
#include "../error/epperr.h"

namespace var{
    class Value{
        std::vector<int> int_val;
        std::vector<float> deci_val;
        std::vector<char> ch_val;
        std::vector<std::string> str_val;
        std::vector<bool> bool_val;

        bool is_array = false;
        bool is_const = false;
        std::string type = "__NULL__"; // __DECI__ __STRING__ __INT__ __CHAR__ __BOOL__
    public:
        int line, column;

        int val_int();
        std::vector<int> val_int_array();
        float val_deci();
        std::vector<float> val_deci_array();
        char val_char();
        std::vector<char> val_char_array();
        std::string val_string();
        std::vector<std::string> val_string_array();
        bool val_bool();
        std::vector<bool> val_bool_array();

        bool isArray();
        bool isConst();
        std::string getType();
        void set_val(int val);
        void set_val(std::string val);
        void set_val(float val);
        void set_val(bool val);
        void set_val(char val);
        void set_lc(int line_, int col_);
    };
}

namespace scope{
    class Scope{
        std::string name;
    public:
        std::vector<std::pair<std::string, var::Value>> vars;
        std::vector<std::string> identifier_table;
        Scope(std::string name_);
        /**
         * @brief create a new variable
         * */
        void new_var(std::string name, var::Value val);
        /**
         * @brief get the name of the scope
         * */
        std::string get_name();
        /**
         * @brief finds if there is a variable named Target in the Scope
         * @return true / false
         * */
        bool find(std::string target);
        int findI(std::string target);
    };

    class ScopeSet{
        int deep_count = 0;
    public:
        std::vector<Scope> scope_pool;
        ScopeSet();
        bool findInAllScope(std::string name);
        int findInAllScopeI(std::string name);
        void next();
        void back();
        void remove();
        void newScope(std::string name);
        int getDeep();
    };
}
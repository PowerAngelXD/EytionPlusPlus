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
#include "../function/func.h"

namespace var{
    class Value{
        std::vector<int> int_val;
        std::vector<float> deci_val;
        std::vector<std::string> ch_val;
        std::vector<std::string> str_val;
        std::vector<bool> bool_val;
        std::vector<efunc::Efunction> func_val;

        bool is_array = false;
        bool is_const = false;
        bool is_func = false;
        std::string type = "__NULL__"; // __DECI__ __STRING__ __INT__ __CHAR__ __BOOL__ __FUNC__
    public:
        int line, column;
        int len = 1;
        Value(bool is_arr, bool is_con);
        Value(bool is_arr, bool is_con, std::string _type);
        Value(bool is_arr, bool is_con, std::string _type, int val);
        Value(bool is_arr, bool is_con, std::string _type, float val);
        Value(bool is_arr, bool is_con, std::string _type, std::string s, bool ischar);
        Value(bool is_arr, bool is_con, std::string _type, bool val);
        Value(bool is_arr, bool is_con, std::string _type, efunc::Efunction val);
        int val_int();
        std::vector<int> val_int_array();
        float val_deci();
        std::vector<float> val_deci_array();
        std::string val_char();
        std::vector<std::string> val_char_array();
        std::string val_string();
        std::vector<std::string> val_string_array();
        bool val_bool();
        std::vector<bool> val_bool_array();
        efunc::Efunction val_func();
        std::vector<efunc::Efunction> val_func_array();

        bool isArray();
        bool isConst();
        bool isFunc();
        std::string getType();
        void set_val(int val);
        void set_val(std::string val, bool is_ch);
        void set_val(float val);
        void set_val(bool val);
        void set_val(efunc::Efunction val);
        void set_lc(int line_, int col_);

        void arr_setVal(int val, int pos);
        void arr_setVal(float val, int pos);
        void arr_setVal(std::string val, int pos, bool is_ch);
        void arr_setVal(bool val, int pos);
        void arr_setVal(efunc::Efunction val, int pos);

        void arr_addVal(int val);
        void arr_addVal(float val);
        void arr_addVal(std::string val, bool is_ch);
        void arr_addVal(bool val);
        void arr_addVal(efunc::Efunction val);
    };
}

namespace scope{
    class Scope{
    public:
        std::string name;
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
        void assign(std::string name, var::Value value);
    };

    class UserScope: public Scope{
    public:
        std::string name;
        east::StatNode stat;
        UserScope(std::string name_, east::StatNode stat_);
    };

    class ScopeSet{
        int deep_count = 0;
    public:
        std::vector<Scope> scope_pool;
        std::vector<UserScope> user_scope_pool;
        ScopeSet();

        /**
         * @brief Find out whether the specified identifier is within this scope and all parent scopes of this scope
         * @param name target identifier's name
         * @return bool
         */
        bool findInAllScope(std::string name);
        /**
         * @brief Find the specified identifier and return the index of the scope
         * @param name target identifier's name
         * @return int 
         */
        int findInAllScopeI(std::string name);
        void next();
        void back();
        void remove();
        /**
         * @brief Create a new scope
         * 
         * @param name scope name
         */
        void newScope(std::string name);
        int getDeep();

        /**
         * @brief A scope with the specified name was found in the user scope pool
         * 
         * @param name target scope's name
         * @return true 
         * @return false 
         */
        bool findInUScope(std::string name);
        /**
         * @brief A scope with the specified name was found in the user scope pool, And return the index
         * 
         * @param name 
         * @return int index
         */
        int findInUScopeI(std::string name);
    };
}
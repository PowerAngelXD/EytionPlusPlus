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
    class UserScope;

    class Value{
        std::vector<int> int_val;
        std::vector<float> deci_val;
        std::vector<std::string> ch_val;
        std::vector<std::string> str_val;
        std::vector<bool> bool_val;
        std::vector<efunc::Efunction> func_val;
        std::vector<UserScope> us_val;
        std::vector<Value> val_val;

        bool is_array = false;
        int nullval = 1;
        bool is_const = false;
        bool is_func = false;
        std::string type = "__NULL__"; // __DECI__ __STRING__ __INT__ __CHAR__ __BOOL__ __FUNC__
    public:
        int line, column;
        int len = 1;
        Value(bool is_arr, bool is_con);
        Value(bool is_arr, bool is_con, std::string _type);
        Value(bool is_null);
        // init value
        Value(bool is_arr, bool is_con, int val);
        Value(bool is_arr, bool is_con, float val);
        Value(bool is_arr, bool is_con, std::string s, bool ischar);
        Value(bool is_arr, bool is_con, bool val);
        Value(bool is_arr, bool is_con, efunc::Efunction val);
        Value(bool is_arr, bool is_con, UserScope val);
        Value(bool is_arr, bool is_con, Value val);
        // init list
        Value(bool is_arr, bool is_con, std::vector<int> val);
        Value(bool is_arr, bool is_con, std::vector<float> val);
        Value(bool is_arr, bool is_con, std::vector<std::string> s, bool ischar);
        Value(bool is_arr, bool is_con, std::vector<bool> val);
        Value(bool is_arr, bool is_con, std::vector<efunc::Efunction> val);
        Value(bool is_arr, bool is_con, std::vector<UserScope> val);
        Value(bool is_arr, bool is_con, std::vector<Value> val);

        int getValueOfInt();
        std::vector<int>getValueOfIntArray();
        float getValueOfDecimal();
        std::vector<float> getValueOfDecimalArray();
        std::string getValueOfChar();
        std::vector<std::string> getValueOfCharArray();
        std::string getValueOfString();
        std::vector<std::string> getValueOfStringArray();
        bool getValueOfBool();
        std::vector<bool> getValueOfBoolArray();
        efunc::Efunction getValueOfFunc();
        std::vector<efunc::Efunction> getValueOfFuncArray();
        UserScope getValueOfUScope();
        std::vector<UserScope> getValueOfUScopeArray();
        Value getValueOfValue();
        std::vector<Value> getValueOfValueArray();

        bool isArray();
        bool isConst();
        bool isFunc();
        std::string getType();
        void set_val(int val);
        void set_val(std::string val, bool is_ch);
        void set_val(float val);
        void set_val(bool val);
        void set_val(efunc::Efunction val);
        void set_val(UserScope val);
        void set_lc(int line_, int col_);

        void arr_setVal(int val, int pos);
        void arr_setVal(float val, int pos);
        void arr_setVal(std::string val, int pos, bool is_ch);
        void arr_setVal(bool val, int pos);
        void arr_setVal(efunc::Efunction val, int pos);
        void arr_setVal(UserScope val, int pos);

        void arr_addVal(int val);
        void arr_addVal(float val);
        void arr_addVal(std::string val, bool is_ch);
        void arr_addVal(bool val);
        void arr_addVal(efunc::Efunction val);
        void arr_addVal(UserScope val);
    };

    class Scope{
    public:
        typedef std::pair<std::string, var::Value> varUnit;
        std::string name;
        std::vector<varUnit> vars;
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
        void deleteVar(std::string name);
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

        // Latest convenience function:

        /**
         * @brief Gets the variable with the specified name
         * 
         * @param name target variable name
         * @return Target object
         */
        Scope::varUnit getTargetVar(std::string name);

        /**
         * @brief Assign a value to the target variable
         * 
         * @param target target name
         * @param val value
         */
        void assignValue(std::string target, var::Value val);

        /**
         * @brief Create a variable
         * 
         * @param name target name
         * @param val value
         */
        void createVariable(std::string name, var::Value val);

        /**
         * @brief Delete a variable
         * 
         * @param name target name
         */
        void deleteVariable(std::string name);
    };
}
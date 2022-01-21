#include "var.h"

//var
var::Value::Value(bool is_arr, bool is_con) : is_array(is_arr), is_const(is_con) {}
bool var::Value::isArray(){
    return is_array;
}
bool var::Value::isConst(){
    return is_const;
}
std::string var::Value::getType(){
    return type;
}
int var::Value::val_int(){
    if(type == "__INT__"){
        if(!is_array) return int_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Integer", line, column);
}
std::vector<int> var::Value::val_int_array(){
    if(type == "__INT__"){
        if(is_array) return int_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Integer(array)", line, column);
}
float var::Value::val_deci(){
    if(type == "__DECI__"){
        if(!is_array) return deci_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Decimal", line, column);
}
std::vector<float> var::Value::val_deci_array(){
    if(type == "__DECI__"){
        if(is_array) return deci_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Decimal(array)", line, column);
}
char var::Value::val_char(){
    if(type == "__CHAR__"){
        if(!is_array) return ch_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Char", line, column);
}
std::vector<char> var::Value::val_char_array(){
    if(type == "__CHAR__"){
        if(is_array) return ch_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Char(array)", line, column);
}
std::string var::Value::val_string(){
    if(type == "__STRING__"){
        if(!is_array) return str_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not String", line, column);
}
std::vector<std::string> var::Value::val_string_array(){
    if(type == "__STRING__"){
        if(is_array) return str_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not String(array)", line, column);
}
bool var::Value::val_bool(){
    if(type == "__BOOL__"){
        if(!is_array) return bool_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Bool", line, column);
}
std::vector<bool> var::Value::val_bool_array(){
    if(type == "__BOOL__"){
        if(is_array) return bool_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Bool(array)", line, column);
}
void var::Value::set_val(int val){
    if(int_val.empty())
        int_val.push_back(val);
    else
        int_val[0] = val;
    type = "__INT__";
}
void var::Value::set_val(std::string val){
    if(str_val.empty())
        str_val.push_back(val);
    else
        str_val[0] = val;
    type = "__STRING__";
}
void var::Value::set_val(float val){
    if(deci_val.empty())
        deci_val.push_back(val);
    else
        deci_val[0] = val;
    type = "__DECI__";
}
void var::Value::set_val(bool val){
    if(bool_val.empty())
        bool_val.push_back(val);
    else
        bool_val[0] = val;
    type = "__BOOL__";
}
void var::Value::set_val(char val){
    if(ch_val.empty())
        ch_val.push_back(val);
    else
        ch_val[0] = val;
    type = "__CHAR__";
}

void var::Value::arr_setVal(int val, int pos){
    if(int_val.empty())
        int_val.push_back(val);
    else
        int_val[pos] = val;
    type = "__INT__";
}
void var::Value::arr_setVal(float val, int pos){
    if(deci_val.empty())
        deci_val.push_back(val);
    else
        deci_val[pos] = val;
    type = "__DECI__";
}
void var::Value::arr_setVal(std::string val, int pos){
    if(str_val.empty())
        str_val.push_back(val);
    else
        str_val[pos] = val;
    type = "__STRING__";
}
void var::Value::arr_setVal(bool val, int pos){
    if(bool_val.empty())
        bool_val.push_back(val);
    else
        bool_val[pos] = val;
    type = "__BOOL__";
}
void var::Value::arr_setVal(char val, int pos){
    if(ch_val.empty())
        ch_val.push_back(val);
    else
        ch_val[pos] = val;
    type = "__CHAR__";
}
void var::Value::arr_addVal(int val){
    int_val.push_back(val);
    type = "__INT__";
    is_array = true;
}
void var::Value::arr_addVal(float val){
    deci_val.push_back(val);
    type = "__DECI__";
    is_array = true;
}
void var::Value::arr_addVal(char val){
    ch_val.push_back(val);
    type = "__CHAR__";
    is_array = true;
}
void var::Value::arr_addVal(std::string val){
    str_val.push_back(val);
    type = "__STRING__";
    is_array = true;
}
void var::Value::arr_addVal(bool val){
    bool_val.push_back(val);
    type = "__BOOL__";
    is_array = true;
}
void var::Value::set_lc(int line_, int col_){
    line = line_;
    column = col_;
}

//

//scope
scope::Scope::Scope(std::string name_) : name(name_){}
void scope::Scope::new_var(std::string name, var::Value val){
    vars.push_back(std::pair<std::string, var::Value>(name, val));
    identifier_table.push_back(name);
}
std::string scope::Scope::get_name(){
    return name;
}
bool scope::Scope::find(std::string target){
    for(int i = 0; i < vars.size(); i++){
        if(vars[i].first == target) return true;
    }
    return false;
}
int scope::Scope::findI(std::string target){
    for(int i = 0; i < vars.size(); i++){
        if(vars[i].first == target) return i;
    }
    return -1;
}

scope::ScopeSet::ScopeSet(){
    this->newScope("__epp_global_scope__");
}
bool scope::ScopeSet::findInAllScope(std::string name){
    if(scope_pool[deep_count].find(name)) return true;
    else{
        int temp = deep_count;
        while(temp >= 0){
            if(scope_pool[temp].find(name)) return true;
            --temp;
        }
        return false;
    }
}
int scope::ScopeSet::findInAllScopeI(std::string name){
    if(scope_pool[deep_count].find(name)) return deep_count;
    else{
        int temp = deep_count;
        while(temp >= 0){
            if(scope_pool[temp].find(name)) return temp;
            --temp;
        }
        return -1;
    }
};
void scope::ScopeSet::next(){
    deep_count += 1;
}
void scope::ScopeSet::back(){
    if(deep_count == 0) deep_count = 0;
    else deep_count -= 1;
}
void scope::ScopeSet::remove(){
    if(scope_pool[scope_pool.size() - 1].get_name() == "__epp_global_scope__") return;
    else scope_pool.pop_back();
    back();
}
void scope::ScopeSet::newScope(std::string name){
    scope_pool.push_back(Scope(name));
}
int scope::ScopeSet::getDeep() {
    return deep_count;
}
//
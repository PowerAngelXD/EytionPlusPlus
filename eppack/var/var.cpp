#include "var.h"

//var
var::Value::Value(bool is_null){
    this->nullval = 0;
    this->type = "__NULL__";
}
var::Value::Value(bool is_arr, bool is_con) : is_array(is_arr), is_const(is_con) {}
var::Value::Value(bool is_arr, bool is_con, std::string _type) : is_array(is_arr), is_const(is_con), type(_type) {}
var::Value::Value(bool is_arr, bool is_con, int val) : is_array(is_arr), is_const(is_con) {
    type = "__INT__";
    int_val.push_back(val);
}
var::Value::Value(bool is_arr, bool is_con, float val) : is_array(is_arr), is_const(is_con) {
    type = "__DECI__";
    deci_val.push_back(val);
}
var::Value::Value(bool is_arr, bool is_con, std::string s, bool ischar) : is_array(is_arr), is_const(is_con) {
    if(ischar){
        type = "__CHAR__";
        ch_val.push_back(s);
    }
    else{
        type = "__STRING__";
        str_val.push_back(s);
    }
}
var::Value::Value(bool is_arr, bool is_con, bool val) : is_array(is_arr), is_const(is_con) {
    type = "__BOOL__";
    bool_val.push_back(val);
}
var::Value::Value(bool is_arr, bool is_con, efunc::Efunction val) : is_array(is_arr), is_const(is_con) {
    type = "__FUNC__";
    func_val.push_back(val);
}
var::Value::Value(bool is_arr, bool is_con, var::UserScope val) : is_array(is_arr), is_const(is_con) {
    type = "__SCOP__";
    us_val.push_back(val);
}
var::Value::Value(bool is_arr, bool is_con, var::Value val) : is_array(is_arr), is_const(is_con) {
    type = "__VAL__";
    val_val.push_back(val);
}

var::Value::Value(bool is_arr, bool is_con, std::vector<int> val) : is_array(is_arr), is_const(is_con), int_val(val) {
    type = "__INT__";
}
var::Value::Value(bool is_arr, bool is_con, std::vector<float> val) : is_array(is_arr), is_const(is_con), deci_val(val) {
    type = "__DECI__";
}
var::Value::Value(bool is_arr, bool is_con, std::vector<std::string> s, bool ischar) : is_array(is_arr), is_const(is_con) {
    if(ischar){
        type = "__CHAR__";
        ch_val = s;
    }
    else{
        type = "__STRING__";
        str_val = s;
    }
}
var::Value::Value(bool is_arr, bool is_con, std::vector<bool> val) : is_array(is_arr), is_const(is_con), bool_val(val) {
    type = "__BOOL__";
}
var::Value::Value(bool is_arr, bool is_con, std::vector<efunc::Efunction> val) : is_array(is_arr), is_const(is_con), func_val(val) {
    type = "__FUNC__";
}
var::Value::Value(bool is_arr, bool is_con, std::vector<UserScope> val) : is_array(is_arr), is_const(is_con), us_val(val) {
    type = "__SCOP__";
}
var::Value::Value(bool is_arr, bool is_con, std::vector<Value> val) : is_array(is_arr), is_const(is_con), val_val(val) {
    type = "__VAL__";
}

bool var::Value::isArray(){
    return is_array;
}
bool var::Value::isConst(){
    return is_const;
}
bool var::Value::isFunc(){
    return is_func;
}
std::string var::Value::getType(){
    return type;
}
int var::Value::getValueOfInt(){
    if(type == "__NULL__") return 0;
    if(type == "__INT__"){
        if(!is_array) return int_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Integer", line, column);
}
std::vector<int> var::Value::getValueOfIntArray(){
    if(type == "__NULL__") return {0};
    if(type == "__INT__"){
        if(is_array) return int_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Integer(array)", line, column);
}
float var::Value::getValueOfDecimal(){
    if(type == "__NULL__") return 0;
    if(type == "__DECI__"){
        if(!is_array) return deci_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Decimal", line, column);
}
std::vector<float> var::Value::getValueOfDecimalArray(){
    if(type == "__NULL__") return {0};
    if(type == "__DECI__"){
        if(is_array) return deci_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Decimal(array)", line, column);
}
std::string var::Value::getValueOfChar(){
    if(type == "__NULL__") return "\0";
    if(type == "__CHAR__"){
        if(!is_array) return ch_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Char", line, column);
}
std::vector<std::string> var::Value::getValueOfCharArray(){
    if(type == "__NULL__") return {"\0"};
    if(type == "__CHAR__"){
        if(is_array) return ch_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Char(array)", line, column);
}
std::string var::Value::getValueOfString(){
    if(type == "__NULL__") return "\n";
    if(type == "__STRING__"){
        if(!is_array) return str_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not String", line, column);
}
std::vector<std::string> var::Value::getValueOfStringArray(){
    if(type == "__NULL__") return {"\n"};
    if(type == "__STRING__"){
        if(is_array) return str_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not String(array)", line, column);
}
bool var::Value::getValueOfBool(){
    if(type == "__NULL__") return false;
    if(type == "__BOOL__"){
        if(!is_array) return bool_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Bool", line, column);
}
std::vector<bool> var::Value::getValueOfBoolArray(){
    if(type == "__NULL__") return {false};
    if(type == "__BOOL__"){
        if(is_array) return bool_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an non-array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Bool(array)", line, column);
}
efunc::Efunction var::Value::getValueOfFunc(){
    if(type == "__FUNC__"){
        if(!is_array) return func_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Function", line, column);
}
std::vector<efunc::Efunction> var::Value::getValueOfFuncArray(){
    if(type == "__FUNC__"){
        if(is_array) return func_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Function", line, column);
}
var::UserScope var::Value::getValueOfUScope(){
    if(type == "__SCOPE__"){
        if(!is_array) return us_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Scope", line, column);
}
std::vector<var::UserScope> var::Value::getValueOfUScopeArray(){
    if(type == "__SCOPE__"){
        if(is_array) return us_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Scope", line, column);
}
var::Value var::Value::getValueOfValue(){
    if(type == "__SCOPE__"){
        if(!is_array) return val_val[0];
        else throw epperr::Epperr("TypeError", "Cannot call the value of a array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Scope", line, column);
}
std::vector<var::Value> var::Value::getValueOfValueArray(){
    if(type == "__SCOPE__"){
        if(is_array) return val_val;
        else throw epperr::Epperr("TypeError", "Cannot call the value of a non-array variable as an array", line, column);
    }
    else throw epperr::Epperr("TypeError", "The type of the variable is not Scope", line, column);
}

void var::Value::set_val(int val){
    if(int_val.empty())
        int_val.push_back(val);
    else
        int_val[0] = val;
    type = "__INT__";
}
void var::Value::set_val(std::string val, bool is_ch){
    if(is_ch){
        if(ch_val.empty())
            ch_val.push_back(val);
        else
            ch_val[0] = val;
        type = "__CHAR__";
    }
    else{
        if(str_val.empty())
            str_val.push_back(val);
        else
            str_val[0] = val;
        type = "__STRING__";
    }
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
void var::Value::set_val(efunc::Efunction val){
    if(func_val.empty())
        func_val.push_back(val);
    else
        func_val[0] = val;
    type = "__FUNC__";
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
void var::Value::arr_setVal(std::string val, int pos, bool is_ch){
    if(is_ch){
        if(ch_val.empty())
            ch_val.push_back(val);
        else
            ch_val[pos] = val;
        type = "__CHAR__";
    }
    else{
        if(str_val.empty())
            str_val.push_back(val);
        else
            str_val[pos] = val;
        type = "__STRING__";
    }
}
void var::Value::arr_setVal(bool val, int pos){
    if(bool_val.empty())
        bool_val.push_back(val);
    else
        bool_val[pos] = val;
    type = "__BOOL__";
}
void var::Value::arr_setVal(efunc::Efunction val, int pos){
    if(func_val.empty())
        func_val.push_back(val);
    else
        func_val[pos] = val;
    type = "__FUNC__";
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
void var::Value::arr_addVal(std::string val, bool is_ch){
    if(is_ch){
        ch_val.push_back(val);
        type = "__CHAR__";
        is_array = true;
    }
    else{
        str_val.push_back(val);
        type = "__STRING__";
        is_array = true;
    }
}
void var::Value::arr_addVal(bool val){
    bool_val.push_back(val);
    type = "__BOOL__";
    is_array = true;
}
void var::Value::arr_addVal(efunc::Efunction val){
    func_val.push_back(val);
    type = "__FUNC__";
    is_array = true;
    is_func = true;
}
void var::Value::set_lc(int line_, int col_){
    line = line_;
    column = col_;
}

//

//scope
var::Scope::Scope(std::string name_) : name(name_){}
void var::Scope::new_var(std::string name, var::Value val){
    vars.push_back(varUnit(name, val));
    identifier_table.push_back(name);
}
std::string var::Scope::get_name(){
    return name;
}
bool var::Scope::find(std::string target){
    for(int i = 0; i < vars.size(); i++){
        if(vars[i].first == target) return true;
    }
    return false;
}
int var::Scope::findI(std::string target){
    for(int i = 0; i < vars.size(); i++){
        if(vars[i].first == target) return i;
    }
    return -1;
}
void var::Scope::deleteVar(std::string name){
    this->identifier_table.erase(this->identifier_table.begin() + this->findI(name));
    this->vars.erase(this->vars.begin() + this->findI(name));
}
void var::Scope::assign(std::string name, var::Value value){
    vars.erase(vars.begin() + findI(name));
    vars.push_back(std::pair<std::string ,var::Value>(name, value));
}

var::UserScope::UserScope(std::string name_, east::StatNode stat_): var::Scope(name_), stat(stat_){}

var::ScopeSet::ScopeSet(){
    this->newScope("__epp_global_scope__");
    this->scope_pool[0].new_var("epp_version", var::Value(false, true, "dev-0.2.6", false));
    this->scope_pool[0].new_var("epp_btime", var::Value(false, true, (std::string)__DATE__, false));
}
bool var::ScopeSet::findInAllScope(std::string name){
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
int var::ScopeSet::findInAllScopeI(std::string name){
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
void var::ScopeSet::next(){
    deep_count += 1;
}
void var::ScopeSet::back(){
    if(deep_count == 0) deep_count = 0;
    else deep_count -= 1;
}
void var::ScopeSet::remove(){
    if(scope_pool[scope_pool.size() - 1].get_name() == "__epp_global_scope__") return;
    else scope_pool.pop_back();
    back();
}
void var::ScopeSet::newScope(std::string name){
    scope_pool.push_back(Scope(name));
}
int var::ScopeSet::getDeep() {
    return deep_count;
}

bool var::ScopeSet::findInUScope(std::string name){
    for(auto s: user_scope_pool){
        if(s.name == name) return true;
    }
    return false;
}
int var::ScopeSet::findInUScopeI(std::string name){
    for(int i=0; i<user_scope_pool.size(); i++){
        if(user_scope_pool[i].name == name) return i;
    }
    return -1;
}

var::Scope::varUnit var::ScopeSet::getTargetVar(std::string name){
    return this->scope_pool[this->findInAllScopeI(name)].vars[this->scope_pool[this->findInAllScopeI(name)].findI(name)];
}
void var::ScopeSet::assignValue(std::string target, var::Value val){
    this->scope_pool[this->findInAllScopeI(target)].assign(target, val);
}
void var::ScopeSet::createVariable(std::string name, var::Value val){
    this->scope_pool[getDeep()].new_var(name, val);
}
void var::ScopeSet::deleteVariable(std::string name){
    this->scope_pool[this->findInAllScopeI(name)].deleteVar(name);
}
//
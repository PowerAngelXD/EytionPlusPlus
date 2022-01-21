#include "calc.h"

cenv::Calculation::Calculation(scope::ScopeSet sset_) : sset(sset_), is_array(false) {}

void cenv::Calculation::push(cenv::calc_unit cu){
    if (env.size() <= env_top) {
        for (int i = env.size() ; i <= env_top ; i ++)
            env.emplace_back("__NULL__", 0.0);
    }
    env[env_top ++] = cu;
}
cenv::calc_unit cenv::Calculation::pop(){
    return env[-- env_top];
}

bool cenv::Calculation::isArray(){
    return is_array;
}

void cenv::Calculation::run(){
    int len = 1;
    for(int i = 0; i < ins.size(); i++){
        if(ins[i].instr == "__PUSH__") {
            push(ins[i].unit);
        }
        else if(ins[i].instr == "__PUSHA__") {
            is_array = true;
            len+=1;
            len = atoi(ins[i].unit.first.c_str());
        }
        else if(ins[i].instr == "__TPOF__") {
            constpool.push_back(pop().first);
            push(cenv::calc_unit("__STRING__", constpool.size()-1));
        }
        else if(ins[i].instr == "__INPUT__"){
            if(ins[i].para == " "){
                std::cout<<" ";
                std::string in;
                std::getline(std::cin, in);
                constpool.push_back(in);
                push(cenv::calc_unit("__STRING__", constpool.size()-1));
            }
            else{
                auto temp = pop();
                if(temp.first == "__STRING__"){
                    std::cout<<constpool[(int)temp.second];
                    std::string in;
                    std::getline(std::cin, in);
                    constpool.push_back(in);
                    push(cenv::calc_unit("__STRING__", constpool.size()-1));
                }
                else throw epperr::Epperr("TypeError", "Types other than String cannot be used here", ins[i].line, ins[i].column);
            }
        }
        else if(ins[i].instr == "__POP__") {
            if(sset.findInAllScope(ins[i].para)){
                auto temp = sset.scope_pool[sset.findInAllScopeI(ins[i].para)].vars[sset.scope_pool[sset.findInAllScopeI(ins[i].para)].findI(ins[i].para)];
                if(temp.second.getType() == "__INT__") push(cenv::calc_unit("__INT__", temp.second.val_int()));
                else if(temp.second.getType() == "__DECI__") push(cenv::calc_unit("__DECI__", temp.second.val_deci()));
                else if(temp.second.getType() == "__BOOL__") push(cenv::calc_unit("__BOOL__", temp.second.val_bool()));
                else if(temp.second.getType() == "__CHAR__") {

                }
                else if(temp.second.getType() == "__STRING__") {
                    constpool.push_back(temp.second.val_string());
                    push(cenv::calc_unit("__STRING__", constpool.size()-1));
                }
            }
            else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "__ARRE_POP__") {
            is_array = true;
            if(sset.findInAllScope(ins[i].para)){
                auto index = pop();
                if(index.first != "__INT__")
                    throw epperr::Epperr("TypeError", "The index of the list must be of type int", ins[i].line, ins[i].column);
                auto temp = sset.scope_pool[sset.findInAllScopeI(ins[i].para)].vars[sset.scope_pool[sset.findInAllScopeI(ins[i].para)].findI(ins[i].para)];
                if(temp.second.getType() == "__INT__") push(cenv::calc_unit("__INT__", temp.second.val_int_array()[index.second]));
                else if(temp.second.getType() == "__DECI__") push(cenv::calc_unit("__DECI__", temp.second.val_deci_array()[index.second]));
                else if(temp.second.getType() == "__BOOL__") push(cenv::calc_unit("__BOOL__", temp.second.val_bool_array()[index.second]));
                else if(temp.second.getType() == "__CHAR__") {

                }
                else if(temp.second.getType() == "__STRING__") {
                    constpool.push_back(temp.second.val_string_array()[index.second]);
                    push(cenv::calc_unit("__STRING__", constpool.size()-1));
                }
            }
            else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "+") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__DECI__", left.second + right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__INT__", left.second + right.second));
            else if(left.first == "__STRING__" && right.first == "__STRING__") {
                constpool.push_back(constpool[left.second] + constpool[right.second]);
                push(cenv::calc_unit("__STRING__", constpool.size() - 1));
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '+'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "-") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__DECI__", left.second - right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__INT__", left.second - right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '-'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "*") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__DECI__", right.second * left.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__INT__", right.second * left.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '*'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "/") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__DECI__", left.second /right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__INT__", left.second /right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '/'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "%") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__INT__", (int)left.second % (int)right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '%'", ins[i].line, ins[i].column);
        }

        else if(ins[i].instr == "==") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second == right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second == right.second));
            else if(left.first == "__STRING__" && right.first == "__STRING__") {
                push(cenv::calc_unit("__BOOL__", constpool[left.second] == constpool[right.second]));
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '=='", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == ">=") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second >= right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second >= right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '>='", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "<=") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second <= right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second <= right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '<='", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == ">") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second > right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second > right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '>'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "<") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second < right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second < right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '<'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "!=") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second != right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second != right.second));
            else if(left.first == "__STRING__" && right.first == "__STRING__") {
                push(cenv::calc_unit("__BOOL__", constpool[left.second] != constpool[right.second]));
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '!='", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "&&") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second && right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second && right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '&&'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "||") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second || right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second || right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '||'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "!") {
            auto val = pop();
            if(val.first == "__BOOL__") push(cenv::calc_unit("__DECI__", !(bool)val.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '!'", ins[i].line, ins[i].column);
        }
    }
    if(is_array || len > 1){
        std::string firstType;
        for(int i = 0; i < env.size(); i++){
            if(i==0) firstType = env[i].first;
            if(env[i].first != firstType) throw epperr::Epperr("TypeError", "Cannot use inconsistent types in lists!", ins[i].line, ins[i].column);
            result.push_back(env[i]);
        }
    }
    else result.push_back(pop());
}

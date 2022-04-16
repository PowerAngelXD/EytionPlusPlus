#include <sstream>
#include "calc.h"

cenv::Calculation::Calculation(var::ScopeSet sset_) : sset(sset_), is_array(false) {}

void cenv::Calculation::push(cenv::calc_unit cu){
    if (env.size() <= env_top) {
        for (int i = env.size() ; i <= env_top ; i ++)
            env.emplace_back("__NULL__", 0.0);
    }
    env[env_top ++] = cu;
}
cenv::calc_unit cenv::Calculation::pop(){
    auto ret = env[-- env_top];
    return ret;
}

bool cenv::Calculation::isArray(){
    return is_array;
}

void cenv::Calculation::run(){
    for(int i = 0; i < ins.size(); i++){
        if(ins[i].instr == "__PUSH__") {
            push(ins[i].unit);
        }
        else if(ins[i].instr == "__PUSHA__") {
            is_array = true;
            for(len = 0; len<env.size(); len++);
        }
        else if(ins[i].instr == "__PUSHI__") {
            push(ins[i].unit);
        }
        else if(ins[i].instr == "__SIADADD__") {
            auto original = pop();
            if(original.first == "__INT__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.assignValue(ins[i].para, var::Value(false, false, (int)(original.second + 1)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else if(original.first == "__DECI__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.assignValue(ins[i].para, var::Value(false, false, (float)(original.second + 1)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '++'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "__SIADSUB__") {
            auto original = pop();
            if(original.first == "__INT__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.assignValue(ins[i].para, var::Value(false, false, (int)(original.second - 1)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else if(original.first == "__DECI__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.assignValue(ins[i].para, var::Value(false, false, (float)(original.second - 1)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '++'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "__SIADSUB_FRONT__") {
            auto original = pop();
            original.second -= 1;
            if(original.first == "__INT__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.scope_pool[sset.findInAllScopeI(ins[i].para)].assign(ins[i].para, var::Value(false, false, "__INT__", (int)(original.second)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else if(original.first == "__DECI__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.scope_pool[sset.findInAllScopeI(ins[i].para)].assign(ins[i].para, var::Value(false, false, "__DECI__", (float)(original.second)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '++'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "__SIADADD_FRONT__") {
            auto original = pop();
            original.second += 1;
            if(original.first == "__INT__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.scope_pool[sset.findInAllScopeI(ins[i].para)].assign(ins[i].para, var::Value(false, false, "__INT__", (int)(original.second)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else if(original.first == "__DECI__"){
                push(original);
                if(sset.scope_pool[sset.findInAllScopeI(ins[i].para)].find(ins[i].para)){
                    this->sset.scope_pool[sset.findInAllScopeI(ins[i].para)].assign(ins[i].para, var::Value(false, false, "__DECI__", (float)(original.second)));
                }
                else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '++'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "__TYT__") {
            auto original = pop();
            auto target_type = ins[i].para;
            if(!is_array){
                if(target_type == original.first) push(cenv::calc_unit(original.first, original.second));
                else if(target_type == "__INT__"){
                    if(original.first == "__STRING__")
                        push(cenv::calc_unit(target_type, atoi(constpool[original.second].c_str())));
                    else if(original.first == "__DECI__")
                        push(cenv::calc_unit(target_type, (int)original.second));
                    else if(original.first == "__BOOL__")
                        push(cenv::calc_unit(target_type, (int)original.second));
                }
                else if(target_type == "__DECI__"){
                    if(original.first == "__STRING__")
                        push(cenv::calc_unit(target_type, atof(constpool[original.second].c_str())));
                    else if(original.first == "__INT__")
                        push(cenv::calc_unit(target_type, (float)original.second));
                    else if(original.first == "__BOOL__")
                        push(cenv::calc_unit(target_type, (float)original.second));
                }
                else if(target_type == "__BOOL__"){
                    if(original.first == "__STRING__"){
                        if(constpool[original.second] == "false")
                            push(cenv::calc_unit(target_type, 0));
                        else push(cenv::calc_unit(target_type, 1));
                    }
                    else if(original.first == "__DECI__")
                        push(cenv::calc_unit(target_type, (bool)original.second));
                    else if(original.first == "__INT__")
                        push(cenv::calc_unit(target_type, (bool)original.second));
                }
                else if(target_type == "__STRING__"){
                    if(original.first == "__INT__"){
                        std::ostringstream oss;
                        oss<<original.second;
                        constpool.push_back(oss.str());
                        push(cenv::calc_unit(target_type, constpool.size()-1));
                    }
                    else if(original.first == "__DECI__"){
                        std::ostringstream oss;
                        oss<<original.second;
                        constpool.push_back(oss.str());
                        push(cenv::calc_unit(target_type, constpool.size()-1));
                    }
                    else if(original.first == "__BOOL__"){
                        if(original.second == 0.0)
                            constpool.push_back("false");
                        else constpool.push_back("true");
                        push(cenv::calc_unit(target_type, constpool.size()-1));
                    }
                }
            }
        }
        else if(ins[i].instr == "__LEN__") {
            auto temp = pop();
            if(is_array){
                is_array = false;
                for(int i = 0; i < len - 1;i++) pop();
                push(cenv::calc_unit("__INT__", len));
            }
            else if(temp.first == "__STRING__"){
                push(cenv::calc_unit("__INT__", constpool[temp.second].size()));
            }
            else throw epperr::Epperr("SyntaxError", "Only lists can use len expressions!", ins[i].line, ins[i].column);
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
        else if(ins[i].instr == "__PRINT__") {
            if(ins[i].para == "NOLINE"){
                auto content = pop();
                if(is_array){
                    is_array = false;
                    for(int i = 0; i < len;i++) {
                        if (env[i].first == "__INT__" || env[i].first == "__DECI__")
                            std::cout << env[i].second;
                        else if (env[i].first == "__BOOL__"){
                            if (env[i].second == 0)
                                std::cout << "true";
                            else
                                std::cout << "false";
                        }
                        else if (env[i].first == "__STRING__" || env[i].first == "__CHAR__")
                            std::cout << constpool[(int)env[i].second];
                        push(content);
                    }
                }
                else{
                    if(content.first == "__INT__" || content.first == "__DECI__"){
                        std::cout<<content.second;
                    }
                    else if(content.first == "__BOOL__"){
                        if(content.second == 0)
                            std::cout<<"true";
                        else
                            std::cout<<"false";
                    }
                    else if(content.first == "__STRING__" || content.first == "__CHAR__"){
                        std::cout<<constpool[(int)content.second];
                    }
                    push(content);
                }
            }
            else{
                auto content = pop();
                if(is_array){
                    is_array = false;
                    for(int i = 0; i < len - 1;i++) {
                        if (env[i].first == "__INT__" || env[i].first == "__DECI__")
                            std::cout << env[i].second << std::endl;
                        else if (env[i].first == "__BOOL__"){
                            if (env[i].second == 0)
                                std::cout << "true" << std::endl;
                            else
                                std::cout << "false" << std::endl;
                        }
                        else if (env[i].first == "__STRING__" || env[i].first == "__CHAR__")
                            std::cout << constpool[(int)env[i].second] << std::endl;
                        pop();
                    }
                }
                else{
                    if(content.first == "__INT__" || content.first == "__DECI__")
                        std::cout<<content.second<<std::endl;
                    else if(content.first == "__BOOL__"){
                        if(content.second == 0)
                            std::cout<<"true"<<std::endl;
                        else
                            std::cout<<"false"<<std::endl;
                    }
                    else if(content.first == "__STRING__" || content.first == "__CHAR__")
                        std::cout<<constpool[(int)content.second]<<std::endl;
                    push(content);
                }
            }
        }
        else if(ins[i].instr == "__POP__") {
            if(sset.findInAllScope(ins[i].para)){
                auto temp = sset.getTargetVar(ins[i].para);
                if(temp.second.isArray()){
                    is_array = true;
                    if(temp.second.getType() == "__INT__")
                        for(len = 0; len<temp.second.getValueOfIntArray().size(); len++)
                            push(cenv::calc_unit("__INT__", temp.second.getValueOfIntArray()[len]));
                    else if(temp.second.getType() == "__DECI__")
                        for(len = 0; len<temp.second.getValueOfDecimalArray().size(); len++)
                            push(cenv::calc_unit("__DECI__", temp.second.getValueOfDecimalArray()[len]));
                    else if(temp.second.getType() == "__BOOL__")
                        for(len = 0; len<temp.second.getValueOfBoolArray().size(); len++)
                            push(cenv::calc_unit("__BOOL__", temp.second.getValueOfBoolArray()[len]));
                    else if(temp.second.getType() == "__CHAR__")
                        for(len = 0; len<temp.second.getValueOfCharArray().size(); len++) {
                            constpool.push_back(temp.second.getValueOfCharArray()[len]);
                            push(cenv::calc_unit("__CHAR__", constpool.size()-1));
                        }
                    else if(temp.second.getType() == "__STRING__")
                        for(len = 0; len<temp.second.getValueOfStringArray().size(); len++) {
                            constpool.push_back(temp.second.getValueOfStringArray()[len]);
                            push(cenv::calc_unit("__STRING__", constpool.size()-1));
                        }
                }
                else{
                    if(temp.second.getType() == "__INT__") push(cenv::calc_unit("__INT__", temp.second.getValueOfInt()));
                    else if(temp.second.getType() == "__DECI__") push(cenv::calc_unit("__DECI__", temp.second.getValueOfDecimal()));
                    else if(temp.second.getType() == "__BOOL__") push(cenv::calc_unit("__BOOL__", temp.second.getValueOfBool()));
                    else if(temp.second.getType() == "__CHAR__") {
                        constpool.push_back(temp.second.getValueOfChar());
                        push(cenv::calc_unit("__CHAR__", constpool.size()-1));
                    }
                    else if(temp.second.getType() == "__STRING__") {
                        constpool.push_back(temp.second.getValueOfString());
                        push(cenv::calc_unit("__STRING__", constpool.size()-1));
                    }
                }
            }
            else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + ins[i].para + "'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "__ARRE_POP__") {
            //is_array = true;
            if(sset.findInAllScope(ins[i].para)){
                auto index = pop();
                if(index.first != "__INT__")
                    throw epperr::Epperr("TypeError", "The index of the list must be of type int", ins[i].line, ins[i].column);
                auto temp = sset.scope_pool[sset.findInAllScopeI(ins[i].para)].vars[sset.scope_pool[sset.findInAllScopeI(ins[i].para)].findI(ins[i].para)];
                if(temp.second.getType() == "__INT__") {
                    if(index.second > temp.second.len-1) throw epperr::Epperr("ArrayError", "The referenced content is outside the bounds of the array", ins[i].line, ins[i].column);
                    push(cenv::calc_unit("__INT__", temp.second.getValueOfIntArray()[index.second]));
                }
                else if(temp.second.getType() == "__DECI__") {
                    if(index.second > temp.second.len-1) throw epperr::Epperr("ArrayError", "The referenced content is outside the bounds of the array", ins[i].line, ins[i].column);
                    push(cenv::calc_unit("__DECI__", temp.second.getValueOfDecimalArray()[index.second]));
                }
                else if(temp.second.getType() == "__BOOL__") {
                    if(index.second > temp.second.len-1) throw epperr::Epperr("ArrayError", "The referenced content is outside the bounds of the array", ins[i].line, ins[i].column);
                    push(cenv::calc_unit("__BOOL__", temp.second.getValueOfBoolArray()[index.second]));
                }
                else if(temp.second.getType() == "__CHAR__") {
                    if(index.second > temp.second.len-1) throw epperr::Epperr("ArrayError", "The referenced content is outside the bounds of the array", ins[i].line, ins[i].column);
                    constpool.push_back(temp.second.getValueOfCharArray()[index.second]);
                    push(cenv::calc_unit("__CHAR__", constpool.size()-1));
                }
                else if(temp.second.getType() == "__STRING__") {
                    if(index.second > temp.second.len-1) throw epperr::Epperr("ArrayError", "The referenced content is outside the bounds of the array", ins[i].line, ins[i].column);
                    constpool.push_back(temp.second.getValueOfStringArray()[index.second]);
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
            if(left.first != right.first) push(cenv::calc_unit("__BOOL__", 0));
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
            if(left.first == right.first) push(cenv::calc_unit("__BOOL__", 0));
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
            else if(left.first == "__BOOL__" || right.first == "__BOOL__") push(cenv::calc_unit("__BOOL__", left.second && right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '&&'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "||") {
            auto right = pop();
            auto left = pop();
            if(left.first == "__DECI__" || right.first == "__DECI__") push(cenv::calc_unit("__BOOL__", left.second || right.second));
            else if(left.first == "__INT__" || right.first == "__INT__") push(cenv::calc_unit("__BOOL__", left.second || right.second));
            else if(left.first == "__BOOL__" || right.first == "__BOOL__") push(cenv::calc_unit("__BOOL__", left.second || right.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '||'", ins[i].line, ins[i].column);
        }
        else if(ins[i].instr == "!") {
            auto val = pop();
            if(val.first == "__BOOL__") push(cenv::calc_unit("__DECI__", !(bool)val.second));
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '!'", ins[i].line, ins[i].column);
        }
    }
    if(is_array){
        std::string firstType;
        for(int i = 0; i < env.size(); i++){
            if(i==0) firstType = env[i].first;
            if(env[i].first != firstType) throw epperr::Epperr("TypeError", "Cannot use inconsistent types in lists!", ins[i].line, ins[i].column); // check type
            result.push_back(env[i]);
        }
    }
    else result.push_back(pop());
}

#include "calc.h"

void cenv::Calculation::push(cenv::calc_unit cu){
    if (env.size() <= env_top) {
        for (int i = env.size() ; i <= env_top ; i ++)
            env.push_back({"__NULL__", calc_unit("__NULL__", 0)});
    }
    env[env_top ++] = cu;
}
cenv::calc_unit cenv::Calculation::pop(){
    return env[-- env_top];
}

void cenv::Calculation::run(){
    for(int i = 0; i < ins.size(); i++){
        if(ins[i].sym == "__PUSH__") {
            push(ins[i].unit);
        }
        else if(ins[i].sym == "+") {
            auto a = pop();
            auto b = pop();
            if(a.first == "__INT__") {
                if(b.first == "__INT__") {
                    auto _a = std::any_cast<int>(a.second);
                    auto _b = std::any_cast<int>(b.second);
                    push(calc_unit("__INT__", _a+_b));
                }
                else if(b.first == "__DECI__") {
                    auto _a = std::any_cast<int>(a.second);
                    auto _b = std::any_cast<double>(b.second);
                    push(calc_unit("__DECI__", _a+_b));
                }
            }
            else if(a.first == "__DECI__") {
                if(b.first == "__INT__") {
                    auto _a = std::any_cast<double>(a.second);
                    auto _b = std::any_cast<int>(b.second);
                    push(calc_unit("__DECI__", _a+_b));
                }
                else if(b.first == "__DECI__") {
                    auto _a = std::any_cast<double>(a.second);
                    auto _b = std::any_cast<double>(b.second);
                    push(calc_unit("__DECI__", _a+_b));
                }
            }
            else throw epperr::Epperr("TypeError", "Type uses unsupported symbol '+'", ins[i].line, ins[i].column);
        }
    }
    result = pop();
}

#include "parser.h"

inline cenv::Calculation _calc(east::ExprNode node) {
    cvisitor::visitor v;
    v.visitAddExpr(node.addexpr);
    cenv::Calculation calc;
    calc.ins = v.ins; calc.constpool = v.constpool;
    calc.run();
    return calc;
}

parser::Parser::Parser(east::StatNode stat_) : stat(stat_) {}
east::StatNode parser::Parser::get(){ return stat;}
void parser::Parser::parse(){
    for(int i = 0; i < stat.stmts.size(); i++){
        if(stat.stmts[i]->outstmt != nullptr){
            cenv::Calculation calc = _calc(*stat.stmts.at(i)->outstmt->content);
            if(calc.result.first == "__STRING__")
                std::cout<<calc.constpool[calc.result.second];
            else
                std::cout<<calc.result.second;
        }
    }
}
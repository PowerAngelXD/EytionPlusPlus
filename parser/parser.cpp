#include "parser.h"

inline cenv::Calculation _calc(east::ExprNode node, scope::ScopeSet sset) {
    cenv::Calculation calc(sset);
    if(node.addexpr != nullptr){
        cvisitor::visitor v;
        v.visitAddExpr(node.addexpr);
        calc.ins = v.ins; calc.constpool = v.constpool;
        calc.run();
    }
    else if(node.listexpr != nullptr){
        cvisitor::visitor v;
        v.visitListExpr(node.listexpr);
        calc.ins = v.ins; calc.constpool = v.constpool;
        calc.run();
    }
    return calc;
}

void parser::Parser::parse(){
    for(int index = 0; index < stat.stmts.size(); index++){
        if(stat.stmts[index]->outstmt != nullptr){
            cenv::Calculation calc = _calc(*stat.stmts[index]->outstmt->content, sset);
            if(calc.isArray()){
                if(calc.result[0].first == "__STRING__") {
                    for(int i = 0; i < calc.result.size(); i++) {
                        std::cout << calc.constpool[calc.result[i].second];
                        std::cout << " " ;
                    }
                }
                else {
                    for(int i = 0; i < calc.result.size(); i++) {
                        std::cout << calc.result[i].second;
                        std::cout << " " ;
                    }
                }
            }
            else{
                if(calc.result[0].first == "__STRING__")
                    std::cout<<calc.constpool[calc.result[0].second];
                else if(calc.result[0].first == "__CHAR__")
                    std::cout<<calc.constpool[calc.result[0].second];
                else
                    std::cout<<calc.result[0].second;
            }
        }
        else if(stat.stmts[index]->vorcstmt != nullptr){
            auto name = stat.stmts[index]->vorcstmt->iden->content;
            auto expr = stat.stmts[index]->vorcstmt->value;
            cenv::Calculation calc = _calc(*expr, sset);
            var::Value val;
            val.set_lc(stat.stmts[index]->vorcstmt->iden->line, stat.stmts[index]->vorcstmt->iden->column);
            if(sset.findInAllScope(name)) throw epperr::Epperr("NameError", "duplicate identifier '" + name + "'!", val.line, val.column);
            if(calc.isArray()){

            }
            else{
                if(calc.result[0].first == "__INT__") val.set_val((int)calc.result[0].second);
                else if(calc.result[0].first == "__DECI__") val.set_val((float)calc.result[0].second);
                else if(calc.result[0].first == "__BOOL__") val.set_val((bool)calc.result[0].second);
                else if(calc.result[0].first == "__STRING__") val.set_val(calc.constpool[(int)calc.result[0].second]);
                else if(calc.result[0].first == "__CHAR__") val.set_val((char)calc.result[0].second);
                else{std::cout<<"none"<<std::endl;}
            }
            sset.scope_pool[sset.getDeep()].identifier_table.emplace_back(name);
            sset.scope_pool[sset.getDeep()].vars.emplace_back(name, val);
        }
        else if(stat.stmts[index]->assignstmt != nullptr){
            auto name = stat.stmts[index]->assignstmt->iden->idens[0]->content;
            auto expr = stat.stmts[index]->assignstmt->val;
            cenv::Calculation calc = _calc(*expr, sset);
            if(sset.findInAllScope(name)){
                if(calc.isArray()){

                }
                else{
                    auto temp = sset.scope_pool[sset.getDeep()].vars[sset.findInAllScopeI(name)];
                    auto type = calc.result[0].first;
                    sset.scope_pool[sset.getDeep()].vars[sset.findInAllScopeI(name)].second.set_val((int)calc.result[0].second);
                }
            }
            else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + name + "'",
                                      stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                      stat.stmts[index]->assignstmt->iden->idens[0]->column);
        }

    }
}
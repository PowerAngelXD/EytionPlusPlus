#include "parser.h"

inline cenv::Calculation _calc(east::ExprNode node, scope::ScopeSet sset) {
    cenv::Calculation calc(sset);
    if(node.addexpr != nullptr){
        cvisitor::visitor v;
        v.visitAddExpr(node.addexpr);
        calc.ins = v.ins; calc.constpool = v.constpool;
        calc.run();
    }
    else if(node.boolexpr != nullptr){
        cvisitor::visitor v;
        v.visitBoolExpr(node.boolexpr);
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
            cenv::Calculation calc = _calc(*stat.stmts[index]->outstmt->content, this->sset);
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

            if(calc.isArray()){
                var::Value val(true, false);
                if(sset.findInAllScope(name)) throw epperr::Epperr("NameError", "duplicate identifier '" + name + "'!", val.line, val.column);
                val.set_lc(stat.stmts[index]->vorcstmt->iden->line, stat.stmts[index]->vorcstmt->iden->column);
                if(stat.stmts[index]->vorcstmt->mark->content == "const") {
                    var::Value _val(true, true);
                    val = _val;
                }
                else {
                    var::Value _val(true, false);
                    val = _val;
                }
                if(calc.result[0].first == "__INT__")
                    for(int i=0; i<calc.result.size(); i++) val.arr_addVal((int)calc.result[i].second);
                if(calc.result[0].first == "__STRING__")
                    for(int i=0; i<calc.result.size(); i++) val.arr_addVal(calc.constpool[(int)calc.result[i].second]);
                if(calc.result[0].first == "__DECI__")
                    for(int i=0; i<calc.result.size(); i++) val.arr_addVal((float)calc.result[i].second);
                if(calc.result[0].first == "__BOOL__")
                    for(int i=0; i<calc.result.size(); i++) val.arr_addVal((bool)calc.result[i].second);
                if(calc.result[0].first == "__CHAR__")
                    for(int i=0; i<calc.result.size(); i++) val.arr_addVal(calc.constpool[(int)calc.result[i].second]);
                sset.scope_pool[sset.getDeep()].identifier_table.emplace_back(name);
                sset.scope_pool[sset.getDeep()].vars.emplace_back(name, val);
            }

            else{
                var::Value val(false, false);
                if(sset.findInAllScope(name)) throw epperr::Epperr("NameError", "duplicate identifier '" + name + "'!", val.line, val.column);
                val.set_lc(stat.stmts[index]->vorcstmt->iden->line, stat.stmts[index]->vorcstmt->iden->column);
                if(stat.stmts[index]->vorcstmt->mark->content == "const") {
                    var::Value _val(false, true);
                    val = _val;
                }
                else {
                    var::Value _val(false, false);
                    val = _val;
                }
                if(calc.result[0].first == "__INT__") val.set_val((int)calc.result[0].second);
                else if(calc.result[0].first == "__DECI__") val.set_val((float)calc.result[0].second);
                else if(calc.result[0].first == "__BOOL__") val.set_val((bool)calc.result[0].second);
                else if(calc.result[0].first == "__STRING__") val.set_val(calc.constpool[(int)calc.result[0].second]);
                else if(calc.result[0].first == "__CHAR__") val.set_val(calc.constpool[(int)calc.result[0].second]);
                else{std::cout<<"none"<<std::endl;}
                sset.scope_pool[sset.getDeep()].identifier_table.emplace_back(name);
                sset.scope_pool[sset.getDeep()].vars.emplace_back(name, val);
            }
        }
        else if(stat.stmts[index]->assignstmt != nullptr){
            auto name = stat.stmts[index]->assignstmt->iden->idens[0]->content;
            auto expr = stat.stmts[index]->assignstmt->val;
            cenv::Calculation calc = _calc(*expr, sset);
            if(sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.getDeep()].findI(name)].second.isConst())
                throw epperr::Epperr("AssignError", "Cannot assign a value to a constant", stat.stmts[index]->assignstmt->iden->idens[0]->line, stat.stmts[index]->assignstmt->iden->idens[0]->column);
            if(sset.findInAllScope(name)){
                if(stat.stmts[index]->assignstmt->iden->getIdenType() == "__ARRE__"){
                    auto temp = sset.scope_pool[sset.getDeep()].vars[sset.findInAllScopeI(name)];
                    auto type = calc.result[0].first;
                    east::ExprNode _temp;
                    _temp.addexpr = stat.stmts[index]->assignstmt->iden->arrindex;
                    auto arri = _calc(_temp, sset);
                    if(temp.second.getType() != type) throw epperr::Epperr("TypeError", "A value of a different type cannot be assigned to this variable",
                                                                           stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                                                           stat.stmts[index]->assignstmt->iden->idens[0]->column);
                    else{
                        if(type == "__INT__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.arr_setVal((int)calc.result[0].second, (int)arri.result[0].second);
                        else if(type == "__DECI__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.arr_setVal(calc.result[0].second, (int)arri.result[0].second);
                        else if(type == "__STRING__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.arr_setVal(calc.constpool[(int)calc.result[0].second], (int)arri.result[0].second);
                        else if(type == "__CHAR__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.arr_setVal(calc.constpool[(int)calc.result[0].second], (int)arri.result[0].second);
                        else if(type == "__BOOL__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.arr_setVal((bool)calc.result[0].second, (int)arri.result[0].second);
                    }
                }
                else{
                    auto temp = sset.scope_pool[sset.getDeep()].vars[sset.findInAllScopeI(name)];
                    auto type = calc.result[0].first;
                    if(temp.second.getType() != type) throw epperr::Epperr("TypeError", "A value of a different type cannot be assigned to this variable",
                                                                                stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                                                                stat.stmts[index]->assignstmt->iden->idens[0]->column);
                    else{
                        if(type == "__INT__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.set_val((int)calc.result[0].second);
                        else if(type == "__DECI__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.set_val(calc.result[0].second);
                        else if(type == "__STRING__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.set_val(calc.constpool[(int)calc.result[0].second]);
                        else if(type == "__CHAR__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.set_val(calc.constpool[(int)calc.result[0].second]);
                        else if(type == "__BOOL__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.findInAllScopeI(name)].second.set_val((bool)calc.result[0].second);
                    }
                }
            }
            else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + name + "'",
                                      stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                      stat.stmts[index]->assignstmt->iden->idens[0]->column);
        }
        else if(stat.stmts[index]->deletestmt != nullptr){
            auto name = stat.stmts[index]->deletestmt->iden->idens[0]->content;
            if(!sset.findInAllScope(name)) throw epperr::Epperr("NameError", "Could not find a designator named '" + name + "'",
                                                                stat.stmts[index]->deletestmt->iden->idens[0]->line,
                                                                stat.stmts[index]->deletestmt->iden->idens[0]->column);
            sset.scope_pool[sset.getDeep()].vars.erase(sset.scope_pool[sset.getDeep()].vars.begin() + sset.findInAllScopeI(name));
        }
        else if(stat.stmts[index]->blockstmt != nullptr){
            Parser subp;
            subp.stat = *stat.stmts[index]->blockstmt->body;
            this->sset.next();
            this->sset.newScope("__epp_temp_scope__");
            subp.sset = this->sset;
            subp.parse();
            this->sset.remove();
        }
    }
}
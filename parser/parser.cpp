#include "parser.h"

inline cenv::Calculation _calc(east::ValExprNode node, var::ScopeSet sset) {
    cenv::Calculation calc(sset);
    cvisitor::visitor v;
    if(node.addexpr != nullptr){
        if(node.addexpr->muls[0]->prims[0]->bif->bifi != nullptr){
            parser::BifParser bif(node.addexpr->muls[0]->prims[0]->bif->bifi, sset);
            if(bif.bif->mark->content == "system"){
                bif.bif_Sytem();
            }
            sset = bif._sset;
            goto end;
        }
        else
            v.visitAddExpr(node.addexpr);
    }
    else if(node.boolexpr != nullptr)
        v.visitBoolExpr(node.boolexpr);
    else if(node.listexpr != nullptr)
        v.visitListExpr(node.listexpr);
    calc.ins = v.ins; calc.constpool = v.constpool;
    calc.run();
    sset = calc.sset;
    end:
    return calc;
}

inline cenv::Calculation _calc(east::BoolExprNode node, var::ScopeSet sset) {
    cenv::Calculation calc(sset);
    cvisitor::visitor v;
    v.visitBoolExpr(&node);
    calc.ins = v.ins; calc.constpool = v.constpool;
    calc.run();
    sset = calc.sset;
    return calc;
}


//-------------------//

parser::BifParser::BifParser(east::BifInstanceNode* bif, var::ScopeSet sset): bif(bif), _sset(sset) {}

void parser::BifParser::bif_Sytem(){
    if(bif->paras.empty() == true) throw epperr::Epperr("SyntaxError", "Too few parameters", bif->mark->line, bif->mark->column);
    else{
        if(bif->paras.size() > 1) throw epperr::Epperr("SyntaxError", "Too many parameters", bif->mark->line, bif->mark->column);
        else{
            if(bif->paras[0]->addexpr!=nullptr, bif->paras[0]->addexpr->muls[0]->prims[0]->str != nullptr){
                cenv::Calculation calc(_sset);
                cvisitor::visitor v;
                v.visitAddExpr(bif->paras[0]->addexpr);
                calc.ins = v.ins; calc.constpool = v.constpool;
                calc.run();
                _sset = calc.sset;
                system(calc.constpool[(int)calc.result[0].second].c_str());
            }
            else throw epperr::Epperr("SyntaxError", "There are no overloaded functions that meet the requirements: 'system(cmd: string)'", bif->mark->line, bif->mark->column);
        }
    }
}

void parser::Parser::parse_OutStmt(east::OutStmtNode* stmt){
    cenv::Calculation calc = _calc(*stmt->content, this->sset);
    if (calc.isArray()){
        if (calc.result[0].first == "__STRING__"){
            for (int i = 0; i < calc.result.size(); i++){
                std::cout << calc.constpool[calc.result[i].second];
            }
        }
        else if (calc.result[0].first == "__CHAR__"){
            for (int i = 0; i < calc.result.size(); i++){
                std::cout << calc.constpool[calc.result[i].second];
            }
        }
        else{
            for (int i = 0; i < calc.result.size(); i++){
                std::cout << calc.result[i].second;
            }
        }
    }
    else
    {
        if (calc.result[0].first == "__STRING__")
            std::cout << calc.constpool[calc.result[0].second];
        else if (calc.result[0].first == "__CHAR__")
            std::cout << calc.constpool[calc.result[0].second];
        else
            std::cout << calc.result[0].second;
    }
}

void parser::Parser::parse_ExprStmt(east::ExprStmtNode* stmt){
    cenv::Calculation calc = _calc(*stmt->expr, sset);
    this->sset = calc.sset;
}

void parser::Parser::parse_VorcStmt(east::VorcStmtNode* stmt){
    auto name = stmt->iden->content;
    auto calc = _calc(*stmt->value, sset);
    //type checker
    auto type = stmt->type;
    if(type == nullptr);
    else{
        if(type->content == "int" && calc.result[0].first == "__INT__");
        else if(type->content == "string" && calc.result[0].first == "__STRING__");
        else if(type->content == "deci" && calc.result[0].first == "__DECI__");
        else if(type->content == "bool" && calc.result[0].first == "__BOOL__");
        else if(type->content == "char" && calc.result[0].first == "__CHAR__");
        else throw epperr::Epperr("TypeError", "The pre type at the time of declaration does not match the actual incoming type", type->line, type->column);
    }
    // Check for duplicate variables / constants
    if(sset.findInAllScope(name)) throw epperr::Epperr("NameError", "duplicate identifier '" + name + "'!", stmt->iden->line, stmt->iden->column);
    if(stmt->mark->content == "var" || stmt->mark->content == "const"){
    // create variable
        if(calc.isArray()){
            if(calc.result[0].first == "__INT__"){
                std::vector<int> list;
                for(int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.result[i].second);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const"?true:false, list));
            }
            else if(calc.result[0].first == "__DECI__"){
                std::vector<float> list;
                for(int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.result[i].second);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const"?true:false, list));
            }
            else if(calc.result[0].first == "__STRING__"){
                std::vector<std::string> list;
                for(int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.constpool[calc.result[i].second]);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const"?true:false, list, false));
            }
            else if(calc.result[0].first == "__CHAR__"){
                std::vector<std::string> list;
                for(int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.constpool[calc.result[i].second]);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const"?true:false, list, true));
            }
            else if(calc.result[0].first == "__BOOL__"){
                std::vector<bool> list;
                for(int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.result[i].second);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const"?true:false, list));
            }
        }
        else{
            if(calc.result[0].first == "__STRING__")
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const"?true:false, 
                calc.constpool[calc.result[0].second], false));
            else if(calc.result[0].first == "__CHAR__")
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const"?true:false, 
                calc.constpool[calc.result[0].second], false));
            else if(calc.result[0].first == "__INT__")
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const"?true:false, 
                (int)calc.result[0].second));
            else
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const"?true:false, 
                calc.result[0].second));
        }
    }
    else{

    }
}

void parser::Parser::parse_ForStmt(east::ForStmtNode* stmt){
    auto name = stmt->iden->idens[0]->content;
    if(stmt->var_mark != nullptr)
        sset.createVariable(name, var::Value(false, false, _calc(*stmt->val, sset).result[0].second));
    auto calcBool = _calc(*stmt->cond, sset);
    while(calcBool.result[0].second > 0){
        parser::Parser stc_p;
        sset.remove();
        if(stmt->stc != nullptr){
            east::StatNode _stat;
            _stat.stmts.push_back(stmt->stc);
            stc_p.stat = _stat;
            stc_p.sset = sset;
            stc_p.parse();
        }
        else{
            if(stmt->body->body == nullptr) continue;
            stc_p.stat = *stmt->body->body;
            this->sset.next();
            this->sset.newScope("__epp_ForTemp_scope__");
            stc_p.sset = this->sset;
            stc_p.parse();
            this->sset.remove();
        }
        //后置语句
        east::StatNode stat;
        stat.stmts.push_back(stmt->dostc);
        if(stmt->dostc->exprstmt!=nullptr && stmt->dostc->exprstmt->expr->addexpr->muls[0]->prims[0]->siad!=nullptr){
            stc_p.stat = stat;
            stc_p.sset = sset;
            stc_p.parse();
        }
        else if(stmt->dostc->assignstmt!=nullptr){
            stc_p.stat = stat;
            stc_p.sset = sset;
            stc_p.parse();
        }
        else throw epperr::Epperr("SyntaxError", "Not the correct statement", stmt->separate_sym2->line, stmt->separate_sym2->column);
        //

        sset = stc_p.sset;
        calcBool = _calc(*stmt->cond, sset);
        if(calcBool.result[0].second == 0) break;
    }
    //sset.deleteVariable(name); // 删除for中的变量
}

void parser::Parser::parse_DeleteStmt(east::DeleteStmtNode* stmt){
    auto name = stmt->iden->idens[0]->content;
    if(!sset.findInAllScope(name)) throw epperr::Epperr("NameError", "Could not find a designator named '" + name + "'",
                                                        stmt->iden->idens[0]->line,
                                                        stmt->iden->idens[0]->column);
    sset.deleteVariable(name);
}

void parser::Parser::parse(){
    for(int index = 0; index < stat.stmts.size(); index++){
        if(stat.stmts[index]->outstmt != nullptr){
            parse_OutStmt(stat.stmts[index]->outstmt);
        }
        else if(stat.stmts[index]->vorcstmt != nullptr){
            parse_VorcStmt(stat.stmts[index]->vorcstmt);
        }
        else if(stat.stmts[index]->assignstmt != nullptr){
            auto name = stat.stmts[index]->assignstmt->iden->idens[0]->content;
            auto expr = stat.stmts[index]->assignstmt->val;
            if(sset.findInAllScope(name) == false)
                throw epperr::Epperr("NameError", "Cannot find identifier named '" + name + "'", stat.stmts[index]->assignstmt->iden->idens[0]->line, stat.stmts[index]->assignstmt->iden->idens[0]->column);
            cenv::Calculation calc = _calc(*expr, sset);
            if(sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.isConst())
                throw epperr::Epperr("AssignError", "Cannot assign a value to a constant", stat.stmts[index]->assignstmt->iden->idens[0]->line, stat.stmts[index]->assignstmt->iden->idens[0]->column);
            if(sset.findInAllScope(name)){
                if(stat.stmts[index]->assignstmt->iden->getIdenType() == "__ARRE__"){
                    auto temp = sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)];
                    auto type = calc.result[0].first;
                    east::ValExprNode _temp;
                    _temp.addexpr = stat.stmts[index]->assignstmt->iden->arrindex;
                    auto arri = _calc(_temp, sset);
                    if(temp.second.getType() != type) throw epperr::Epperr("TypeError", "A value of a different type cannot be assigned to this variable",
                                                                           stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                                                           stat.stmts[index]->assignstmt->iden->idens[0]->column);
                    else{
                        if(type == "__INT__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal((int)calc.result[0].second, (int)arri.result[0].second);
                        else if(type == "__DECI__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal(calc.result[0].second, (int)arri.result[0].second);
                        else if(type == "__STRING__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal(calc.constpool[(int)calc.result[0].second], (int)arri.result[0].second, false);
                        else if(type == "__CHAR__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal(calc.constpool[(int)calc.result[0].second], (int)arri.result[0].second, true);
                        else if(type == "__BOOL__") sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal((bool)calc.result[0].second, (int)arri.result[0].second);
                    }
                }
                else{
                    auto temp = sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)];
                    auto type = calc.result[0].first;
                    if(temp.second.getType() != type) throw epperr::Epperr("TypeError", "A value of a different type cannot be assigned to this variable",
                                                                                stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                                                                stat.stmts[index]->assignstmt->iden->idens[0]->column);
                    if(temp.second.isArray()){
                        if(type == "__INT__")
                            for(int i = 0; i<temp.second.len; i++)
                                sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal((int)calc.result[i].second, i);
                        else if(type == "__DECI__")
                            for(int i = 0; i<temp.second.len; i++)
                                sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal((float)calc.result[i].second, i);
                        else if(type == "__BOOL__")
                            for(int i = 0; i<temp.second.len; i++)
                                sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal((bool)calc.result[i].second, i);
                        else if(type == "__STRING__")
                            for(int i = 0; i<temp.second.len; i++)
                                sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal(calc.constpool[(int)calc.result[i].second], i, false);
                        else if(type == "__CHAR__")
                            for(int i = 0; i<temp.second.len; i++)
                                sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.arr_setVal(calc.constpool[(int)calc.result[i].second], i, true);
                    }
                    else{
                        if(type == "__INT__") sset.assignValue(name, var::Value(false, false,((int)calc.result[0].second)));
                        else if(type == "__DECI__") sset.assignValue(name, var::Value(false, false, (calc.result[0].second)));
                        else if(type == "__STRING__") sset.assignValue(name, var::Value(false, false, calc.constpool[(int)calc.result[0].second], false));
                        else if(type == "__CHAR__") sset.assignValue(name, var::Value(false, false, calc.constpool[(int)calc.result[0].second], true));
                        else if(type == "__BOOL__") sset.assignValue(name, var::Value(false, false, ((bool)calc.result[0].second)));
                    }
                }
            }
            else throw epperr::Epperr("NameError", "Unable to find identifier named: '" + name + "'",
                                      stat.stmts[index]->assignstmt->iden->idens[0]->line,
                                      stat.stmts[index]->assignstmt->iden->idens[0]->column);
        }
        else if(stat.stmts[index]->deletestmt != nullptr){
            parse_DeleteStmt(stat.stmts[index]->deletestmt);
        }
        else if(stat.stmts[index]->blockstmt != nullptr){
            if(stat.stmts[index]->blockstmt->body == nullptr) continue;
            Parser subp;
            subp.stat = *stat.stmts[index]->blockstmt->body;
            this->sset.next();
            this->sset.newScope("__epp_temp_scope__");
            subp.sset = this->sset;
            subp.parse();
            this->sset.remove();
        }
        else if(stat.stmts[index]->ifstmt != nullptr){
            cenv::Calculation calc = _calc(*stat.stmts[index]->ifstmt->cond, sset);
            if(calc.result[0].first != "__STRING__" && calc.result[0].second > 0){
                parser::Parser stc_p;
                if(stat.stmts[index]->ifstmt->stc != nullptr){
                    east::StatNode _stat;
                    _stat.stmts.push_back(stat.stmts[index]->ifstmt->stc);
                    stc_p.stat = _stat;
                    stc_p.sset = sset;
                    stc_p.parse();
                }
                else{
                    if(stat.stmts[index]->ifstmt->body->body == nullptr) continue;
                    stc_p.stat = *stat.stmts[index]->ifstmt->body->body;
                    this->sset.next();
                    this->sset.newScope("__epp_ifTemp_scope__");
                    stc_p.sset = this->sset;
                    stc_p.parse();
                    this->sset.remove();
                }
                sset = stc_p.sset;
                _if_control = 1;
            }
            else {
                _if_control = 0;
                continue;
            }
        }
        else if(stat.stmts[index]->whilestmt != nullptr){
            cenv::Calculation calc = _calc(*stat.stmts[index]->whilestmt->cond, sset);
            try{
                while(calc.result[0].first != "__STRING__" && calc.result[0].second > 0){
                    parser::Parser stc_p;
                    sset.remove();
                    if(stat.stmts[index]->whilestmt->stc != nullptr){
                        east::StatNode _stat;
                        _stat.stmts.push_back(stat.stmts[index]->whilestmt->stc);
                        stc_p.stat = _stat;
                        stc_p.sset = sset;
                        stc_p.parse();
                    }
                    else{
                        if(stat.stmts[index]->whilestmt->body->body == nullptr) continue;
                        stc_p.stat = *stat.stmts[index]->whilestmt->body->body;
                        this->sset.next();
                        this->sset.newScope("__epp_whileTemp_scope__");
                        stc_p.sset = this->sset;
                        stc_p.parse();
                        this->sset.remove();
                    }
                    sset = stc_p.sset;
                    calc = _calc(*stat.stmts[index]->whilestmt->cond, sset);
                    if(calc.result[0].second == 0) break;
                }
            }
            catch(excphandling::Excp e){
                std::cout<<e.excpTitle<<"\n"<<e.excpContent<<std::endl;
            }
            catch(...){}
        }
        else if(stat.stmts[index]->reptstmt != nullptr){
            cenv::Calculation calc = _calc(*stat.stmts[index]->reptstmt->cond, sset);
            if(calc.result[0].first != "__INT__" || calc.isArray())
                throw epperr::Epperr("TypeError", "The condition of the repeat statement must be an integer",
                                     stat.stmts[index]->reptstmt->mark->line,
                                     stat.stmts[index]->reptstmt->mark->column);
            try{
                int i=1;
                while(i<=(int)calc.result[0].second){
                    i+=1;
                    parser::Parser stc_p;
                    if(stat.stmts[index]->reptstmt->stc != nullptr){
                        east::StatNode _stat;
                        _stat.stmts.push_back(stat.stmts[index]->reptstmt->stc);
                        stc_p.stat = _stat;
                        stc_p.sset = sset;
                        stc_p.parse();
                    }
                    else{
                        if(stat.stmts[index]->reptstmt->body->body == nullptr) continue;
                        stc_p.stat = *stat.stmts[index]->reptstmt->body->body;
                        this->sset.next();
                        this->sset.newScope("__epp_repeatTemp_scope__");
                        stc_p.sset = this->sset;
                        stc_p.parse();
                        this->sset.remove();
                    }
                    sset = stc_p.sset;
                }
            }
            catch(excphandling::Excp e){
                std::cout<<e.excpTitle<<"\n"<<e.excpContent<<std::endl;
            }
            catch(...){}
        }
        else if(stat.stmts[index]->brkstmt != nullptr){
            throw epperr::Epperr("SyntaxError", "You cannot use the 'break' statement outside the loop body", stat.stmts[index]->brkstmt->mark->line, stat.stmts[index]->brkstmt->mark->column);
        }
        else if(stat.stmts[index]->elifstmt != nullptr){
            cenv::Calculation calc = _calc(*stat.stmts[index]->elifstmt->cond, sset);
            if(_if_control == -1)
                throw epperr::Epperr("SyntaxError", "Cannot use else statement without else-if statement",
                                     stat.stmts[index]->elsestmt->mark->line,
                                     stat.stmts[index]->elsestmt->mark->column);
            if((_if_control == 0) && (calc.result[0].first != "__STRING__" && calc.result[0].second > 0)){
                parser::Parser stc_p;
                if(stat.stmts[index]->elifstmt->stc != nullptr){
                    east::StatNode _stat;
                    _stat.stmts.push_back(stat.stmts[index]->elifstmt->stc);
                    stc_p.stat = _stat;
                    stc_p.sset = sset;
                    stc_p.parse();
                }
                else{
                    if(stat.stmts[index]->elifstmt->body->body == nullptr) continue;
                    stc_p.stat = *stat.stmts[index]->elifstmt->body->body;
                    this->sset.next();
                    this->sset.newScope("__epp_elifTemp_scope__");
                    stc_p.sset = this->sset;
                    stc_p.parse();
                    this->sset.remove();
                }
                sset = stc_p.sset;
                _if_control = 1;
            }
            else ;
        }
        else if(stat.stmts[index]->elsestmt != nullptr){
            if(_if_control == -1)
                throw epperr::Epperr("SyntaxError", "Cannot use else statement without if statement",
                                     stat.stmts[index]->elsestmt->mark->line,
                                     stat.stmts[index]->elsestmt->mark->column);
            if(_if_control == 0){
                parser::Parser stc_p;
                if(stat.stmts[index]->elsestmt->stc != nullptr){
                    east::StatNode _stat;
                    _stat.stmts.push_back(stat.stmts[index]->elsestmt->stc);
                    stc_p.stat = _stat;
                    stc_p.sset = sset;
                    stc_p.parse();
                }
                else{
                    if(stat.stmts[index]->elsestmt->body->body == nullptr) continue;
                    stc_p.stat = *stat.stmts[index]->elsestmt->body->body;
                    this->sset.next();
                    this->sset.newScope("__epp_elifTemp_scope__");
                    stc_p.sset = this->sset;
                    stc_p.parse();
                    this->sset.remove();
                }
                sset = stc_p.sset;
            }
            else _if_control = -1;
        }
        else if(stat.stmts[index]->forstmt != nullptr){
            parse_ForStmt(stat.stmts[index]->forstmt);
        }
        else if(stat.stmts[index]->foreachstmt != nullptr){
            std::vector<cenv::calc_unit> list;
            auto name = stat.stmts[index]->foreachstmt->iden->content;
            cenv::Calculation calc = _calc(*stat.stmts[index]->foreachstmt->ariden, sset);
            if(calc.isArray());
            else if(!calc.isArray()) throw epperr::Epperr("TypeError", "Cannot traverse an object that is not an array", 
                stat.stmts[index]->foreachstmt->ariden->addexpr->muls[0]->prims[0]->iden->idens[0]->line,
                stat.stmts[index]->foreachstmt->ariden->addexpr->muls[0]->prims[0]->iden->idens[0]->column);
            else list = calc.result;
            try{
                int i = 0;
                while (i < calc.result.size()){
                    i += 1;
                    parser::Parser stc_p;
                    if (stat.stmts[index]->foreachstmt->stc != nullptr){
                        east::StatNode _stat;
                        _stat.stmts.push_back(stat.stmts[index]->foreachstmt->stc);
                        this->sset.scope_pool[this->sset.getDeep()].new_var(name, var::Value(false, false, calc.result[0].first));
                        if(calc.result[0].first == "__INT__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val((int)calc.result[i-1].second);
                        else if(calc.result[0].first == "__DECI__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val(calc.result[i-1].second);
                        else if(calc.result[0].first == "__BOOL__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val((bool)calc.result[i-1].second);
                        else if(calc.result[0].first == "__STRING__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val(calc.constpool[(int)calc.result[i-1].second], false);
                        else if(calc.result[0].first == "__CHAR__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val(calc.constpool[(int)calc.result[i-1].second], true);
                        stc_p.stat = _stat;
                        stc_p.sset = sset;
                        stc_p.parse();
                    }
                    else{
                        if(stat.stmts[index]->foreachstmt->body->body == nullptr) continue;
                        stc_p.stat = *stat.stmts[index]->foreachstmt->body->body;
                        this->sset.next();
                        this->sset.newScope("__epp_foreachTemp_scope__");
                        this->sset.scope_pool[this->sset.getDeep()].new_var(name, var::Value(false, false, calc.result[0].first));
                        if(calc.result[0].first == "__INT__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val((int)calc.result[i-1].second);
                        else if(calc.result[0].first == "__DECI__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val(calc.result[i-1].second);
                        else if(calc.result[0].first == "__BOOL__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val((bool)calc.result[i-1].second);
                        else if(calc.result[0].first == "__STRING__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val(calc.constpool[(int)calc.result[i-1].second], false);
                        else if(calc.result[0].first == "__CHAR__")
                            sset.scope_pool[sset.findInAllScopeI(name)].vars[sset.scope_pool[sset.findInAllScopeI(name)].findI(name)].second.set_val(calc.constpool[(int)calc.result[i-1].second], true);
                        stc_p.sset = this->sset;
                        stc_p.parse();
                        this->sset.remove();
                    }
                    sset = stc_p.sset;
                }
            }
            catch(excphandling::Excp e){
                std::cout<<e.excpTitle<<"\n"<<e.excpContent<<std::endl;
            }
            catch (...){
            }
        }
        else if(stat.stmts[index]->areastmt != nullptr){
            auto users = var::UserScope(stat.stmts[index]->areastmt->iden->content, *stat.stmts[index]->areastmt->body->body);
            
        }
        else if(stat.stmts[index]->exprstmt != nullptr){
            parse_ExprStmt(stat.stmts[index]->exprstmt);
        }
    }
}
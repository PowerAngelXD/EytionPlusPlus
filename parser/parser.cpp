#include "parser.h"

//FunctionParser
cenv::Calculation parser::funcParser::call(Parser p, east::FuncCallExprNode* node){
    //test: var foo=func(s:string, age:integer){println(s+"is"+str(age));};
    cenv::Calculation calc;
    auto iden = node->func_name->idens[0];
    auto name = node->func_name->idens[0]->content;
    if(p.sset.getTargetVar(name).second.isFunc() == false) throw epperr::Epperr("TypeError", "This identifier is not a callable object", iden->line, iden->column);
    auto funcObj = p.sset.getTargetVar(name).second.getValueOfFunc();
    if(p.sset.findInAllScope(name) == false)
        throw epperr::Epperr("NameError", "No function with name '" + name + "'", iden->line, iden->column);

    else {
        p.sset.next();
        p.sset.newScope("__epp_FuncTemp_scope__");
        p.stat = *p.sset.getTargetVar(name).second.getValueOfFunc().body->body;
        //para
        for(int i = 0; i < funcObj.nor_para.size(); i++){
            cenv::Calculation _calc = parser::getCalc(*node->act_paras[i], p.sset);
            if(getTypestring(funcObj.nor_para[i]->type->content) == _calc.result[0].first){
                if(_calc.result[0].first == "__STRING__"){
                    p.sset.createVariable(funcObj.nor_para[i]->name->content, var::Value(false, false, _calc.constpool[_calc.result[0].second], false));
                }
                else if(_calc.result[0].first == "__CHAR__")
                    p.sset.createVariable(funcObj.nor_para[i]->name->content, var::Value(false, false, _calc.constpool[_calc.result[0].second], true));
                else if(_calc.result[0].first == "__INT__")
                    p.sset.createVariable(funcObj.nor_para[i]->name->content, var::Value(false, false, (int)_calc.result[0].second));
                else if(_calc.result[0].first == "__DECI__")
                    p.sset.createVariable(funcObj.nor_para[i]->name->content, var::Value(false, false, (float)_calc.result[0].second));
                else if(_calc.result[0].first == "__BOOL__")
                    p.sset.createVariable(funcObj.nor_para[i]->name->content, var::Value(false, false, (bool)_calc.result[0].second));
            }
        }
        //
        p.parse();
        
        p.sset.remove();
    }
    return calc;
}

//Parser/BifParser
std::string parser::getTypestring(std::string type){
    std::string ret;
    if(type == "integer") ret = "__INT__";
    else if(type == "decimal") ret = "__DECI__";
    else if(type == "boolean") ret = "__BOOL__";
    else if(type == "string") ret = "__STRING__";
    else if(type == "char") ret = "__CHAR__";
    else throw epperr::EppClierr("parser::getTypestring ERROR: cannot transformation");
    return ret;
}

cenv::Calculation parser::getCalc(east::ValExprNode node, var::ScopeSet sset) {
    cenv::Calculation calc(sset);
    cvisitor::visitor v;
    if(node.addexpr != nullptr){
        if(node.addexpr->muls[0]->prims[0]->bif != nullptr){ // Check if it is a BIF
            if(node.addexpr->muls[0]->prims[0]->bif->bifi != nullptr){ // Check if it is a BIF_INSTANCE
                parser::BifParser bif(node.addexpr->muls[0]->prims[0]->bif->bifi, sset);
                if(bif.bif->mark->content == "system"){
                    calc = bif.bif_Sytem();
                }
                sset = bif._sset;
                goto end;
            }
            else
                v.visitAddExpr(node.addexpr);
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

efunc::Efunction parser::getCalc(east::FuncDefineExprNode node, var::ScopeSet sset){
    efunc::Efunction func;
    for(auto unit: node.paras)
        func.nor_para.push_back(unit);
    func.body = node.body;
    return func;
}

cenv::Calculation parser::getCalc(east::AssignExprNode node, var::ScopeSet sset){
    cenv::Calculation calc(sset);
    cvisitor::visitor v;
    v.visitAssignExpr(&node);
    calc.ins = v.ins; calc.constpool = v.constpool;
    calc.run();
    sset = calc.sset;
    return calc;
}

cenv::Calculation parser::getCalc(east::BoolExprNode node, var::ScopeSet sset) {
    cenv::Calculation calc(sset);
    cvisitor::visitor v;
    v.visitBoolExpr(&node);
    calc.ins = v.ins; calc.constpool = v.constpool;
    calc.run();
    sset = calc.sset;
    return calc;
}


//-------------------//BIF

parser::BifParser::BifParser(east::BifInstanceNode* bif, var::ScopeSet sset): bif(bif), _sset(sset) {}

cenv::Calculation parser::BifParser::bif_Sytem(){
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
                calc.result.clear();
                calc.result.push_back(cenv::calc_unit("__NULL__", 0.0));
                return calc;
            }
            else throw epperr::Epperr("SyntaxError", "There are no overloaded functions that meet the requirements: 'system(cmd: string)'", bif->mark->line, bif->mark->column);
        }
    }
}

//------------------//PARSER

void parser::Parser::parse_OutStmt(east::OutStmtNode* stmt){
    cenv::Calculation calc = getCalc(*stmt->content, this->sset);
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
    if(stmt->assign != nullptr){
        cenv::Calculation calc = getCalc(*stmt->assign, sset);
        this->sset = calc.sset;
    }
    else if(stmt->expr->addexpr->muls[0]->prims[0]->fcall != nullptr){
        parser::funcParser::call(*this, stmt->expr->addexpr->muls[0]->prims[0]->fcall);
    }
    else{
        cenv::Calculation calc = getCalc(*stmt->expr, sset);
        this->sset = calc.sset;
    }
}

void parser::Parser::parse_VorcStmt(east::VorcStmtNode* stmt){
    auto name = stmt->iden->content;
    if(stmt->value->fdefexpr != nullptr){
        efunc::Efunction func = getCalc(*stmt->value->fdefexpr, sset);
        if (stmt->type == nullptr);
        else{
            if (stmt->type->content == "integer") func.ret_type = "__INT__";
            else if (stmt->type->content == "string") func.ret_type = "__STRING__";
            else if (stmt->type->content == "decimal") func.ret_type = "__DECI__";
            else if (stmt->type->content == "boolean") func.ret_type = "__BOOL__";
            else if (stmt->type->content == "char") func.ret_type = "__CHAR__";
        }
        if(stmt->mark->content == "const") sset.createVariable(stmt->iden->content, var::Value(false, true, func));
        else sset.createVariable(stmt->iden->content, var::Value(false, false, func));
    }
    else{
        auto calc = getCalc(*stmt->value, sset);
        // type checker
        auto type = stmt->type;
        if (type == nullptr);
        else{
            if (type->content == "integer" && calc.result[0].first == "__INT__");
            else if (type->content == "string" && calc.result[0].first == "__STRING__");
            else if (type->content == "decimal" && calc.result[0].first == "__DECI__");
            else if (type->content == "boolean" && calc.result[0].first == "__BOOL__");
            else if (type->content == "char" && calc.result[0].first == "__CHAR__");
            else
                throw epperr::Epperr("TypeError", "The pre type at the time of declaration does not match the actual incoming type", type->line, type->column);
        }
        // Check for duplicate variables / constants
        if (sset.findInAllScope(name))
            throw epperr::Epperr("NameError", "duplicate identifier '" + name + "'!", stmt->iden->line, stmt->iden->column);
        // create variable
        if (calc.isArray()){
            if (calc.result[0].first == "__INT__"){
                std::vector<int> list;
                for (int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.result[i].second);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const" ? true : false, list));
            }
            else if (calc.result[0].first == "__DECI__"){
                std::vector<float> list;
                for (int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.result[i].second);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const" ? true : false, list));
            }
            else if (calc.result[0].first == "__STRING__"){
                std::vector<std::string> list;
                for (int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.constpool[calc.result[i].second]);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const" ? true : false, list, false));
            }
            else if (calc.result[0].first == "__CHAR__"){
                std::vector<std::string> list;
                for (int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.constpool[calc.result[i].second]);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const" ? true : false, list, true));
            }
            else if (calc.result[0].first == "__BOOL__"){
                std::vector<bool> list;
                for (int i = 0; i < calc.result.size(); i++)
                    list.push_back(calc.result[i].second);
                sset.createVariable(name, var::Value(true, stmt->mark->content == "const" ? true : false, list));
            }
        }
        else{
            if (calc.result[0].first == "__STRING__")
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const" ? true : false,
                                                     calc.constpool[calc.result[0].second], false));
            else if (calc.result[0].first == "__CHAR__")
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const" ? true : false,
                                                     calc.constpool[calc.result[0].second], false));
            else if (calc.result[0].first == "__INT__")
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const" ? true : false,
                                                     (int)calc.result[0].second));
            else if (calc.result[0].first == "__NULL__")
                sset.createVariable(name, var::Value(true));
            else
                sset.createVariable(name, var::Value(false, stmt->mark->content == "const" ? true : false,
                                                     calc.result[0].second));
        }
    }
}

void parser::Parser::parse_ForStmt(east::ForStmtNode* stmt){
    auto name = stmt->iden->idens[0]->content;
    if(stmt->var_mark != nullptr)
        sset.createVariable(name, var::Value(false, false, getCalc(*stmt->val, sset).result[0].second));
    auto calcBool = getCalc(*stmt->cond, sset);
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
            this->sset.remove(); stc_p.sset.remove();
        }
        //后置语句
        east::StatNode stat;
        stat.stmts.push_back(stmt->dostc);
        if(stmt->dostc->exprstmt!=nullptr && stmt->dostc->exprstmt->expr->addexpr->muls[0]->prims[0]->siad!=nullptr){
            stc_p.stat = stat;
            stc_p.sset = sset;
            stc_p.parse();
        }
        else if(stmt->dostc->exprstmt!=nullptr && stmt->dostc->exprstmt->assign != nullptr){
            stc_p.stat = stat;
            stc_p.sset = sset;
            stc_p.parse();
        }
        else throw epperr::Epperr("SyntaxError", "Not the correct statement", stmt->separate_sym2->line, stmt->separate_sym2->column);
        //

        sset = stc_p.sset;
        calcBool = getCalc(*stmt->cond, sset);
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
            cenv::Calculation calc = getCalc(*stat.stmts[index]->ifstmt->cond, sset);
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
            cenv::Calculation calc = getCalc(*stat.stmts[index]->whilestmt->cond, sset);
            try{
                while(calc.result[0].first != "__STRING__" && calc.result[0].second > 0){
                    parser::Parser stc_p;
                    //sset.remove();
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
                        this->sset.remove(); stc_p.sset.remove();
                    }
                    sset = stc_p.sset;
                    calc = getCalc(*stat.stmts[index]->whilestmt->cond, sset);
                    if(calc.result[0].second == 0) break;
                }
            }
            catch(excphandling::Excp e){
                std::cout<<e.excpTitle<<"\n"<<e.excpContent<<std::endl;
            }
            catch(...){}
        }
        else if(stat.stmts[index]->reptstmt != nullptr){
            cenv::Calculation calc = getCalc(*stat.stmts[index]->reptstmt->cond, sset);
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
                        this->sset.remove(); stc_p.sset.remove();
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
            cenv::Calculation calc = getCalc(*stat.stmts[index]->elifstmt->cond, sset);
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
            cenv::Calculation calc = getCalc(*stat.stmts[index]->foreachstmt->ariden, sset);
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
                        this->sset.remove(); stc_p.sset.remove();
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
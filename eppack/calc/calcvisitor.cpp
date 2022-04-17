#include "calcvisitor.h"

void cvisitor::visitor::visitNull(epplex::Token* token){
    ins.push_back({"__PUSH__", cenv::calc_unit("__NULL__", 0), "__NULL__", token->line, token->column});
}
void cvisitor::visitor::visitNumber(epplex::Token* token){
    if(token->content.find(".") != token->content.npos) {
        ins.push_back({"__PUSH__", cenv::calc_unit("__DECI__", atof(token->content.c_str())), "__NULL__", token->line,
                       token->column});
    }
    else{
        ins.push_back({"__PUSH__", cenv::calc_unit("__INT__", atoi(token->content.c_str())), "__NULL__", token->line,
                       token->column});
    }

}
void cvisitor::visitor::visitString(epplex::Token* token){
    std::string content = token->content;
    std::string result;
    for(int i = 0; i < content.size(); i++){
        if(content[i] == '\\') {
            switch (content[i + 1]) {
                case '\\': result += '\\'; break;
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case '0': result += '\0'; break;
                default: break;
            }
            i++;
        }
        else result += content[i];
    }
    constpool.push_back(result);
    ins.push_back({"__PUSH__", cenv::calc_unit("__STRING__", constpool.size()-1), "__NULL__", token->line, token->column});
}
void cvisitor::visitor::visitChar(epplex::Token* token){
    char ch = token->content[0];
    switch (ch) {
        case '\\': ch = '\\'; break;
        case 'n': ch = '\n'; break;
        case 't': ch = '\t'; break;
        case 'r': ch = '\r'; break;
        case '0': ch = '\0'; break;
        default: break;
    }
    std::string ch_s;
    ch_s.push_back(ch);
    constpool.push_back(ch_s);
    ins.push_back({"__PUSH__", cenv::calc_unit("__CHAR__", constpool.size()-1), "__NULL__", token->line, token->column});
}
void cvisitor::visitor::visitSiad(east::SelfIaDExprNode* node){
    visitIden(node->iden);
    if(node->op->content == "++"){
        if(node->isFront)
            ins.push_back({"__SIADADD_FRONT__", cenv::calc_unit("__NULL__", 0.0), node->iden->idens[0]->content, node->iden->idens[0]->line, node->iden->idens[0]->column});
        else
            ins.push_back({"__SIADADD__", cenv::calc_unit("__NULL__", 0.0), node->iden->idens[0]->content, node->iden->idens[0]->line, node->iden->idens[0]->column});
    }
    else{
        if(node->isFront)
            ins.push_back({"__SIADSUB_FRONT__", cenv::calc_unit("__NULL__", 0.0), node->iden->idens[0]->content, node->iden->idens[0]->line, node->iden->idens[0]->column});
        else
            ins.push_back({"__SIADSUB__", cenv::calc_unit("__NULL__", 0.0), node->iden->idens[0]->content, node->iden->idens[0]->line, node->iden->idens[0]->column});
    }
}
void cvisitor::visitor::visitTpof(east::TypeOfExprNode* node){
    if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
    else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
    else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
    ins.push_back({"__TPOF__", cenv::calc_unit("__NULL__", 0.0), "__NULL__", node->mark->line, node->mark->column});
}
void cvisitor::visitor::visitInput(east::InputExprNode* node){
    if(node->expr == nullptr) ins.push_back({"__INPUT__", cenv::calc_unit("__NULL__", 0.0), " ", node->mark->line, node->mark->column});
    else{
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        ins.push_back({"__INPUT__", cenv::calc_unit("__NULL__", 0.0), "__NULL__", node->mark->line, node->mark->column});
    }
}
void cvisitor::visitor::visitPrintoLn(east::PrintoLnExprNode* node){
    if(node->mark->content == "print"){
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
        else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
        ins.push_back({"__PRINT__", cenv::calc_unit("__NULL__", 0.0), "NOLINE", node->mark->line, node->mark->column});
    }
    else if(node->mark->content == "println"){
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
        else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
        ins.push_back({"__PRINT__", cenv::calc_unit("__NULL__", 0.0), "LINE", node->mark->line, node->mark->column});
    }
}
void cvisitor::visitor::visitTypeTo(east::TypeToExprNode* node){
    if(node->mark->content == "int"){
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
        else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
        ins.push_back({"__TYT__", cenv::calc_unit("__NULL__", 0.0), "__INT__", node->mark->line, node->mark->column});
    }
    else if(node->mark->content == "str"){
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
        else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
        ins.push_back({"__TYT__", cenv::calc_unit("__NULL__", 0.0), "__STRING__", node->mark->line, node->mark->column});
    }
    else if(node->mark->content == "bool"){
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
        else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
        ins.push_back({"__TYT__", cenv::calc_unit("__NULL__", 0.0), "__BOOL__", node->mark->line, node->mark->column});
    }
    else if(node->mark->content == "deci"){
        if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
        else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
        else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
        ins.push_back({"__TYT__", cenv::calc_unit("__NULL__", 0.0), "__DECI__", node->mark->line, node->mark->column});
    }
}
void cvisitor::visitor::visitLen(east::LenExprNode* node){
    if(node->expr->addexpr != nullptr) visitAddExpr(node->expr->addexpr);
    else if(node->expr->boolexpr != nullptr) visitBoolExpr(node->expr->boolexpr);
    else if(node->expr->listexpr != nullptr) visitListExpr(node->expr->listexpr);
    ins.push_back({"__LEN__", cenv::calc_unit("__NULL__", 0.0), "__NULL__", node->mark->line, node->mark->column});
}
void cvisitor::visitor::visitIden(east::IdentifierNode* node){
    if(node->getIdenType() == "__ARRE__"){
        visitAddExpr(node->arrindex);
        ins.push_back({"__ARRE_POP__", cenv::calc_unit("__NULL__", 0.0), node->idens[0]->content, node->idens[0]->line, node->idens[0]->column});
    }
    else
        ins.push_back({"__POP__", cenv::calc_unit("__NULL__", 0.0), node->idens[0]->content, node->idens[0]->line, node->idens[0]->column});// TODO: After updating the scope, it needs to be adapted to the scope
}
void cvisitor::visitor::visitAddOp(east::AddOpNode* node){
    ins.push_back({node->op->content, cenv::calc_unit(node->op->content, 0.0), "__NULL__", node->op->line, node->op->column});
}
void cvisitor::visitor::visitMulOp(east::MulOpNode* node){
    ins.push_back({node->op->content, cenv::calc_unit(node->op->content, 0.0), "__NULL__", node->op->line, node->op->column});
}
void cvisitor::visitor::visitPrimExpr(east::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->null != nullptr) visitNull(node->null);
    else if(node->boolconst != nullptr) {
        if(node->boolconst->content == "true")
            ins.push_back({"__PUSH__", cenv::calc_unit("__BOOL__", true), "__NULL__", node->boolconst->line, node->boolconst->column});
        else if(node->boolconst->content == "false"){
            ins.push_back({"__PUSH__", cenv::calc_unit("__BOOL__", false), "__NULL__", node->boolconst->line, node->boolconst->column});
        }
    }
    else if(node->str != nullptr) visitString(node->str);
    else if(node->ch != nullptr) visitChar(node->ch);
    else if(node->iden != nullptr) visitIden(node->iden);
    else if(node->siad != nullptr) visitSiad(node->siad);
    else if(node->addexpr != nullptr) visitAddExpr(node->addexpr);
    else if(node->bif != nullptr){
        if(node->bif->typef != nullptr) visitTpof(node->bif->typef);
        else if(node->bif->input != nullptr) visitInput(node->bif->input);
        else if(node->bif->len != nullptr) visitLen(node->bif->len);
        else if(node->bif->tyt != nullptr) visitTypeTo(node->bif->tyt);
        else if(node->bif->print != nullptr) visitPrintoLn(node->bif->print);
    }
}
void cvisitor::visitor::visitMulExpr(east::MulExprNode* node){
    visitPrimExpr(node->prims[0]);
    for(int i = 0; i < node->ops.size(); i++){
        visitPrimExpr(node->prims[i + 1]);
        visitMulOp(node->ops[i]);
    }
}
void cvisitor::visitor::visitAddExpr(east::AddExprNode* node){
    visitMulExpr(node->muls[0]);
    for(int i = 0; i < node->ops.size(); i++){
        visitMulExpr(node->muls[i + 1]);
        visitAddOp(node->ops[i]);
    }
}
void cvisitor::visitor::visitListExpr(east::ListExprNode* node){
    int len = 0;
    for(len; len < node->arrayelts.size(); len++) {
        if(node->arrayelts[len]->addexpr != nullptr)
            visitAddExpr(node->arrayelts[len]->addexpr);
        else if(node->arrayelts[len]->boolexpr != nullptr)
            visitBoolExpr(node->arrayelts[len]->boolexpr);
        else if(node->arrayelts[len]->listexpr != nullptr)
            visitListExpr(node->arrayelts[len]->listexpr);
    }
    ins.push_back({"__PUSHA__", cenv::calc_unit("__NULL__", 0.0), std::to_string(len), node->left->line, node->left->column});
}
void cvisitor::visitor::visitCmpOp(east::CmpOpNode* node){
    ins.push_back({node->op->content, cenv::calc_unit(node->op->content, 0.0), "__NULL__", node->op->line, node->op->column});
}
void cvisitor::visitor::visitBoolOp(east::BoolOpNode* node){
    ins.push_back({node->op->content, cenv::calc_unit(node->op->content, 0.0), "__NULL__", node->op->line, node->op->column});
}
void cvisitor::visitor::visitCmpExpr(east::CmpExprNode* node){
    visitAddExpr(node->expr);
    if(node->op != nullptr){
        visitAddExpr(node->target);
        visitCmpOp(node->op);
    }
}
void cvisitor::visitor::visitBoolExpr(east::BoolExprNode* node){
    visitCmpExpr(node->cmps[0]);
    for(int i = 0; i < node->ops.size(); i++){
        visitCmpExpr(node->cmps[i + 1]);
        visitBoolOp(node->ops[i]);
    }
    if(node->notsign != nullptr) ins.push_back({"!", cenv::calc_unit(node->notsign->content, 0.0), "__NULL__", node->notsign->line, node->notsign->column});
}
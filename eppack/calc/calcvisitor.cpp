#include "calcvisitor.h"

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
    ins.push_back({"__PUSH__", cenv::calc_unit("__CHAR__", ch), "__NULL__", token->line, token->column});
}
void cvisitor::visitor::visitIden(east::IdentifierNode* node){
    ins.push_back({"__PUSH__", cenv::calc_unit("__NULL__", 0.0), node->idens[0]->content, node->idens[0]->line, node->idens[0]->column}); // TODO: After updating the scope, it needs to be adapted to the scope
}
void cvisitor::visitor::visitAddOp(east::AddOpNode* node){
    ins.push_back({node->op->content, cenv::calc_unit(node->op->content, 0.0), "__NULL__", node->op->line, node->op->column});
}
void cvisitor::visitor::visitMulOp(east::MulOpNode* node){
    ins.push_back({node->op->content, cenv::calc_unit(node->op->content, 0.0), "__NULL__", node->op->line, node->op->column});
}
void cvisitor::visitor::visitPrimExpr(east::PrimExprNode* node){
    if(node->number != nullptr) visitNumber(node->number);
    else if(node->str != nullptr) visitString(node->str);
    else if(node->ch != nullptr) visitChar(node->ch);
    else if(node->iden != nullptr) visitIden(node->iden);
    else if(node->addexpr != nullptr) visitAddExpr(node->addexpr);
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
void cvisitor::visitor::visitCmpOp(east::CmpOpNode* node){

}
void cvisitor::visitor::visitBoolOp(east::BoolOpNode* node){

}
void cvisitor::visitor::visitCmpExpr(east::CmpExprNode* node){

}
void cvisitor::visitor::visitBoolExpr(east::BoolExprNode* node){

}
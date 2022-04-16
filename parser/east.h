/**
 * @file east.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief ast file
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "../lexer/eplex.h"
#include "../eppack/error/epperr.h"

namespace east{
    class AddExprNode; class ValExprNode; class IdentifierNode; class PrimExprNode; class BoolExprNode; class StatNode; 
    class astParser; class TypeOfExprNode; class BlockStmtNode; class IndexOpNode; class StmtNode;

    class AssignExprNode{
    public:
        IdentifierNode* iden;
        epplex::Token* equ;
        ValExprNode* val;
        epplex::Token* end;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class SelfIaDExprNode{
        //i++ / ++i;
    public:
        IdentifierNode* iden;
        epplex::Token* op;
        std::string tag = "__CALC__";
        bool isFront = false;

        std::string to_string();
        static bool is(astParser ap);
    };

    class WholeExprNode{
    public:
        AssignExprNode* assignexpr = nullptr;
        ValExprNode* valexpr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class IdentifierNode{
        /**
         * Field description:
         * __PURE__ : Pure identifier
         * __ARRE__ : Array reference identifier
         */
        std::string type = "__PURE__";
    public:
        std::vector<epplex::Token*> idens;
        std::vector<epplex::Token*> dots; // .

        std::vector<IndexOpNode*> indexops;
        epplex::Token* arrleft = nullptr;
        AddExprNode* arrindex = nullptr;
        epplex::Token* arrright = nullptr;
        std::string tag = "__CALC__";

        IdentifierNode(std::string type_ = "__PURE__");
        std::string to_string();
        std::string getIdenType();
        static bool is(astParser ap);
    };

    class AddOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class MulOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class CmpOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class BoolOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class IndexOpNode{
    public:
        epplex::Token* left;
        AddExprNode* begin;
        epplex::Token* spliter = nullptr;
        AddExprNode* end = nullptr;
        epplex::Token* right;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class MulExprNode{
    public:
        std::vector<PrimExprNode*> prims;
        std::vector<MulOpNode*> ops;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class AddExprNode{
    public:
        std::vector<MulExprNode*> muls;
        std::vector<AddOpNode*> ops;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class CmpExprNode{
    public:
        AddExprNode* expr;
        CmpOpNode* op=nullptr;
        AddExprNode* target=nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class BoolExprNode{
    public:
        epplex::Token* notsign = nullptr; // !
        std::vector<CmpExprNode*> cmps;
        std::vector<BoolOpNode*> ops;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ListExprNode{
    public:
        epplex::Token* left;
        std::vector<ValExprNode*> arrayelts;
        epplex::Token* right;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ScopeExprNode{
    public:
        epplex::Token* left;
        StatNode* body;
        epplex::Token* right;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class TypeOfExprNode{
    public:
        epplex::Token* mark;
        ValExprNode* expr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class InputExprNode{
    public:
        epplex::Token* mark;
        ValExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class PrintoLnExprNode{
    public:
        epplex::Token* mark;
        ValExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class TypeToExprNode{
    public:
        epplex::Token* mark;
        ValExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class LenExprNode{
    public:
        epplex::Token* mark;
        ValExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class BifInstanceNode{
    public:
        epplex::Token* mark;
        epplex::Token* left;
        std::vector<ValExprNode*> paras;
        std::vector<epplex::Token*> dots;
        epplex::Token* right;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class FuncDefineExprNode{
    public:
        struct Para{
        public:
            epplex::Token* name;
            epplex::Token* mh;
            epplex::Token* type;
            std::string to_string();
        };
        epplex::Token* mark;
        epplex::Token* left;
        std::vector<Para*> paras;
        std::vector<epplex::Token*> dots;
        epplex::Token* right;
        BlockStmtNode* body;
        std::string tag = "__FUNC_DEF__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class FuncCallExprNode{
    public:
        IdentifierNode* func_name;
        std::vector<ValExprNode*> act_paras;
        std::string tag = "__CALC_STMT__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class BifNode{
    public:
        LenExprNode* len=nullptr;
        TypeOfExprNode* typef=nullptr;
        TypeToExprNode* tyt=nullptr;
        PrintoLnExprNode* print=nullptr;
        InputExprNode* input=nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class PrimExprNode{
    public:
        epplex::Token* number = nullptr;
        epplex::Token* boolconst = nullptr;
        IdentifierNode* iden = nullptr;
        epplex::Token* str = nullptr;
        epplex::Token* ch = nullptr;
        BifNode* bif = nullptr;
        SelfIaDExprNode* siad = nullptr;
        epplex::Token* left = nullptr;
        FuncCallExprNode* fcall = nullptr;
        AddExprNode* addexpr = nullptr; // use '()'
        BoolExprNode* boolexpr = nullptr; // use '()'
        epplex::Token* right = nullptr;

        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ValExprNode{
    public:
        AddExprNode* addexpr = nullptr;
        BoolExprNode* boolexpr = nullptr;
        ListExprNode* listexpr = nullptr;
        FuncDefineExprNode* fdefexpr = nullptr;
        ScopeExprNode* scopeexpr = nullptr;

        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ExprStmtNode{
    public:
        ValExprNode* expr;

        epplex::Token* end;

        std::string tag = "__CALC__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class OutStmtNode{
        // out "hello";
    public:
        epplex::Token* mark;
        ValExprNode* content;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class VorcStmtNode{
        // var v = 1;
        // const c = "hey";
        // var list = [1,2,3,4];
        // var a:string = "dasd";
    public:
        epplex::Token* mark;
        epplex::Token* iden;
        epplex::Token* type_exp=nullptr;
        epplex::Token* type=nullptr;
        epplex::Token* equ;
        ValExprNode* value;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class AssignStmtNode{
        // a = 1;
        // a.b.c = 2;
        // list[3] = 5;
    public:
        IdentifierNode* iden;
        epplex::Token* equ;
        ValExprNode* val;
        epplex::Token* end = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class DeleteStmtNode{
        // delete c;
    public:
        epplex::Token* mark;
        IdentifierNode* iden;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class BlockStmtNode{
    public:
        epplex::Token* left;
        StatNode* body = nullptr;
        epplex::Token* right;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class BreakStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class AreaStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* iden;
        BlockStmtNode* body;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class IfStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* left;
        ValExprNode* cond;
        epplex::Token* right;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ElseifStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* left;
        ValExprNode* cond;
        epplex::Token* right;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ElseStmtNode{
    public:
        epplex::Token* mark;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class WhileStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* left;
        ValExprNode* cond;
        epplex::Token* right;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class RepeatStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* left;
        ValExprNode* cond;
        epplex::Token* right;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ForEachStmtNode{
    public:
        epplex::Token* mark; // for_each
        epplex::Token* left;
        epplex::Token* var_mark;
        epplex::Token* iden;
        epplex::Token* mh;
        ValExprNode* ariden;
        epplex::Token* right;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;

        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ForStmtNode{
        // for(var a=0; a < 10; a++/a=a+2){}
    public:
        epplex::Token* mark;
        epplex::Token* left;
        epplex::Token* var_mark=nullptr;
        IdentifierNode* iden;
        epplex::Token* eq;
        ValExprNode* val;

        epplex::Token* separate_sym1;

        BoolExprNode* cond;

        epplex::Token* separate_sym2;

        StmtNode* dostc;

        epplex::Token* right;
        BlockStmtNode* body=nullptr;
        StmtNode* stc=nullptr;

        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class DeferStmtNode{
    public:
        epplex::Token* mark;
        StmtNode* stc=nullptr;
        BlockStmtNode* body=nullptr;

        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class ActStmtNode{
    public:
        epplex::Token* mark;
        IdentifierNode* iden;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class TryStmtNode{};

    class CatchStmtNode{};

    class StmtNode{
    public:
        OutStmtNode* outstmt = nullptr;
        VorcStmtNode* vorcstmt = nullptr;
        AssignStmtNode* assignstmt = nullptr;
        DeleteStmtNode* deletestmt = nullptr;
        BlockStmtNode* blockstmt = nullptr;
        IfStmtNode* ifstmt = nullptr;
        ElseifStmtNode* elifstmt = nullptr;
        BreakStmtNode* brkstmt = nullptr;
        WhileStmtNode* whilestmt = nullptr;
        RepeatStmtNode* reptstmt = nullptr;
        ElseStmtNode* elsestmt = nullptr;
        ForEachStmtNode* foreachstmt = nullptr;
        AreaStmtNode* areastmt = nullptr;
        ForStmtNode* forstmt = nullptr;
        ExprStmtNode* exprstmt = nullptr;

        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };

    class StatNode{
    public:
        std::vector<StmtNode*> stmts;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is(astParser ap);
    };
    
    /**
     * @brief Class used to generate AST
     */
    class astParser{
    public:
        std::vector<epplex::Token> tg;
        int pos = 0; // belong to member: 'tg'
        astParser(std::vector<epplex::Token> tg_);
        /**
         * @brief Get the "Token" of the current position and increase the variable "pos" by one
         * @return epplex::Token 
         * 
         */
        epplex::Token* token();

        /**
         * @brief Get the "token" of the current location without increasing the variable "pos"" by one
         * @param pos_ Add "pos_" on the basis of "pos" Units
         * @return epplex::Token 
         * 
         */
        epplex::Token* peek(int pos_ = 0);

        WholeExprNode* gen_wholeExprNode();
        ValExprNode* gen_valExprNode();
        AddOpNode* gen_addOpNode();
        MulOpNode* gen_mulOpNode();
        IndexOpNode* gen_indexOpNode();
        CmpOpNode* gen_cmpOpNode();
        BoolOpNode* gen_boolOpNode();
        PrimExprNode* gen_primExprNode();
        IdentifierNode* gen_identifierNode();
        AssignExprNode* gen_assignExprNode();
        AddExprNode* gen_addExprNode();
        MulExprNode* gen_mulExprNode();
        CmpExprNode* gen_cmpExprNode();
        BoolExprNode* gen_boolExprNode();
        ListExprNode* gen_listExprNode();
        TypeOfExprNode* gen_tpofExprNode();
        InputExprNode* gen_inputExprNode();
        PrintoLnExprNode* gen_polnExprNode();
        FuncCallExprNode* gen_fcallExprNode();
        SelfIaDExprNode* gen_siadExprNode();
        FuncDefineExprNode* gen_fdefExprNode();
        LenExprNode* gen_lenExprNode();
        TypeToExprNode* gen_tytExprNode();
        BifNode* gen_bifNode();

        StmtNode* gen_stmtNode();
        StatNode* gen_statNode();
        OutStmtNode* gen_outStmtNode();
        VorcStmtNode* gen_vorcStmtNode();
        AssignStmtNode* gen_assignStmtNode(bool asexpr = false);
        DeleteStmtNode* gen_delStmtNode();
        IfStmtNode* gen_ifStmtNode();
        ElseifStmtNode* gen_elifStmtNode();
        ElseStmtNode* gen_elseStmtNode();
        BreakStmtNode* gen_brkStmtNode();
        ForEachStmtNode* gen_foreachStmtNode();
        WhileStmtNode* gen_whileStmtNode();
        RepeatStmtNode* gen_reptStmtNode();
        BlockStmtNode* gen_blockStmtNode();
        AreaStmtNode* gen_areaStmtNode();
        ForStmtNode* gen_forStmtNode();
        ExprStmtNode* gen_exprStmtNode();
    };
}
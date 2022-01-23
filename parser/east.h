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
    class AddExprNode;
    class ExprNode;
    class IdentifierNode;
    class PrimExprNode;
    class BoolExprNode;
    class StatNode;
    class astParser;
    class TypeOfExprNode;
    class StmtNode;

    class IdentifierNode{
        /**
         * Field description:
         * __PURE__ : Pure identifier
         * __ARRE__ : Array reference identifier
         * __FUNC__ : Function call identifier
         */
        std::string type = "__PURE__";
    public:
        std::vector<epplex::Token*> idens;
        std::vector<epplex::Token*> dots; // .

        epplex::Token* arrleft = nullptr;
        AddExprNode* arrindex = nullptr;
        epplex::Token* arrright = nullptr;
        std::string tag = "__CALC__";

        IdentifierNode(std::string type_ = "__PURE__");
        std::string to_string();
        std::string getIdenType();
        static bool is_it(astParser ap);
    };

    class AddOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class MulOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class CmpOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class BoolOpNode{
    public:
        epplex::Token* op;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class MulExprNode{
    public:
        std::vector<PrimExprNode*> prims;
        std::vector<MulOpNode*> ops;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class AddExprNode{
    public:
        std::vector<MulExprNode*> muls;
        std::vector<AddOpNode*> ops;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class CmpExprNode{
    public:
        AddExprNode* expr;
        CmpOpNode* op=nullptr;
        AddExprNode* target=nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class BoolExprNode{
    public:
        epplex::Token* notsign = nullptr; // !
        std::vector<CmpExprNode*> cmps;
        std::vector<BoolOpNode*> ops;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class ListExprNode{
    public:
        epplex::Token* left;
        std::vector<ExprNode*> arrayelts;
        epplex::Token* right;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class TypeOfExprNode{
    public:
        epplex::Token* mark;
        ExprNode* expr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class InputExprNode{
    public:
        epplex::Token* mark;
        ExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class ExprNode{
    public:
        AddExprNode* addexpr = nullptr;
        BoolExprNode* boolexpr = nullptr;
        ListExprNode* listexpr = nullptr;

        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class TypeToExprNode{
    public:
        epplex::Token* mark;
        ExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class LenExprNode{
    public:
        epplex::Token* mark;
        ExprNode* expr = nullptr;
        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class PrimExprNode{
    public:
        epplex::Token* number = nullptr;
        epplex::Token* boolconst = nullptr;
        IdentifierNode* iden = nullptr;
        epplex::Token* str = nullptr;
        epplex::Token* ch = nullptr;
        TypeOfExprNode* tpof = nullptr;
        InputExprNode* input = nullptr;
        TypeToExprNode* typeto = nullptr;
        LenExprNode* glen = nullptr;
        epplex::Token* left = nullptr;
        AddExprNode* addexpr = nullptr; // use '()'
        BoolExprNode* boolexpr = nullptr; // use '()'
        epplex::Token* right = nullptr;

        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class OutStmtNode{
        // out "hello";
    public:
        epplex::Token* mark;
        ExprNode* content;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class VorcStmtNode{
        // var v = 1;
        // const c = "hey";
        // var list = [1,2,3,4];
    public:
        epplex::Token* mark;
        epplex::Token* iden;
        epplex::Token* equ;
        ExprNode* value;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class AssignStmtNode{
        // a = 1;
        // a.b.c = 2;
        // list[3] = 5;
    public:
        IdentifierNode* iden;
        epplex::Token* equ;
        ExprNode* val;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class DeleteStmtNode{
        // delete c;
    public:
        epplex::Token* mark;
        IdentifierNode* iden;
        epplex::Token* end;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class BlockStmtNode{
    public:
        epplex::Token* left;
        StatNode* body;
        epplex::Token* right;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class AreaStmtNode{
        /*  area a{
         *      var b="hello ";
         *      var c="world!";
         *      out a+b;
         *  }
         *  out a.b+a.c;
         *  act a;
         *  ---OutPut---
         *  hello world!
         *  hello world!
         */
    public:

    };

    class IfStmtNode{
    public:
        epplex::Token* mark;
        epplex::Token* left;
        ExprNode* cond;
        epplex::Token* right;
        BlockStmtNode* body = nullptr;
        StmtNode* stc = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class StmtNode{
    public:
        OutStmtNode* outstmt = nullptr;
        VorcStmtNode* vorcstmt = nullptr;
        AssignStmtNode* assignstmt = nullptr;
        DeleteStmtNode* deletestmt = nullptr;
        BlockStmtNode* blockstmt = nullptr;
        IfStmtNode* ifstmt = nullptr;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class StatNode{
    public:
        std::vector<StmtNode*> stmts;
        std::string tag = "__OTHER__";

        std::string to_string();
        static bool is_it(astParser ap);
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

        ExprNode* gen_exprNode();
        AddOpNode* gen_addOpNode();
        MulOpNode* gen_mulOpNode();
        CmpOpNode* gen_cmpOpNode();
        BoolOpNode* gen_boolOpNode();
        PrimExprNode* gen_primExprNode();
        IdentifierNode* gen_identifierNode();
        AddExprNode* gen_addExprNode();
        MulExprNode* gen_mulExprNode();
        CmpExprNode* gen_cmpExprNode();
        BoolExprNode* gen_boolExprNode();
        ListExprNode* gen_listExprNode();
        TypeOfExprNode* gen_tpofExprNode();
        InputExprNode* gen_inputExprNode();
        LenExprNode* gen_lenExprNode();
        TypeToExprNode* gen_tytExprNode();

        StmtNode* gen_stmtNode();
        StatNode* gen_statNode();
        OutStmtNode* gen_outStmtNode();
        VorcStmtNode* gen_vorcStmtNode();
        AssignStmtNode* gen_assignStmtNode();
        DeleteStmtNode* gen_delStmtNode();
        IfStmtNode* gen_ifStmtNode();
        BlockStmtNode* gen_blockStmtNode();
    };
}
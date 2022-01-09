/**
 * @file east.h
 * @author PowerAngelXd (692732163@qq.com)
 * @brief ast file
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <string>
#include <vector>
#include "../lexer/eplex.h"
#include "../eppack/error/epperr.h"

namespace east{
    class AddExprNode;
    class ExprNode;
    class IdentifierNode;
    class PrimExprNode;
    class BoolExprNode;
    class astParser;

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

        epplex::Token* arrelt = nullptr;
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
        PrimExprNode* expr;
        CmpOpNode* op;
        PrimExprNode* target;
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

    class ExprNode{
    public:
        AddExprNode* addexpr = nullptr;
        BoolExprNode* boolexpr = nullptr;

        std::string tag = "__CALC__";

        std::string to_string();
        static bool is_it(astParser ap);
    };

    class PrimExprNode{
    public:
        epplex::Token* number = nullptr;
        IdentifierNode* iden = nullptr;
        epplex::Token* str = nullptr;
        epplex::Token* ch = nullptr;
        epplex::Token* left = nullptr;
        AddExprNode* addexpr = nullptr; // use '()'
        BoolExprNode* boolexpr = nullptr; // use '()'
        epplex::Token* right = nullptr;

        std::string tag = "__CALC__";

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
    };
}
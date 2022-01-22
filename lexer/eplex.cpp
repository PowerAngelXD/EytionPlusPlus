#include "eplex.h"
#include <map>
std::string epplex::Token::format(){
    return "(val: " + this->content + ", type" + this->type + ", detail_type: " + this->detail_type + ", ln: " + std::to_string(this->line) + ", col: " + std::to_string(this->column) + ")";
}
std::string epplex::Token::simply_format(){
    return "(val: " + this->content + ", tp: " + this->detail_type + ")";
}

epplex::Token epplex::Lexer::Num() {
    char ch = get();
    std::string str = "";
    bool dot = false;
    while (((!input.eof()) && isdigit(ch)) || ch == '.') {
        if(ch == '.' && !dot){ str += ch; dot = true;}
        else if(ch == '.')
            throw epperr::Epperr("SyntaxError", "Too many decimal points!", line, column);
        else str += ch;
        ch = get();
    }
    put(ch);
    return {str, "__NUMBER__", "__LITERAL__", line, column};
}

epplex::Token epplex::Lexer::Hex() {
    char ch = get();
    std::string str = "";
    while ((! input.eof()) && (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
        str += ch;
        ch = get();
    }
    put(ch);
    return {str, "__HEXNUMBER__", "__LITERAL__", line, column};
}

epplex::Token epplex::Lexer::Oct() {
    char ch = get();
    std::string str;
    while ((! input.eof()) && (ch >= '0' && ch <= '7')) {
        str += ch;
        ch = get();
    }
    put(ch);
    return {str, "__OCTNUMBER__", "__LITERAL__", line, column};
}

epplex::Token epplex::Lexer::OctHex() {
    char ch = get();
    if (ch == 'x') {
        epplex::Token t = Hex();
        std::string content;
        content += ch;
        return {content + t.content, t.detail_type, "__LITERAL__", line, column};
    }
    else {
        put(ch);
        return Oct();
    }
}

void epplex::Lexer::Comment() {
    char ch = get();
    while (ch != '\n' && ch != '\r' && (! input.eof())) {
        ch = get();
    }
}

epplex::Token epplex::Lexer::Sign() { //符号
    char ch = get();
    std::string str;
    str += ch;
    if (ch == '=') {
        ch = get();
        if (ch == '=')
            str += ch;
        else if (ch == '>')
            str += ch;
        else
            put(ch);
    }
    if (ch == '!') { //以=, !开头，后可接=
        ch = get();
        if (ch == '=')
            str += ch;
        else
            put(ch);
    }
    else if (ch == '&') { //以&开头，后可接&
        ch = get();
        if (ch == '&')
            str += ch;
        else
            put(ch);
    }
    else if (ch == '|') { //以|开头，后可接|
        ch = get();
        if (ch == '|')
            str += ch;
        else
            put(ch);
    }
    else if (ch == '>') {//以>开头，后可接=,>
        ch = get();
        if (ch == '=' || ch == '>')
            str += ch;
        else
            put(ch);
    }
    else if(ch == '<') {//以<开头，后可接=,<
        ch = get();
        if (ch == '=' || ch == '<')
            str += ch;
        else
            put(ch);
    }

    if (str == "=") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == ">") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "<") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "!") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "&") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "|") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "==") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "=>") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == ">=") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "<=") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "!=") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "&&") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "||") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == ">>") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    else if (str == "<<") return {str, "__SYMBOL__", "__SYMBOL__", line, column};
    return {};
}

epplex::Token epplex::Lexer::Identifier() {
    char ch = get();
    std::string str ;
    while (((! input.eof()) && (isalnum(ch) || ch == '_'))) {
        str += ch;
        ch = get();
    }
    put(ch);
    std::vector keymap = {"out", "var", "const", "typeof", "input", "delete", "area", "act", "true", "false", "if"};
    for(auto iden : keymap)
        if (str == iden) {return {str, "__KEYWORD__", "__IDENTIFIER__", line, column};}
    return {str, "__IDENTIFIER__", "__IDENTIFIER__", line, column};
}

epplex::Token epplex::Lexer::String(){
    char sign = get();
    char ch=  get();
    std::string str;
    if(sign == '"'){
        while (ch != '"'){
            if(input.eof()) if(str.size() > 1) throw epperr::Epperr("StringError", "Expect '\"'", line, column);
            str += ch;
            ch = get();
        }
        return {str, "__STRING__", "__LITERAL__",line, column};
    }
    else if(sign == '\''){
        while (ch != '\''){
            if(input.eof()) if(str.size() > 1) throw epperr::Epperr("StringError", "Expect '\''", line, column);
            str += ch;
            ch = get();
        }
        if(str.size() > 1) throw epperr::Epperr("TypeError", "It is too long as a 'char' type of data", line, column);
        return {str, "__CHAR__", "__LITERAL__",line, column};
    }
    return {"__NULLTOKEN__", "__NULLTOKEN__", 0, 0};
}

epplex::Token epplex::Lexer::Start() {
    if (input.peek() == EOF) { //以eof开头, 单词为eof
        return {"eof", "__EOF__", "__LEXER__",line, column};
    }
    char ch = get();
    if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t');
    else if (std::isdigit(ch)) { //以0-9开头，必定为10进制数
        put(ch);
        return Num();
    }
    else if (ch == '\"' || ch == '\''){
        put(ch);
        return String();
    }
    else if (ch == '#') { //以/开头，也许是注释
        ch = get();
        Comment();
        return {};
    }
    else if (isalpha(ch) || ch == '_') { //以字母或_开头,标识符
        put(ch);
        return Identifier();
    }
    else {
        switch (ch) {
        case '+':
            return {"+", "__SYMBOL__", "__SYMBOL__", line, column};
        case '-':
            return {"-", "__SYMBOL__", "__SYMBOL__", line, column};
        case '*':
            return {"*", "__SYMBOL__", "__SYMBOL__", line, column};
        case '/':
            return {"/", "__SYMBOL__", "__SYMBOL__", line, column};
        case '%':
            return {"%", "__SYMBOL__", "__SYMBOL__", line, column};
        case '{':
            return {"{", "__SYMBOL__", "__SYMBOL__", line, column};
        case '}':
            return {"}", "__SYMBOL__", "__SYMBOL__", line, column};
        case '[':
            return {"[", "__SYMBOL__", "__SYMBOL__", line, column};
        case ']':
            return {"]", "__SYMBOL__", "__SYMBOL__", line, column};
        case '(':
            return {"(", "__SYMBOL__", "__SYMBOL__", line, column};
        case ')':
            return {")", "__SYMBOL__", "__SYMBOL__", line, column};
        case '^':
            return {"^", "__SYMBOL__", "__SYMBOL__", line, column};
        case '$':
            return {"$", "__SYMBOL__", "__SYMBOL__", line, column};
        case ':':
            return {":", "__SYMBOL__", "__SYMBOL__", line, column}; //符号
        case ';':
            return {";", "__SYMBOL__", "__SYMBOL__", line, column};
        case ',':
            return {",", "__SYMBOL__", "__SYMBOL__", line, column};
        case '.':
            return {".", "__SYMBOL__", "__SYMBOL__", line, column};
        case '=':
        case '>':
        case '<':
        case '!':
        case '&':
        case '|':
            put(ch); //以= > < ! & |开头，有可能继续接着
            return Sign();
        }
    }
    return {"__NULLTOLEN__", "__NULLTOKEN__", "__NULLTOKEN__",0, 0};
}

epplex::Lexer::Lexer(std::istream& input) : input(input), line(1), column(0) {}

char epplex::Lexer::get(){
    char ch = input.get();
    if (ch == '\n' || ch == '\r')
        line ++, column = 1;
    else column ++;
    return ch;
}

void epplex::Lexer::put(char ch) {
    if (ch == '\n' || ch == '\r')
        line --, column = 1;
    else column --;
    input.putback(ch);
}

std::vector<epplex::Token> epplex::Lexer::getTokenGroup(){
    std::vector<epplex::Token> tg;
    std::vector<epplex::Token> rtg;
    while (true) {
        Token t = Start(); //Start of token
        if (t.type == "__EOF__" || input.eof())
            break;
        if (t.type !="__NULLTOKEN__")
            tg.push_back(t);
    }
    for(auto tok : tg){
        if(tok.type == "__NULLTOKEN__");
        else rtg.push_back(tok);
    }
    rtg.push_back({"__EOF__", "__EOF__", "__LEXER__", 0, 0});
    return rtg;
}
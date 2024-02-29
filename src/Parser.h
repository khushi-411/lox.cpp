#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "Expr.h"
#include "Token.h"
#include "TokenType.h"

namespace lox {

class Parser {

public:
    const std::vector<Token> tokens;
    int current = 0;

    Parser(std::vector<Token> tokens);

    Expr parse();

    Expr expression();
    Expr equality();

    template <typename T, typename... TokenType>
    bool match(TokenType... types);

    Token consume(TokenType type, std::string message);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    ParseError error(Token token, std::string message);
    Expr comparision();
    Expr term();
    Expr factor();
    Expr unary();
    Expr primary();
    void synchronize();
};

class ParseError : class Parser {

};

}  // namespace lox

#endif

#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "Expr.h"
#include "Token.h"
#include "TokenType.h"

namespace lox {

template <class T>
class Parser {

public:
    const std::vector<Token> tokens;
    int current = 0;

    Parser(std::vector<Token> tokens);

    lox::expr::Expr parse();

    lox::expr::Expr expression();
    lox::expr::Expr equality();

    template <typename T, typename... TokenType>
    bool match(TokenType... types);

    Token consume(TokenType type, std::string message);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    //ParseError error(Token token, std::string message);
    lox::expr::Expr comparision();
    lox::expr::Expr term();
    lox::expr::Expr factor();
    lox::expr::Expr unary();
    lox::expr::Expr primary();
    void synchronize();
};

class ParseError : public Parser {

};

}  // namespace lox

#endif

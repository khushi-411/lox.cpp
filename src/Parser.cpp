#include <vector>

#include "Expr.h"
#include "Parser.h"
#include "TokenType.h"


namespace lox {

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}


lox::expr::Expr parse() {
    try {
        return Parser::expression();
    } catch (ParseError error) {
        return NULL;
    }
}

lox::expr::Expr Parser::expression() {
    return Parser::equality();
}


lox::expr::Expr Parser::equality() {
    lox::expr::Expr expr = Parser::comparison();
    while (Parser::match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token op = Parser::previous();
        lox::expr::Expr right = Parser::comparison();
        expr = Expr::Binary(expr, op, right);
    }
    return expr;
}


bool Parser::match(TokenType... types) {
    for (TokenType type : types) {
        if (Parser::check(type)) {
            Parser::advance();
            return true;
        }
    }
    return false;
}


bool Parser::check(TokenType type) {
    if(Parser::isAtEnd()) {
        return false;
    }
    return Parser::peek().type == type;
}


Token Parser::advance() {
    if (!Parser::isAtEnd()) {
        current++;
    }
    return Parser::previous();
}


bool Parser::isAtEnd() {
    return Parser::peek().type == TokenType::_EOF;
}


Token Parser::peek() {
    return tokens.get(current);
}


Token Parser::previous() {
    return tokens.get(current - 1);
}


lox::expr::Expr Parser::comparison() {
    lox::expr expr = Parser::term();

    while(Parser::match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
        Token op = Parser::previous();
        lox::expr::expr right = Parser::term();
        expr = lox::expr::Binary(expr, op, right);
    }

    return expr;
}


lox::expr::Expr Parser::term() {
    lox::expr expr = Parser::factor();

    while(Parser::match(TokenType::MINUS, TokenType::PLUS)) {
        Token op = Parser::previous();
        Expr right = Parser::factor();
        expr = Expr::Binary(expr, op, right);
    }

    return expr;
}


lox::expr::Expr Parser::factor() {
    lox::expr expr = Parser::unary();

    while(Parser::match(TokenType::SLASH, TokenType::STAR)) {
        Token op = Parser::previous();
        Expr right = Parser::unary();
        expr = Expr::Binary(expr, op, right);
    }

    return expr;
}


lox::expr::Expr Parser::unary() {
    if (Parser::match(TokenType::BANG, TokenType::MINUS)) {
        Token op = Parser::previous();
        Expr right = Parser::unary();
        return Expr::Unary(op, right);
    }

    return Parser::primary();
}


lox::expr::Expr Parser::primary() {
    if (Parser::match(TokenType::FALSE)) {
        return Expr::Literal(false);
    }
    if (Parser::match(TokenType::TRUE)) {
        return Expr::Literal(true);
    }
    if (Parser::match(TokenType::NIL)) {
        return Expr::Literal(NULL);
    }

    if (Parser::match(TokenType::NUMBER, TokenType::STRING)) {
        return Expr::Literal(Parser::previous().literal());
    }

    if (Parser::match(TokenType::LEFT_PAREN)) {
        Expr expr = Parser::expression();
        Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return Expr::Grouping(expr);
    }

    throw Parser::error(Parser::peek(), "Expect expression.");
}


Token Parser::consume(TokenType type, std::string message) {
    if (Parser::check(type)) {
        return Parser::advance();
    }
    throw Parser::error(Parser::peek(), message);
}


ParseError error(Token token, std::string message) {
    Lox::error(token, message);
    return ParseError();
}


void Parser::synchronize() {
    Parser::advance();

    while(!Parser::isAtEnd()) {
        if (Parser::previous().type == TokenType::SEMICOLON) {
            return;
        }
        switch (Parser::peek().type) {
            case TokenType::CLASS:
            case TokenType::FOR:
            case TokenTyoe::FUN:
            case TokenType::IF:
            case TokenType::PRINT:
            case TokenType::RETURN:
            case TokenType::Var:
            case TokenType::WHILE:
                return;
        }
        Parser::advance();
    }
}

}  // namespace lox

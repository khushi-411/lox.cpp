#include <iostream>
#include <string.h>

#include "Lox.h"
#include "Token.h"

namespace lox {

Token::Token(TokenType type, std::string lexeme, std::string literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

std::string Token::toString() {
    return type + " " + lexeme + " " + literal;
}

}  // namespace lox


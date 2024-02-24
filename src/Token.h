#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <cstddef>

#include "TokenType.h"

namespace lox {

class Token {

private:
    // TODO: Check if there's any other way
    friend std::ostream& operator<<(std::ostream& os, const Token& _x) {
        os << _x;
        return os;
    }

public:
    // https://stackoverflow.com/questions/4971286
    // TODO: const or its alternative
    TokenType type;
    std::string lexeme;
    // https://stackoverflow.com/questions/4233123
    std::string literal;  // todo
    int line;

    Token(TokenType type, std::string lexeme, std::string literal, int line);
    std::string toString();

};

}  // namespace lox

#endif

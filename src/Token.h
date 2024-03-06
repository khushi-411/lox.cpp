#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <cstddef>
#include <variant>

#include "TokenType.h"

namespace lox {

class Token {

public:
    // https://stackoverflow.com/questions/4971286
    // TODO: const or its alternative
    const TokenType type;
    const std::string lexeme;
    // https://stackoverflow.com/questions/4233123
    const std::variant<std::nullptr_t, std::string, double, bool> literal;
    const int line;

    Token(const TokenType &type, const std::string &lexeme, decltype(literal) literal, const int &line);
    const std::string to_string() const;  // TODO

};


// https://stackoverflow.com/questions/1801247
inline std::ostream& operator<<(std::ostream& os, const Token& _x) {
    // TODO: Check if there's any other way
    os << _x.to_string();
    return os;
}

}  // namespace lox

#endif

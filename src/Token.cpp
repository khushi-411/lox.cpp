#include <iostream>
#include <string.h>
#include <variant>

#include "Token.h"

namespace lox {

// check the difference between assignment method and initialization list way
// We have declared all the parameters as constant, assigning it here, is contradictory,
// therefore we'll have to use initialization list
// TODO: why std::move
Token::Token(
        const TokenType &type,
        const std::string &lexeme,
        std::variant<std::nullptr_t, std::string, double, bool> literal,
        const int &line)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}


const std::string Token::to_string() const {
    return type + " " + lexeme + " " + std::get<std::string>(literal);
}

}  // namespace lox

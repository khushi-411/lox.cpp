#include <iostream>
#include <string>
#include <variant>

#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

// check the difference between assignment method and initialization list way
// We have declared all the parameters as constant, assigning it here, is
// contradictory, therefore we'll have to use initialization list
// TODO: why std::move
Token::Token(
    const TokenType& type,
    const std::string& lexeme,
    const Object& literal,
    const int& line)
    : type(type),
      lexeme(lexeme),
      literal(literal),
      line(line) {}


std::string Token::to_string() const {
  std::string lit;
  if (std::holds_alternative<std::string>(literal)) {
    lit = std::get<std::string>(literal);
  } else if (std::holds_alternative<double>(literal)) {
    lit = std::to_string(std::get<double>(literal));
  } else if (std::holds_alternative<bool>(literal)) {
    lit = std::get<bool>(literal) ? "true" : "false";
  } else {
    lit = "nil";
  }
  return std::to_string(static_cast<int>(type)) + " " + lexeme + " " + lit;
}


const TokenType& Token::tokentype() const {
  return type;
}


const std::string& Token::getLexeme() const {
  return lexeme;
}


const Object& Token::getLiteral() const {
  return literal;
}


const int& Token::getLine() const {
  return line;
}


}  // namespace lox

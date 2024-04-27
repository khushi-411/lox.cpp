#include <iostream>
#include <sstream>
#include <string>
#include <variant>

#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

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
      lexeme(std::move(lexeme)),
      literal(std::move(literal)),
      line(line) {}


const std::string& Token::to_string() const {
  std::stringstream ss;

  if (std::holds_alternative<std::nullptr_t>(literal)) {
    ss << type << " " << lexeme << " " << std::get<std::nullptr_t>(literal);

  } else if (std::holds_alternative<std::string>(literal)) {
    ss << type << " " << lexeme << " " << std::get<std::string>(literal);

  } else if (std::holds_alternative<double>(literal)) {
    ss << type << " " << lexeme << " " << std::get<double>(literal);

  } else if (std::holds_alternative<bool>(literal)) {
    ss << type << " " << lexeme << " " << std::get<bool>(literal);
  }

  static std::string result;
  result = ss.str();
  return result;
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

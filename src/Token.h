#ifndef TOKEN_H
#define TOKEN_H

#include <cstddef>
#include <string>
#include <variant>

#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

class Token {
 private:
  // https://stackoverflow.com/questions/4971286
  TokenType type;
  std::string lexeme;
  // https://stackoverflow.com/questions/4233123
  Object literal;
  int line;

 public:
  Token(
      const TokenType& type,
      const std::string& lexeme,
      const decltype(literal)& literal,  // TODO: decltype type deduction
      const int& line);

  const std::string& to_string() const;
  const TokenType& tokentype() const;
  const std::string& getLexeme() const;
  const Object& getLiteral() const;
  const int& getLine() const;
};


// https://stackoverflow.com/questions/1801247
inline std::ostream& operator<<(std::ostream& os, const Token& _x) {
  // TODO: Check if there's any other way
  os << _x.to_string();
  return os;
}

}  // namespace lox

#endif

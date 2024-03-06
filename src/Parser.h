#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include <string>
#include <vector>

#include "Expr.h"
#include "Token.h"
#include "TokenType.h"

namespace lox {

class ParseError : public std::runtime_error {};


template <class T>
class Parser {
 public:
  const std::vector<Token> tokens;
  int current = 0;

  Parser(const std::vector<Token>& tokens);

  lox::expr::Expr parse();

  lox::expr::Expr expression();
  lox::expr::Expr equality();

  template <typename T, typename... TokenType>
  bool match(const TokenType...& types);

  Token consume(const TokenType& type, const std::string& message);
  bool check(const TokenType& type);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  ParseError error(const Token& token, const std::string& message);
  lox::expr::Expr comparision();
  lox::expr::Expr term();
  lox::expr::Expr factor();
  lox::expr::Expr unary();
  lox::expr::Expr primary();
  void synchronize();
};

}  // namespace lox

#endif

#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include <string>
#include <vector>

#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"


namespace lox {

namespace parser {

inline std::vector<Token> tokens;
inline int current = 0;


class ParseError : public std::runtime_error {
 public:
  ParseError(const Token& token, const std::string& message)
      : std::runtime_error(message), token(token) {}

  const Token& token;
  ParseError error(const Token& token, const std::string& message);
};


template <class T>
class Parser {
 public:
  Parser(const std::vector<Token>& tokens);

  std::vector<lox::stmt::Stmt<T>> parseStmt();
  lox::stmt::Stmt<T> statement();
  lox::stmt::Stmt<T> printStatement();
  lox::stmt::Stmt<T> expressionStatement();
  lox::stmt::Stmt<T> declaration();
  lox::stmt::Stmt<T> classDeclaration();
  lox::stmt::Stmt<T> forStatement();
  lox::stmt::Stmt<T> ifStatement();
  lox::stmt::Stmt<T> returnStatement();
  lox::stmt::Stmt<T> varDeclaration();
  lox::stmt::Stmt<T> whileStatement();
  lox::stmt::Stmt<T> function(const std::string& kind);
  std::vector<lox::stmt::Stmt<T>> block();

  lox::expr::Expr<T> parse();
  lox::expr::Expr<T> assignment();
  lox::expr::Expr<T> _or();
  lox::expr::Expr<T> _and();
  lox::expr::Expr<T> expression();
  lox::expr::Expr<T> equality();
  bool match(const TokenType& types);
  Token consume(const TokenType& type, const std::string& message);
  bool check(const TokenType& type);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  lox::expr::Expr<T> comparison();
  lox::expr::Expr<T> term();
  lox::expr::Expr<T> factor();
  lox::expr::Expr<T> unary();
  lox::expr::Expr<T> finishCall(const lox::expr::Expr<T>& callee);
  lox::expr::Expr<T> call();
  lox::expr::Expr<T> primary();
  void synchronize();
};


}  // namespace parser

}  // namespace lox

#endif

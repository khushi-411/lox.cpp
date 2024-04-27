#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "Expr.h"
#include "Lox.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"


namespace lox {

namespace parser {


class ParseError : public std::runtime_error {
 public:
  ParseError(const Token& token, const std::string& message)
      : std::runtime_error(message), token(token) {}

  const Token& token;
  ParseError error(const Token& token, const std::string& message);
};


class Parser {
 private:
  std::vector<Token> tokens;
  int current = 0;

 public:
  Parser() {}
  Parser(const std::vector<Token>& tokens);

  std::vector<lox::stmt::Stmt> parseStmt();
  lox::stmt::Stmt statement();
  lox::stmt::Stmt printStatement();
  lox::stmt::Stmt expressionStatement();
  // lox::stmt::Stmt declaration();
  // lox::stmt::Stmt classDeclaration();
  lox::stmt::Stmt forStatement();
  lox::stmt::Stmt ifStatement();
  lox::stmt::Stmt returnStatement();
  lox::stmt::Stmt varDeclaration();
  lox::stmt::Stmt whileStatement();
  // lox::stmt::Stmt function(const std::string& kind);
  std::vector<lox::stmt::Stmt> block();

  lox::expr::Expr parse();
  // lox::expr::Expr assignment();
  lox::expr::Expr _or();
  lox::expr::Expr _and();
  lox::expr::Expr expression();
  lox::expr::Expr equality();
  bool match(const TokenType& types, ...);
  Token consume(const TokenType& type, const std::string& message);
  bool check(const TokenType& type);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  lox::expr::Expr comparison();
  lox::expr::Expr term();
  lox::expr::Expr factor();
  lox::expr::Expr unary();
  //  lox::expr::Expr finishCall(const lox::expr::Expr& callee);
  //  lox::expr::Expr call();
  lox::expr::Expr primary();
  void synchronize();
};


}  // namespace parser

}  // namespace lox

// template class lox::parser::Parser<double>;

#endif

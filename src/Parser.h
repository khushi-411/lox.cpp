#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <initializer_list>
#include <memory>
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

  Token token;
};


class Parser {
 private:
  std::vector<Token> tokens;
  int current = 0;

 public:
  Parser() {}
  Parser(const std::vector<Token>& tokens);

  std::vector<std::shared_ptr<lox::stmt::Stmt>> parse();

  std::shared_ptr<lox::stmt::Stmt> declaration();
  std::shared_ptr<lox::stmt::Stmt> classDeclaration();
  std::shared_ptr<lox::stmt::Stmt> statement();
  std::shared_ptr<lox::stmt::Stmt> printStatement();
  std::shared_ptr<lox::stmt::Stmt> expressionStatement();
  std::shared_ptr<lox::stmt::Stmt> forStatement();
  std::shared_ptr<lox::stmt::Stmt> ifStatement();
  std::shared_ptr<lox::stmt::Stmt> returnStatement();
  std::shared_ptr<lox::stmt::Stmt> varDeclaration();
  std::shared_ptr<lox::stmt::Stmt> whileStatement();
  std::shared_ptr<lox::stmt::Function> function(const std::string& kind);
  std::vector<std::shared_ptr<lox::stmt::Stmt>> block();

  std::shared_ptr<lox::expr::Expr> expression();
  std::shared_ptr<lox::expr::Expr> assignment();
  std::shared_ptr<lox::expr::Expr> _or();
  std::shared_ptr<lox::expr::Expr> _and();
  std::shared_ptr<lox::expr::Expr> equality();
  bool match(std::initializer_list<TokenType> types);
  Token consume(const TokenType& type, const std::string& message);
  bool check(const TokenType& type);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  std::shared_ptr<lox::expr::Expr> comparison();
  std::shared_ptr<lox::expr::Expr> term();
  std::shared_ptr<lox::expr::Expr> factor();
  std::shared_ptr<lox::expr::Expr> unary();
  std::shared_ptr<lox::expr::Expr> finishCall(
      std::shared_ptr<lox::expr::Expr> callee);
  std::shared_ptr<lox::expr::Expr> call();
  std::shared_ptr<lox::expr::Expr> primary();
  ParseError error(const Token& token, const std::string& message);
  void synchronize();
};


}  // namespace parser

}  // namespace lox

// template class lox::parser::Parser<double>;

#endif

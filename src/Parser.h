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


template <class T>
class Parser {
 private:
  std::vector<Token> tokens;
  int current = 0;

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
  bool match(const TokenType& types, ...);
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


// input: sequence of tokens

template <class T>
Parser<T>::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}


template <class T>
std::vector<lox::stmt::Stmt<T>> Parser<T>::parseStmt() {
  std::vector<lox::stmt::Stmt<T>> statements;

  while (!Parser<T>::isAtEnd()) {
    statements.push_back(Parser<T>::statement());
  }
  return statements;
}


template <class T>
lox::stmt::Stmt<T> Parser<T>::statement() {
  if (Parser<T>::match(TokenType::FOR)) {
    return Parser<T>::forStatement();
  }

  if (Parser<T>::match(TokenType::IF)) {
    return Parser<T>::ifStatement();
  }

  if (Parser<T>::match(TokenType::PRINT)) {
    return Parser<T>::printStatement();
  }

  if (Parser<T>::match(TokenType::RETURN)) {
    return Parser<T>::returnStatement();
  }

  if (Parser<T>::match(TokenType::WHILE)) {
    return Parser<T>::whileStatement();
  }

  if (Parser<T>::match(TokenType::LEFT_BRACE)) {
    return lox::stmt::Block<T>(Parser<T>::block());
  }

  return Parser<T>::expressionStatement();
}


// for stmt

template <class T>
lox::stmt::Stmt<T> Parser<T>::forStatement() {
  Parser<T>::consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

  lox::stmt::Stmt<T> initializer;
  if (Parser<T>::match(TokenType::SEMICOLON)) {
    initializer = nullptr;

  } else if (Parser<T>::match(TokenType::VAR)) {
    initializer = Parser<T>::varDeclaration();

  } else {
    initializer = Parser<T>::expressionStatement();
  }

  lox::expr::Expr<T> condition = nullptr;
  if (!Parser<T>::match(TokenType::SEMICOLON)) {
    condition = Parser<T>::expression();
  }

  Parser<T>::consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

  lox::expr::Expr<T> increment = nullptr;
  if (!Parser<T>::check(TokenType::RIGHT_PAREN)) {
    increment = Parser<T>::expression();
  }

  Parser<T>::consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

  lox::stmt::Stmt<T> body = Parser<T>::statement();

  if (increment != nullptr) {
    std::vector<lox::stmt::Stmt<T>> _stmt;
    _stmt.push_back(lox::stmt::Expression<T>(increment));
    body = lox::stmt::Block<T>(_stmt);
  }

  if (condition == nullptr) {
    condition = lox::expr::Literal<T>(true);
  }

  body = lox::stmt::While<T>(condition, body);

  if (initializer != nullptr) {
    std::vector<lox::stmt::Stmt<T>> _stmt = body;
    body = lox::stmt::Block<T>(_stmt);
  }

  return body;
}


// if stmt

template <class T>
lox::stmt::Stmt<T> Parser<T>::ifStatement() {
  Parser<T>::consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  lox::expr::Expr<T> condition = Parser<T>::expression();

  Parser<T>::consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
  lox::stmt::Stmt<T> thenBranch = Parser<T>::statement();

  lox::stmt::Stmt<T> elseBranch = nullptr;

  if (Parser<T>::match(TokenType::ELSE)) {
    elseBranch = Parser<T>::statement();
  }

  return lox::stmt::If<T>(condition, thenBranch, elseBranch);
}


// print stmt

template <class T>
lox::stmt::Stmt<T> Parser<T>::printStatement() {
  lox::expr::Expr<T> value = Parser<T>::expression();
  Parser<T>::consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return new lox::stmt::Print<T>(value);
}


// return stmt

template <class T>
lox::stmt::Stmt<T> Parser<T>::returnStatement() {
  Token keyword = Parser<T>::previous();
  lox::expr::Expr<T> value = nullptr;

  if (!Parser<T>::check(TokenType::SEMICOLON)) {
    value = Parser<T>::expression();
  }

  Parser<T>::consume(TokenType::SEMICOLON, "Expect ';' after return value.");

  return lox::stmt::Return<T>(keyword, value);
}


// var declaration

template <class T>
lox::stmt::Stmt<T> Parser<T>::varDeclaration() {
  Token name =
      Parser<T>::consume(TokenType::IDENTIFIER, "Expect variable name.");

  lox::expr::Expr<T> initializer = nullptr;
  if (Parser<T>::match(TokenType::EQUAL)) {
    initializer = Parser<T>::expression();
  }

  Parser<T>::consume(
      TokenType::SEMICOLON, "Expect ';' after variable declaration.");

  return lox::stmt::Var<T>(name, initializer);
}


// while stmt

template <class T>
lox::stmt::Stmt<T> Parser<T>::whileStatement() {
  Parser<T>::consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  lox::expr::Expr<T> condition = Parser<T>::expression();

  Parser<T>::consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
  lox::stmt::Stmt<T> body = Parser<T>::statement();

  return lox::stmt::While<T>(condition, body);
}


// expression stmt

template <class T>
lox::stmt::Stmt<T> Parser<T>::expressionStatement() {
  lox::expr::Expr<T> _expr = Parser<T>::expression();
  Parser<T>::consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return new lox::stmt::Expression<T>(_expr);
}


template <class T>
lox::stmt::Function<T> function(const std::string& kind) {
  Token name =
      Parser<T>::consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

  Parser<T>::consume(
      TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");

  std::vector<Token> parameters;
  if (!Parser<T>::check(TokenType::RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255) {
        ParseError error(
            Parser<T>::peek(), "Can't have more than 255 parameters.");
      }

      parameters.push_back(
          Parser<T>::consume(TokenType::IDENTIFIER, "Expect parameter name."));
    } while (Parser<T>::match(TokenType::COMMA));
  }

  Parser<T>::consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
  Parser<T>::consume(
      TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
  std::vector<lox::stmt::Stmt<T>> body = Parser<T>::block();

  return lox::stmt::Function<T>(name, parameters, body);
}

template <class T>
std::vector<lox::stmt::Stmt<T>> Parser<T>::block() {
  std::vector<lox::stmt::Stmt<T>> statements;

  while (!Parser<T>::check(TokenType::RIGHT_BRACE) && !Parser<T>::isAtEnd()) {
    statements.push_back(Parser<T>::declaration());
  }

  Parser<T>::consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}


template <class T>
lox::stmt::Stmt<T> Parser<T>::declaration() {
  try {
    if (Parser<T>::match(TokenType::CLASS)) {
      return Parser<T>::classDeclaration();
    }

    if (Parser<T>::match(TokenType::FUN)) {
      return Parser<T>::function("function");
    }

    if (Parser<T>::match(TokenType::VAR)) {
      return Parser<T>::varDeclaration();
    }

    return Parser<T>::statement();

  } catch (ParseError error) {
    Parser<T>::synchronize();
    return nullptr;
  }
}


template <class T>
lox::stmt::Stmt<T> Parser<T>::classDeclaration() {
  Token name = Parser<T>::consume(TokenType::IDENTIFIER, "Expect class name.");

  lox::expr::Variable<T> superclass = nullptr;
  if (Parser<T>::match(TokenType::LESS)) {
    Parser<T>::consume(TokenType::IDENTIFIER, "Expect superclass name.");
    superclass = lox::expr::Variable<T>(Parser<T>::previous());
  }

  Parser<T>::consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

  std::vector<lox::stmt::Function<T>> methods;
  while (!Parser<T>::check(TokenType::RIGHT_BRACE) && !Parser<T>::isAtEnd()) {
    methods.push_back(Parser<T>::function("method"));
  }

  Parser<T>::consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

  return lox::stmt::Class<T>(name, superclass, methods);
}


// below are the rules, converting themselves to the tree structure

template <class T>
lox::expr::Expr<T> Parser<T>::parse() {
  try {
    return Parser<T>::expression();
  } catch (ParseError error) {
    // returns NULL because we want to take this to the interpreter
    return lox::expr::Expr<T>();
  }
}


template <class T>
lox::expr::Expr<T> Parser<T>::expression() {
  // for parsing expressions
  return Parser<T>::equality();
}


template <class T>
lox::expr::Expr<T> Parser<T>::equality() {
  lox::expr::Expr<T> expr = Parser<T>::comparison();

  while (Parser<T>::match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::comparison();
    expr = lox::expr::Binary<T>(expr, op, right);
  }

  return expr;
}


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::assignment() {
  lox::expr::Expr<T> _expr = Parser<T>::_or();

  if (Parser<T>::match(TokenType::EQUAL)) {
    Token equals = Parser<T>::previous();
    lox::expr::Expr<T> value = Parser<T>::assignment();

    if (instanceof <lox::expr::Variable<T>>(_expr)) {
      Token name = (reinterpret_cast<lox::expr::Variable<T>>(_expr)).name;
      return lox::expr::Assign<T>(name, value);

    } else if (instanceof <lox::expr::Get<T>>(_expr)) {
      lox::expr::Get<T> get =
          reinterpret_cast<lox::expr::Get<T>>(_expr);  // TODO
      return lox::expr::Set<T>(get.object, get.name, value);
    }

    ParseError error(equals, "Invalid assignment target.");
  }

  return _expr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::_or() {
  lox::expr::Expr<T> _expr = Parser<T>::_and();

  while (Parser<T>::match(TokenType::OR)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::_and();
    _expr = lox::expr::Logical<T>(_expr, op, right);  // TODO: new?
  }

  return _expr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::_and() {
  lox::expr::Expr<T> _expr = Parser<T>::equality();

  while (Parser<T>::match(TokenType::AND)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::equality();
    _expr = lox::expr::Logical<T>(_expr, op, right);
  }

  return _expr;
}


template <class T>
bool Parser<T>::match(const TokenType& types, ...) {
  va_list args;
  va_start(args, types);

  for (TokenType type = types; type != TokenType::_EOF;
       type = va_arg(args, TokenType)) {
    if (Parser<T>::check(type)) {
      Parser<T>::advance();
      va_end(args);
      return true;
    }
  }

  va_end(args);
  return false;
}


template <class T>
bool Parser<T>::check(const TokenType& type) {
  if (Parser<T>::isAtEnd()) {
    return false;
  }
  return Parser<T>::peek().tokentype() == type;
}


template <class T>
Token Parser<T>::advance() {
  if (!Parser<T>::isAtEnd()) {
    current++;
  }
  return Parser<T>::previous();
}


template <class T>
bool Parser<T>::isAtEnd() {
  return Parser<T>::peek().tokentype() == TokenType::_EOF;
}


template <class T>
Token Parser<T>::peek() {
  return tokens[current];
}


template <class T>
Token Parser<T>::previous() {
  return tokens[current - 1];
}


template <class T>
lox::expr::Expr<T> Parser<T>::comparison() {
  lox::expr::Expr<T> expr = Parser<T>::term();

  while (Parser<T>::match(
      TokenType::GREATER,
      TokenType::GREATER_EQUAL,
      TokenType::LESS,
      TokenType::LESS_EQUAL)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::term();
    expr = lox::expr::Binary<T>(expr, op, right);
  }

  return expr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::term() {
  lox::expr::Expr<T> expr = Parser<T>::factor();

  while (Parser<T>::match(TokenType::MINUS, TokenType::PLUS)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::factor();
    expr = lox::expr::Binary<T>(expr, op, right);
  }

  return expr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::factor() {
  lox::expr::Expr<T> expr = Parser<T>::unary();

  while (Parser<T>::match(TokenType::SLASH, TokenType::STAR)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::unary();
    expr = lox::expr::Binary<T>(expr, op, right);
  }

  return expr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::unary() {
  if (Parser<T>::match(TokenType::BANG, TokenType::MINUS)) {
    Token op = Parser<T>::previous();
    lox::expr::Expr<T> right = Parser<T>::unary();
    return lox::expr::Unary<T>(op, right);
  }

  // return Parser<T>::primary();
  return Parser<T>::call();
}


template <class T>
lox::expr::Expr<T> Parser<T>::finishCall(const lox::expr::Expr<T>& callee) {
  std::vector<lox::expr::Expr<T>> arguments;

  if (!Parser<T>::check(TokenType::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        ParseError error(
            Parser<T>::peek(), "Can't have more than 255 arguments.");
      }

      arguments.push_back(Parser<T>::expression());
    } while (Parser<T>::match(TokenType::COMMA));
  }

  Token paren =
      Parser<T>::consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

  return lox::expr::Call<T>(callee, paren, arguments);
}


template <class T>
lox::expr::Expr<T> Parser<T>::call() {
  lox::expr::Expr<T> _expr = Parser<T>::primary();

  while (true) {
    if (Parser<T>::match(TokenType::LEFT_PAREN)) {
      _expr = Parser<T>::finishCall(_expr);

    } else if (Parser<T>::match(TokenType::DOT)) {
      Token name = Parser<T>::consume(
          TokenType::IDENTIFIER, "Expect property name after '.'.");
      _expr = lox::expr::Get<T>(_expr, name);
    } else {
      break;
    }
  }
  return _expr;
}


template <class T>
lox::expr::Expr<T> Parser<T>::primary() {
  if (Parser<T>::match(TokenType::FALSE)) {
    return lox::expr::Literal<T>(false);
  }

  if (Parser<T>::match(TokenType::TRUE)) {
    return lox::expr::Literal<T>(true);
  }

  if (Parser<T>::match(TokenType::NIL)) {
    return lox::expr::Literal<T>(nullptr);
  }

  if (Parser<T>::match(TokenType::NUMBER, TokenType::STRING)) {
    return lox::expr::Literal<T>(Parser<T>::previous().getLiteral());
  }

  if (Parser<T>::match(TokenType::SUPER)) {
    Token keyword = Parser<T>::previous();
    Parser<T>::consume(TokenType::DOT, "Expect '.' after 'super'.");
    Token method = Parser<T>::consume(
        TokenType::IDENTIFIER, "Expect superclass method name.");
    // don't use "new" keyword
    return lox::expr::Super<T>(keyword, method);
  }

  if (Parser<T>::match(TokenType::THIS)) {
    return lox::expr::This<T>(Parser<T>::previous());
  }

  if (Parser<T>::match(TokenType::IDENTIFIER)) {
    return lox::expr::Variable<T>(Parser<T>::previous());
  }

  if (Parser<T>::match(TokenType::LEFT_PAREN)) {
    lox::expr::Expr<T> expr = Parser<T>::expression();
    Parser<T>::consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return lox::expr::Grouping<T>(expr);
  }

  throw ParseError(Parser<T>::peek(), "Expect expression.");
}


// entering panic mode

template <class T>
Token Parser<T>::consume(const TokenType& type, const std::string& message) {
  if (Parser<T>::check(type)) {
    return Parser<T>::advance();
  }

  throw ParseError(Parser<T>::peek(), message);
}


template <class T>
lox::parser::ParseError error(const Token& token, const std::string& message) {
  // Lox<T>::error(token, message);
  return lox::parser::ParseError(token, message);
}


// synchronizing the recursive decent parser

template <class T>
void Parser<T>::synchronize() {
  Parser<T>::advance();

  while (!Parser<T>::isAtEnd()) {
    if (Parser<T>::previous().tokentype() == TokenType::SEMICOLON) {
      return;
    }
    switch (Parser<T>::peek().tokentype()) {
      case CLASS:
      case FUN:
      case VAR:
      case FOR:
      case IF:
      case WHILE:
      case PRINT:
      case RETURN:
        return;
    }
    Parser<T>::advance();
  }
}


}  // namespace parser

}  // namespace lox

// template class lox::parser::Parser<double>;

#endif

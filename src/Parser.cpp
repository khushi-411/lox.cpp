#include <vector>

#include "Expr.h"
#include "Lox.h"
#include "Parser.h"
#include "Token.h"
#include "TokenType.h"


namespace lox {

namespace parser {

// input: sequence of tokens

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}


std::vector<lox::stmt::Stmt> Parser::parseStmt() {
  std::vector<lox::stmt::Stmt> statements;

  while (!Parser::isAtEnd()) {
    statements.push_back(Parser::statement());
  }
  return statements;
}


lox::stmt::Stmt Parser::statement() {
  if (Parser::match(TokenType::FOR)) {
    return Parser::forStatement();
  }

  if (Parser::match(TokenType::IF)) {
    return Parser::ifStatement();
  }

  if (Parser::match(TokenType::PRINT)) {
    return Parser::printStatement();
  }

  if (Parser::match(TokenType::RETURN)) {
    return Parser::returnStatement();
  }

  if (Parser::match(TokenType::WHILE)) {
    return Parser::whileStatement();
  }

  if (Parser::match(TokenType::LEFT_BRACE)) {
    return lox::stmt::Block(Parser::block());
  }

  return Parser::expressionStatement();
}


// for stmt

lox::stmt::Stmt Parser::forStatement() {
  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

  lox::stmt::Stmt initializer;
  if (Parser::match(TokenType::SEMICOLON)) {
    initializer = nullptr;

  } else if (Parser::match(TokenType::VAR)) {
    initializer = Parser::varDeclaration();

  } else {
    initializer = Parser::expressionStatement();
  }

  lox::expr::Expr condition;
  condition = nullptr;
  if (!Parser::match(TokenType::SEMICOLON)) {
    condition = Parser::expression();
  }

  Parser::consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

  lox::expr::Expr increment;
  increment = nullptr;
  if (!Parser::check(TokenType::RIGHT_PAREN)) {
    increment = Parser::expression();
  }

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

  lox::stmt::Stmt body = Parser::statement();

  if (increment != nullptr) {
    std::vector<lox::stmt::Stmt> _stmt;
    _stmt.push_back(lox::stmt::Expression(increment));
    body = lox::stmt::Block(_stmt);
  }

  if (condition == nullptr) {
    condition = lox::expr::Literal(true);
  }

  body = lox::stmt::While(condition, body);

  if (initializer != nullptr) {
    std::vector<lox::stmt::Stmt> _stmt;
    _stmt.push_back(body);
    body = lox::stmt::Block(_stmt);
  }

  return body;
}


// if stmt

lox::stmt::Stmt Parser::ifStatement() {
  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  lox::expr::Expr condition = Parser::expression();

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
  lox::stmt::Stmt thenBranch = Parser::statement();

  lox::stmt::Stmt elseBranch;
  elseBranch = nullptr;

  if (Parser::match(TokenType::ELSE)) {
    elseBranch = Parser::statement();
  }

  // TODO: remove this
  const lox::stmt::Stmt* _thenBranch = &thenBranch;
  const lox::stmt::Stmt* _elseBranch = &elseBranch;

  return lox::stmt::If(condition, _thenBranch, _elseBranch);
}


// print stmt

lox::stmt::Stmt Parser::printStatement() {
  lox::expr::Expr value = Parser::expression();
  Parser::consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return lox::stmt::Print(value);
}


// return stmt

lox::stmt::Stmt Parser::returnStatement() {
  Token keyword = Parser::previous();
  lox::expr::Expr value;
  value = nullptr;

  if (!Parser::check(TokenType::SEMICOLON)) {
    value = Parser::expression();
  }

  Parser::consume(TokenType::SEMICOLON, "Expect ';' after return value.");

  return lox::stmt::Return(keyword, value);
}


// var declaration

lox::stmt::Stmt Parser::varDeclaration() {
  Token name = Parser::consume(TokenType::IDENTIFIER, "Expect variable name.");

  lox::expr::Expr initializer;
  initializer = nullptr;
  if (Parser::match(TokenType::EQUAL)) {
    initializer = Parser::expression();
  }

  Parser::consume(
      TokenType::SEMICOLON, "Expect ';' after variable declaration.");

  return lox::stmt::Var(name, initializer);
}


// while stmt

lox::stmt::Stmt Parser::whileStatement() {
  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  lox::expr::Expr condition = Parser::expression();

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
  lox::stmt::Stmt body = Parser::statement();

  return lox::stmt::While(condition, body);
}


// expression stmt

lox::stmt::Stmt Parser::expressionStatement() {
  lox::expr::Expr _expr = Parser::expression();
  Parser::consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return lox::stmt::Expression(_expr);
}


lox::stmt::Function function(const std::string& kind) {
  Parser _parser;
  Token name =
      _parser.consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

  _parser.consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");

  std::vector<Token> parameters;
  if (!_parser.check(TokenType::RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255) {
        ParseError error(
            _parser.peek(), "Can't have more than 255 parameters.");
      }

      parameters.push_back(
          _parser.consume(TokenType::IDENTIFIER, "Expect parameter name."));
    } while (_parser.match(TokenType::COMMA));
  }

  _parser.consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
  _parser.consume(
      TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
  std::vector<lox::stmt::Stmt> body = _parser.block();

  return lox::stmt::Function(name, parameters, body);
}


std::vector<lox::stmt::Stmt> Parser::block() {
  std::vector<lox::stmt::Stmt> statements;

  while (!Parser::check(TokenType::RIGHT_BRACE) && !Parser::isAtEnd()) {
    // statements.push_back(Parser::declaration());
  }

  Parser::consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

/*
lox::stmt::Stmt Parser::declaration() {
  try {
    //if (Parser::match(TokenType::CLASS)) {
    //  return Parser::classDeclaration();
    //}

    if (Parser::match(TokenType::FUN)) {
      return Parser::function("function");
    }

    if (Parser::match(TokenType::VAR)) {
      return Parser::varDeclaration();
    }

    return Parser::statement();

  } catch (ParseError error) {
    Parser::synchronize();
    return lox::stmt::Stmt();
  }
}


lox::stmt::Stmt Parser::classDeclaration() {
  Token name = Parser::consume(TokenType::IDENTIFIER, "Expect class name.");

  lox::expr::Variable superclass(Token());
  if (Parser::match(TokenType::LESS)) {
    Parser::consume(TokenType::IDENTIFIER, "Expect superclass name.");
    superclass = lox::expr::Variable(Parser::previous());
  }

  Parser::consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

  std::vector<lox::stmt::Function> methods;
  while (!Parser::check(TokenType::RIGHT_BRACE) && !Parser::isAtEnd()) {
    methods.push_back(Parser::function("method"));
  }

  Parser::consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

  return lox::stmt::Class(name, superclass, methods);
}
*/

// below are the rules, converting themselves to the tree structure

lox::expr::Expr Parser::parse() {
  try {
    return Parser::expression();
  } catch (ParseError error) {
    // returns NULL because we want to take this to the interpreter
    return lox::expr::Expr();
  }
}


lox::expr::Expr Parser::expression() {
  // for parsing expressions
  return Parser::equality();
}


lox::expr::Expr Parser::equality() {
  lox::expr::Expr expr = Parser::comparison();

  while (Parser::match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::comparison();
    expr = lox::expr::Binary(expr, op, right);
  }

  return expr;
}


template <typename base, typename T>
bool instanceof (const T& ptr) {
  return dynamic_cast<const base*>(&ptr) != nullptr;
}

/*
lox::expr::Expr Parser::assignment() {
  lox::expr::Expr _expr = Parser::_or();

  if (Parser::match(TokenType::EQUAL)) {
    Token equals = Parser::previous();
    lox::expr::Expr value = Parser::assignment();

    if (instanceof <lox::expr::Variable>(_expr)) {
      lox::expr::Variable& variable = static_cast<lox::expr::Variable>(_expr);
      Token name = variable.getName();
      return lox::expr::Assign(name, value);

    } else if (instanceof <lox::expr::Get>(_expr)) {
      lox::expr::Get get = static_cast<lox::expr::Get>(_expr);
      return lox::expr::Set(get.getObject(), get.getName(), value);
    }

    ParseError error(equals, "Invalid assignment target.");
  }

  return _expr;
}
*/

lox::expr::Expr Parser::_or() {
  lox::expr::Expr _expr = Parser::_and();

  while (Parser::match(TokenType::OR)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::_and();
    _expr = lox::expr::Logical(_expr, op, right);  // TODO: new?
  }

  return _expr;
}


lox::expr::Expr Parser::_and() {
  lox::expr::Expr _expr = Parser::equality();

  while (Parser::match(TokenType::AND)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::equality();
    _expr = lox::expr::Logical(_expr, op, right);
  }

  return _expr;
}


bool Parser::match(const TokenType& types, ...) {
  va_list args;
  va_start(args, types);

  for (TokenType type = types; type != TokenType::_EOF;
       type = va_arg(args, TokenType)) {
    if (Parser::check(type)) {
      Parser::advance();
      va_end(args);
      return true;
    }
  }

  va_end(args);
  return false;
}


bool Parser::check(const TokenType& type) {
  if (Parser::isAtEnd()) {
    return false;
  }
  return Parser::peek().tokentype() == type;
}


Token Parser::advance() {
  if (!Parser::isAtEnd()) {
    current++;
  }
  return Parser::previous();
}


bool Parser::isAtEnd() {
  return Parser::peek().tokentype() == TokenType::_EOF;
}


Token Parser::peek() {
  return tokens[current];
}


Token Parser::previous() {
  return tokens[current - 1];
}


lox::expr::Expr Parser::comparison() {
  lox::expr::Expr expr = Parser::term();

  while (Parser::match(
      TokenType::GREATER,
      TokenType::GREATER_EQUAL,
      TokenType::LESS,
      TokenType::LESS_EQUAL)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::term();
    expr = lox::expr::Binary(expr, op, right);
  }

  return expr;
}


lox::expr::Expr Parser::term() {
  lox::expr::Expr expr = Parser::factor();

  while (Parser::match(TokenType::MINUS, TokenType::PLUS)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::factor();
    expr = lox::expr::Binary(expr, op, right);
  }

  return expr;
}


lox::expr::Expr Parser::factor() {
  lox::expr::Expr expr = Parser::unary();

  while (Parser::match(TokenType::SLASH, TokenType::STAR)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::unary();
    expr = lox::expr::Binary(expr, op, right);
  }

  return expr;
}


lox::expr::Expr Parser::unary() {
  if (Parser::match(TokenType::BANG, TokenType::MINUS)) {
    Token op = Parser::previous();
    lox::expr::Expr right = Parser::unary();
    return lox::expr::Unary(op, right);
  }

  // return Parser::primary();
  return Parser::call();
}


lox::expr::Expr Parser::finishCall(const lox::expr::Expr& callee) {
  std::vector<lox::expr::Expr> arguments;

  if (!Parser::check(TokenType::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        ParseError error(Parser::peek(), "Can't have more than 255 arguments.");
      }

      arguments.push_back(Parser::expression());
    } while (Parser::match(TokenType::COMMA));
  }

  Token paren =
      Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

  return lox::expr::Call(callee, paren, arguments);
}


lox::expr::Expr Parser::call() {
  lox::expr::Expr _expr = Parser::primary();

  while (true) {
    if (Parser::match(TokenType::LEFT_PAREN)) {
      _expr = Parser::finishCall(_expr);

    } else if (Parser::match(TokenType::DOT)) {
      Token name = Parser::consume(
          TokenType::IDENTIFIER, "Expect property name after '.'.");
      _expr = lox::expr::Get(_expr, name);
    } else {
      break;
    }
  }
  return _expr;
}


lox::expr::Expr Parser::primary() {
  if (Parser::match(TokenType::FALSE)) {
    return lox::expr::Literal(false);
  }

  if (Parser::match(TokenType::TRUE)) {
    return lox::expr::Literal(true);
  }

  if (Parser::match(TokenType::NIL)) {
    return lox::expr::Literal(nullptr);
  }

  if (Parser::match(TokenType::NUMBER, TokenType::STRING)) {
    return lox::expr::Literal(Parser::previous().getLiteral());
  }

  if (Parser::match(TokenType::SUPER)) {
    Token keyword = Parser::previous();
    Parser::consume(TokenType::DOT, "Expect '.' after 'super'.");
    Token method = Parser::consume(
        TokenType::IDENTIFIER, "Expect superclass method name.");
    // don't use "new" keyword
    return lox::expr::Super(keyword, method);
  }

  if (Parser::match(TokenType::THIS)) {
    return lox::expr::This(Parser::previous());
  }

  if (Parser::match(TokenType::IDENTIFIER)) {
    return lox::expr::Variable(Parser::previous());
  }

  if (Parser::match(TokenType::LEFT_PAREN)) {
    lox::expr::Expr expr = Parser::expression();
    Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return lox::expr::Grouping(expr);
  }

  throw ParseError(Parser::peek(), "Expect expression.");
}


// entering panic mode

Token Parser::consume(const TokenType& type, const std::string& message) {
  if (Parser::check(type)) {
    return Parser::advance();
  }

  throw ParseError(Parser::peek(), message);
}


lox::parser::ParseError error(const Token& token, const std::string& message) {
  // Lox::error(token, message);
  return lox::parser::ParseError(token, message);
}


// synchronizing the recursive decent parser

void Parser::synchronize() {
  Parser::advance();

  while (!Parser::isAtEnd()) {
    if (Parser::previous().tokentype() == TokenType::SEMICOLON) {
      return;
    }
    switch (Parser::peek().tokentype()) {
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
    Parser::advance();
  }
}

}  // namespace parser

}  // namespace lox

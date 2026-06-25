#include <initializer_list>
#include <memory>
#include <utility>
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


std::vector<std::shared_ptr<lox::stmt::Stmt>> Parser::parse() {
  std::vector<std::shared_ptr<lox::stmt::Stmt>> statements;

  while (!Parser::isAtEnd()) {
    std::shared_ptr<lox::stmt::Stmt> declaration = Parser::declaration();
    if (declaration != nullptr) {
      statements.push_back(declaration);
    }
  }
  return statements;
}


// declaration

std::shared_ptr<lox::stmt::Stmt> Parser::declaration() {
  try {
    if (Parser::match({TokenType::CLASS})) {
      return Parser::classDeclaration();
    }

    if (Parser::match({TokenType::FUN})) {
      return Parser::function("function");
    }

    if (Parser::match({TokenType::VAR})) {
      return Parser::varDeclaration();
    }

    return Parser::statement();

  } catch (const ParseError&) {
    Parser::synchronize();
    return nullptr;
  }
}


// class declaration

std::shared_ptr<lox::stmt::Stmt> Parser::classDeclaration() {
  Token name = Parser::consume(TokenType::IDENTIFIER, "Expect class name.");

  std::shared_ptr<lox::expr::Variable> superclass = nullptr;
  if (Parser::match({TokenType::LESS})) {
    Parser::consume(TokenType::IDENTIFIER, "Expect superclass name.");
    superclass = std::make_shared<lox::expr::Variable>(Parser::previous());
  }

  Parser::consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

  std::vector<std::shared_ptr<lox::stmt::Function>> methods;
  while (!Parser::check(TokenType::RIGHT_BRACE) && !Parser::isAtEnd()) {
    methods.push_back(Parser::function("method"));
  }

  Parser::consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

  return std::make_shared<lox::stmt::Class>(
      name, superclass, std::move(methods));
}


std::shared_ptr<lox::stmt::Stmt> Parser::statement() {
  if (Parser::match({TokenType::FOR})) {
    return Parser::forStatement();
  }

  if (Parser::match({TokenType::IF})) {
    return Parser::ifStatement();
  }

  if (Parser::match({TokenType::PRINT})) {
    return Parser::printStatement();
  }

  if (Parser::match({TokenType::RETURN})) {
    return Parser::returnStatement();
  }

  if (Parser::match({TokenType::WHILE})) {
    return Parser::whileStatement();
  }

  if (Parser::match({TokenType::LEFT_BRACE})) {
    return std::make_shared<lox::stmt::Block>(Parser::block());
  }

  return Parser::expressionStatement();
}


// for stmt

std::shared_ptr<lox::stmt::Stmt> Parser::forStatement() {
  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

  std::shared_ptr<lox::stmt::Stmt> initializer;
  if (Parser::match({TokenType::SEMICOLON})) {
    initializer = nullptr;

  } else if (Parser::match({TokenType::VAR})) {
    initializer = Parser::varDeclaration();

  } else {
    initializer = Parser::expressionStatement();
  }

  std::shared_ptr<lox::expr::Expr> condition = nullptr;
  if (!Parser::check(TokenType::SEMICOLON)) {
    condition = Parser::expression();
  }

  Parser::consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

  std::shared_ptr<lox::expr::Expr> increment = nullptr;
  if (!Parser::check(TokenType::RIGHT_PAREN)) {
    increment = Parser::expression();
  }

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

  std::shared_ptr<lox::stmt::Stmt> body = Parser::statement();

  if (increment != nullptr) {
    std::vector<std::shared_ptr<lox::stmt::Stmt>> _stmt;
    _stmt.push_back(body);
    _stmt.push_back(std::make_shared<lox::stmt::Expression>(increment));
    body = std::make_shared<lox::stmt::Block>(std::move(_stmt));
  }

  if (condition == nullptr) {
    condition = std::make_shared<lox::expr::Literal>(true);
  }

  body = std::make_shared<lox::stmt::While>(condition, body);

  if (initializer != nullptr) {
    std::vector<std::shared_ptr<lox::stmt::Stmt>> _stmt;
    _stmt.push_back(initializer);
    _stmt.push_back(body);
    body = std::make_shared<lox::stmt::Block>(std::move(_stmt));
  }

  return body;
}


// if stmt

std::shared_ptr<lox::stmt::Stmt> Parser::ifStatement() {
  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  std::shared_ptr<lox::expr::Expr> condition = Parser::expression();

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
  std::shared_ptr<lox::stmt::Stmt> thenBranch = Parser::statement();

  std::shared_ptr<lox::stmt::Stmt> elseBranch = nullptr;
  if (Parser::match({TokenType::ELSE})) {
    elseBranch = Parser::statement();
  }

  return std::make_shared<lox::stmt::If>(condition, thenBranch, elseBranch);
}


// print stmt

std::shared_ptr<lox::stmt::Stmt> Parser::printStatement() {
  std::shared_ptr<lox::expr::Expr> value = Parser::expression();
  Parser::consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_shared<lox::stmt::Print>(value);
}


// return stmt

std::shared_ptr<lox::stmt::Stmt> Parser::returnStatement() {
  Token keyword = Parser::previous();
  std::shared_ptr<lox::expr::Expr> value = nullptr;

  if (!Parser::check(TokenType::SEMICOLON)) {
    value = Parser::expression();
  }

  Parser::consume(TokenType::SEMICOLON, "Expect ';' after return value.");

  return std::make_shared<lox::stmt::Return>(keyword, value);
}


// var declaration

std::shared_ptr<lox::stmt::Stmt> Parser::varDeclaration() {
  Token name = Parser::consume(TokenType::IDENTIFIER, "Expect variable name.");

  std::shared_ptr<lox::expr::Expr> initializer = nullptr;
  if (Parser::match({TokenType::EQUAL})) {
    initializer = Parser::expression();
  }

  Parser::consume(
      TokenType::SEMICOLON, "Expect ';' after variable declaration.");

  return std::make_shared<lox::stmt::Var>(name, initializer);
}


// while stmt

std::shared_ptr<lox::stmt::Stmt> Parser::whileStatement() {
  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  std::shared_ptr<lox::expr::Expr> condition = Parser::expression();

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
  std::shared_ptr<lox::stmt::Stmt> body = Parser::statement();

  return std::make_shared<lox::stmt::While>(condition, body);
}


// expression stmt

std::shared_ptr<lox::stmt::Stmt> Parser::expressionStatement() {
  std::shared_ptr<lox::expr::Expr> _expr = Parser::expression();
  Parser::consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return std::make_shared<lox::stmt::Expression>(_expr);
}


// function

std::shared_ptr<lox::stmt::Function> Parser::function(
    const std::string& kind) {
  Token name = Parser::consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

  Parser::consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");

  std::vector<Token> parameters;
  if (!Parser::check(TokenType::RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255) {
        Parser::error(Parser::peek(), "Can't have more than 255 parameters.");
      }

      parameters.push_back(
          Parser::consume(TokenType::IDENTIFIER, "Expect parameter name."));
    } while (Parser::match({TokenType::COMMA}));
  }

  Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
  Parser::consume(
      TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
  std::vector<std::shared_ptr<lox::stmt::Stmt>> body = Parser::block();

  return std::make_shared<lox::stmt::Function>(
      name, std::move(parameters), std::move(body));
}


std::vector<std::shared_ptr<lox::stmt::Stmt>> Parser::block() {
  std::vector<std::shared_ptr<lox::stmt::Stmt>> statements;

  while (!Parser::check(TokenType::RIGHT_BRACE) && !Parser::isAtEnd()) {
    std::shared_ptr<lox::stmt::Stmt> declaration = Parser::declaration();
    if (declaration != nullptr) {
      statements.push_back(declaration);
    }
  }

  Parser::consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}


// below are the rules, converting themselves to the tree structure

std::shared_ptr<lox::expr::Expr> Parser::expression() {
  return Parser::assignment();
}


// assignment

std::shared_ptr<lox::expr::Expr> Parser::assignment() {
  std::shared_ptr<lox::expr::Expr> _expr = Parser::_or();

  if (Parser::match({TokenType::EQUAL})) {
    Token equals = Parser::previous();
    std::shared_ptr<lox::expr::Expr> value = Parser::assignment();

    if (auto variable =
            std::dynamic_pointer_cast<lox::expr::Variable>(_expr)) {
      return std::make_shared<lox::expr::Assign>(variable->getName(), value);

    } else if (auto get = std::dynamic_pointer_cast<lox::expr::Get>(_expr)) {
      return std::make_shared<lox::expr::Set>(
          get->getObject(), get->getName(), value);
    }

    Parser::error(equals, "Invalid assignment target.");
  }

  return _expr;
}


std::shared_ptr<lox::expr::Expr> Parser::equality() {
  std::shared_ptr<lox::expr::Expr> expr = Parser::comparison();

  while (Parser::match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::comparison();
    expr = std::make_shared<lox::expr::Binary>(expr, op, right);
  }

  return expr;
}


std::shared_ptr<lox::expr::Expr> Parser::_or() {
  std::shared_ptr<lox::expr::Expr> _expr = Parser::_and();

  while (Parser::match({TokenType::OR})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::_and();
    _expr = std::make_shared<lox::expr::Logical>(_expr, op, right);
  }

  return _expr;
}


std::shared_ptr<lox::expr::Expr> Parser::_and() {
  std::shared_ptr<lox::expr::Expr> _expr = Parser::equality();

  while (Parser::match({TokenType::AND})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::equality();
    _expr = std::make_shared<lox::expr::Logical>(_expr, op, right);
  }

  return _expr;
}


bool Parser::match(std::initializer_list<TokenType> types) {
  for (const auto& type : types) {
    if (Parser::check(type)) {
      Parser::advance();
      return true;
    }
  }
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


std::shared_ptr<lox::expr::Expr> Parser::comparison() {
  std::shared_ptr<lox::expr::Expr> expr = Parser::term();

  while (Parser::match(
      {TokenType::GREATER,
       TokenType::GREATER_EQUAL,
       TokenType::LESS,
       TokenType::LESS_EQUAL})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::term();
    expr = std::make_shared<lox::expr::Binary>(expr, op, right);
  }

  return expr;
}


std::shared_ptr<lox::expr::Expr> Parser::term() {
  std::shared_ptr<lox::expr::Expr> expr = Parser::factor();

  while (Parser::match({TokenType::MINUS, TokenType::PLUS})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::factor();
    expr = std::make_shared<lox::expr::Binary>(expr, op, right);
  }

  return expr;
}


std::shared_ptr<lox::expr::Expr> Parser::factor() {
  std::shared_ptr<lox::expr::Expr> expr = Parser::unary();

  while (Parser::match({TokenType::SLASH, TokenType::STAR})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::unary();
    expr = std::make_shared<lox::expr::Binary>(expr, op, right);
  }

  return expr;
}


std::shared_ptr<lox::expr::Expr> Parser::unary() {
  if (Parser::match({TokenType::BANG, TokenType::MINUS})) {
    Token op = Parser::previous();
    std::shared_ptr<lox::expr::Expr> right = Parser::unary();
    return std::make_shared<lox::expr::Unary>(op, right);
  }

  return Parser::call();
}


std::shared_ptr<lox::expr::Expr> Parser::finishCall(
    std::shared_ptr<lox::expr::Expr> callee) {
  std::vector<std::shared_ptr<lox::expr::Expr>> arguments;

  if (!Parser::check(TokenType::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        Parser::error(Parser::peek(), "Can't have more than 255 arguments.");
      }

      arguments.push_back(Parser::expression());
    } while (Parser::match({TokenType::COMMA}));
  }

  Token paren =
      Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

  return std::make_shared<lox::expr::Call>(callee, paren, std::move(arguments));
}


std::shared_ptr<lox::expr::Expr> Parser::call() {
  std::shared_ptr<lox::expr::Expr> _expr = Parser::primary();

  while (true) {
    if (Parser::match({TokenType::LEFT_PAREN})) {
      _expr = Parser::finishCall(_expr);

    } else if (Parser::match({TokenType::DOT})) {
      Token name = Parser::consume(
          TokenType::IDENTIFIER, "Expect property name after '.'.");
      _expr = std::make_shared<lox::expr::Get>(_expr, name);
    } else {
      break;
    }
  }
  return _expr;
}


std::shared_ptr<lox::expr::Expr> Parser::primary() {
  if (Parser::match({TokenType::FALSE})) {
    return std::make_shared<lox::expr::Literal>(false);
  }

  if (Parser::match({TokenType::TRUE})) {
    return std::make_shared<lox::expr::Literal>(true);
  }

  if (Parser::match({TokenType::NIL})) {
    return std::make_shared<lox::expr::Literal>(nullptr);
  }

  if (Parser::match({TokenType::NUMBER, TokenType::STRING})) {
    return std::make_shared<lox::expr::Literal>(Parser::previous().getLiteral());
  }

  if (Parser::match({TokenType::SUPER})) {
    Token keyword = Parser::previous();
    Parser::consume(TokenType::DOT, "Expect '.' after 'super'.");
    Token method = Parser::consume(
        TokenType::IDENTIFIER, "Expect superclass method name.");
    return std::make_shared<lox::expr::Super>(keyword, method);
  }

  if (Parser::match({TokenType::THIS})) {
    return std::make_shared<lox::expr::This>(Parser::previous());
  }

  if (Parser::match({TokenType::IDENTIFIER})) {
    return std::make_shared<lox::expr::Variable>(Parser::previous());
  }

  if (Parser::match({TokenType::LEFT_PAREN})) {
    std::shared_ptr<lox::expr::Expr> expr = Parser::expression();
    Parser::consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_shared<lox::expr::Grouping>(expr);
  }

  throw Parser::error(Parser::peek(), "Expect expression.");
}


// entering panic mode

Token Parser::consume(const TokenType& type, const std::string& message) {
  if (Parser::check(type)) {
    return Parser::advance();
  }

  throw Parser::error(Parser::peek(), message);
}


ParseError Parser::error(const Token& token, const std::string& message) {
  lox::Lox::error(token, message);
  return ParseError(token, message);
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
      default:
        break;
    }
    Parser::advance();
  }
}

}  // namespace parser

}  // namespace lox

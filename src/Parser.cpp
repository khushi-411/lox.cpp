#include <vector>

#include "Expr.h"
#include "Lox.h"
#include "Parser.h"
#include "Token.h"
#include "TokenType.h"


namespace lox {

// input: sequence of tokens

template <class T>
lox::parser::Parser<T>::Parser(const std::vector<Token>& tokens) {
  // don't use constructor initialization, because tokens
  // is not a member of parser class
  this->tokens = tokens;
}


template <class T>
std::vector<lox::stmt::Stmt<T>> lox::parser::Parser<T>::parseStmt() {
  std::vector<lox::stmt::Stmt<T>> statements;

  while (!isAtEnd()) {
    statements.push_back(lox::parser::Parser<T>::statement());
  }
  return statements;
}


template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::statement() {
  if (lox::parser::Parser<T>::match(TokenType::FOR)) {
    return lox::parser::Parser<T>::forStatement();
  }

  if (lox::parser::Parser<T>::match(TokenType::IF)) {
    return lox::parser::Parser<T>::ifStatement();
  }

  if (lox::parser::Parser<T>::match(TokenType::PRINT)) {
    return lox::parser::Parser<T>::printStatement();
  }

  if (lox::parser::Parser<T>::match(TokenType::RETURN)) {
    return lox::parser::Parser<T>::returnStatement();
  }

  if (lox::parser::Parser<T>::match(TokenType::WHILE)) {
    return lox::parser::Parser<T>::whileStatement();
  }

  if (lox::parser::Parser<T>::match(TokenType::LEFT_BRACE)) {
    return lox::stmt::Stmt<T>::Block(lox::parser::Parser<T>::block());
  }

  return lox::parser::Parser<T>::expressionStatement();
}


// for stmt

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::forStatement() {
  lox::parser::Parser<T>::consume(
      TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

  lox::stmt::Stmt<T> initializer;
  if (lox::parser::Parser<T>::match(TokenType::SEMICOLON)) {
    initializer = nullptr;

  } else if (lox::parser::Parser<T>::match(TokenType::VAR)) {
    initializer = lox::parser::Parser<T>::varDeclaration();

  } else {
    initializer = lox::parser::Parser<T>::expressionStatement();
  }

  lox::expr::Expr<T> condition = nullptr;
  if (!lox::parser::Parser<T>::match(TokenType::SEMICOLON)) {
    condition = lox::parser::Parser<T>::expression();
  }

  lox::parser::Parser<T>::consume(
      TokenType::SEMICOLON, "Expect ';' after loop condition.");

  lox::expr::Expr<T> increment = nullptr;
  if (!lox::parser::Parser<T>::check(TokenType::RIGHT_PAREN)) {
    increment = lox::parser::Parser<T>::expression();
  }

  lox::parser::Parser<T>::consume(
      TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

  lox::stmt::Stmt<T> body = lox::parser::Parser<T>::statement();

  if (increment != nullptr) {
    std::vector<lox::stmt::Stmt<T>> _stmt;
    _stmt.push_back(lox::stmt::Stmt<T>::Expression(increment));
    body = lox::stmt::Stmt<T>::Block(_stmt);
  }

  if (condition == nullptr) {
    condition = lox::expr::Expr<T>::Literal(true);
  }

  body = lox::stmt::Stmt<T>::While(condition, body);

  if (initializer != nullptr) {
    std::vector<lox::stmt::Stmt<T>> _stmt = body;
    body = lox::stmt::Stmt<T>::Block(_stmt);
  }

  return body;
}


// if stmt

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::ifStatement() {
  lox::parser::Parser<T>::consume(
      TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  lox::expr::Expr<T> condition = lox::parser::Parser<T>::expression();
  lox::parser::Parser<T>::consume(
      TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

  lox::stmt::Stmt<T> thenBranch = lox::parser::Parser<T>::statement();
  lox::stmt::Stmt<T> elseBranch = nullptr;

  if (lox::parser::Parser<T>::match(TokenType::ELSE)) {
    elseBranch = lox::parser::Parser<T>::statement();
  }

  return lox::stmt::Stmt<T>::If(condition, thenBranch, elseBranch);
}


// print stmt

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::printStatement() {
  lox::expr::Expr<T> value = lox::parser::Parser<T>::expression();
  lox::parser::Parser<T>::consume(
      TokenType::SEMICOLON, "Expect ';' after value.");
  return new lox::stmt::Stmt<T>::Print(value);
}


// return stmt

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::returnStatement() {
  Token keyword = lox::parser::Parser<T>::previous();
  lox::expr::Expr<T> value = nullptr;

  if (!lox::parser::Parser<T>::check(TokenType::SEMICOLON)) {
    value = lox::parser::Parser<T>::expression();
  }

  lox::parser::Parser<T>::consume(
      TokenType::SEMICOLON, "Expect ';' after return value.");

  return lox::stmt::Stmt<T>::Return(keyword, value);
}


// var declaration

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::varDeclaration() {
  Token name = lox::parser::Parser<T>::consume(
      TokenType::IDENTIFIER, "Expect variable name.");

  lox::expr::Expr<T> initializer = nullptr;
  if (lox::parser::Parser<T>::match(TokenType::EQUAL)) {
    initializer = lox::parser::Parser<T>::expression();
  }

  lox::parser::Parser<T>::consume(
      TokenType::SEMICOLON, "Expect ';' after variable declaration.");

  return lox::stmt::Stmt<T>::Var(name, initializer);
}


// while stmt

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::whileStatement() {
  lox::parser::Parser<T>::consume(
      TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  lox::expr::Expr<T> condition = lox::parser::Parser<T>::expression();

  lox::parser::Parser<T>::consume(
      TokenType::RIGHT_PAREN, "Expect ')' after condition.");
  lox::stmt::Stmt<T> body = lox::parser::Parser<T>::statement();

  return lox::stmt::Stmt<T>::While(condition, body);
}


// expression stmt

template <class T>
lox::stmt::Stmt<T> lox::parser::Parser<T>::expressionStatement() {
  lox::expr::Expr<T> _expr = lox::parser::Parser<T>::expression();
  lox::parser::Parser<T>::consume(
      TokenType::SEMICOLON, "Expect ';' after expression.");
  return new lox::stmt::Stmt<T>::Expression(_expr);
}


template <class T>
lox::stmt::Stmt<T>::Function function(const std::string& kind) {
  Token name = lox::parser::Parser<T>::consume(
      TokenType::IDENTIFIER, "Expect " + kind + " name.");

  lox::parser::Parser<T>::consume(
      TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");

  std::vector<Token> parameters;
  if (!lox::parser::Parser<T>::check(TokenType::RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255) {
        lox::parser::Parser<T>::error(
            lox::parser::Parser<T>::peek(),
            "Can't have more than 255 parameters.");
      }

      parameters.push_back(lox::parser::Parser<T>::consume(
          TokenType::IDENTIFIER, "Expect parameter name."));
    } while (lox::parser::Parser<T>::match(TokenType::COMMA));
  }

  lox::parser::Parser<T>::consume(
      TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
  lox::parser::Parser<T>::consume(
      TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
  std::vector<lox::stmt::Stmt<T>> body = lox::parser::Parser<T>::block();

  return lox::stmt::Stmt<T>::Function(name, parameters, body);
}

template <class T>
std::vector<lox::stmt::Stmt<T>> lox::parser::Parser<T>::block() {
  std::vector<lox::stmt::Stmt<T>> statements;

  while (!lox::parser::Parser<T>::check(TokenType::RIGHT_BRACE) &&
         !lox::parser::Parser<T>::isAtEnd()) {
    statements.push_back(lox::parser::Parser<T>::declaration());
  }

  lox::parser::Parser<T>::consume(
      TokenType::RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}


template <class T>
lox::stmt::Stmt<T> declaration() {
  try {
    if (lox::parser::Parser<T>::match(TokenType::CLASS)) {
      return lox::parser::Parser<T>::classDeclaration();
    }

    if (lox::parser::Parser<T>::match(TokenType::FUN)) {
      return lox::parser::Parser<T>::function("function");
    }

    if (lox::parser::Parser<T>::match(TokenType::VAR)) {
      return lox::parser::Parser<T>::varDeclaration();
    }

    return lox::parser::Parser<T>::statement();

  } catch (lox::parser::ParseError error) {
    lox::parser::Parser<T>::synchronize();
    return nullptr;
  }
}


template <class T>
lox::stmt::Stmt<T> classDeclaration() {
  Token name = lox::parser::Parser<T>::consume(
      TokenType::IDENTIFIER, "Expect class name.");

  lox::expr::Variable<T> superclass = nullptr;
  if (lox::parser::Parser<T>::match(TokenType::LESS)) {
    lox::parser::Parser<T>::consume(
        TokenType::IDENTIFIER, "Expect superclass name.");
    superclass =
        lox::expr::Expr<T>::Variable(lox::parser::Parser<T>::previous());
  }

  lox::parser::Parser<T>::consume(
      TokenType::LEFT_BRACE, "Expect '{' before class body.");

  std::vector<lox::stmt::Function<T>> methods;
  while (!lox::parser::Parser<T>::check(TokenType::RIGHT_BRACE) &&
         !lox::parser::Parser<T>::isAtEnd()) {
    methods.push_back(lox::parser::Parser<T>::function("method"));
  }

  lox::parser::Parser<T>::consume(
      TokenType::RIGHT_BRACE, "Expect '}' after class body.");

  return lox::stmt::Stmt<T>::Class(name, superclass, methods);
}


// below are the rules, converting themselves to the tree structure

template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::parse() {
  try {
    return lox::parser::Parser<T>::expression();
  } catch (lox::parser::ParseError error) {
    // returns NULL because we want to take this to the interpreter
    return NULL;
  }
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::expression() {
  // for parsing expressions
  return lox::parser::Parser<T>::equality();
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::equality() {
  lox::expr::Expr<T> expr = lox::parser::Parser<T>::comparison();
  while (lox::parser::Parser<T>::match(
      TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::comparison();
    expr = lox::expr::Expr<T>::Binary(expr, op, right);
  }
  return expr;
}


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::assignment() {
  lox::expr::Expr<T> _expr = lox::parser::Parser<T>::_or();

  if (lox::parser::Parser<T>::match(TokenType::EQUAL)) {
    Token equals = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> value = lox::parser::Parser<T>::assignment();

    if (_expr<lox::expr::Expr<T>::Variable>(_expr)) {
      Token name = (reinterpret_cast<lox::expr::Variable<T>>(_expr)).name;
      return lox::expr::Expr<T>::Assign(name, value);

    } else if (_expr<lox::expr::Expr<T>::Get>(_expr)) {
      lox::expr::Get<T> get =
          reinterpret_cast<lox::expr::Get<T>>(_expr);  // TODO
      return lox::expr::Expr<T>::Set(get.object, get.name, value);
    }

    lox::parser::Parser<T>::error(equals, "Invalid assignment target.");
  }

  return _expr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::_or() {
  lox::expr::Expr<T> _expr = lox::parser::Parser<T>::_and();

  while (lox::parser::Parser<T>::match(TokenType::OR)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::_and();
    _expr = lox::expr::Expr<T>::Logical(_expr, op, right);  // TODO: new?
  }

  return _expr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::_and() {
  lox::expr::Expr<T> _expr = lox::parser::Parser<T>::equality();

  while (lox::parser::Parser<T>::match(TokenType::AND)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::equality();
    _expr = lox::expr::Expr<T>::Logical(_expr, op, right);
  }

  return _expr;
}


template <class T>
bool lox::parser::Parser<T>::match(const TokenType& types) {
  for (TokenType type : types) {
    if (lox::parser::Parser<T>::check(type)) {
      lox::parser::Parser<T>::advance();
      return true;
    }
  }
  return false;
}


template <class T>
bool lox::parser::Parser<T>::check(const TokenType& type) {
  if (lox::parser::Parser<T>::isAtEnd()) {
    return false;
  }
  return lox::parser::Parser<T>::peek().type == type;
}


template <class T>
Token lox::parser::Parser<T>::advance() {
  if (!lox::parser::Parser<T>::isAtEnd()) {
    current++;
  }
  return lox::parser::Parser<T>::previous();
}


template <class T>
bool lox::parser::Parser<T>::isAtEnd() {
  return lox::parser::Parser<T>::peek().type == TokenType::_EOF;
}


template <class T>
Token lox::parser::Parser<T>::peek() {
  return tokens[current];
}


template <class T>
Token lox::parser::Parser<T>::previous() {
  return tokens[current - 1];
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::comparison() {
  lox::expr::Expr<T> expr = lox::parser::Parser<T>::term();

  while (lox::parser::Parser<T>::match(
      TokenType::GREATER,
      TokenType::GREATER_EQUAL,
      TokenType::LESS,
      TokenType::LESS_EQUAL)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::term();
    expr = lox::expr::Expr<T>::Binary(expr, op, right);
  }

  return expr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::term() {
  lox::expr::Expr<T> expr = lox::parser::Parser<T>::factor();

  while (lox::parser::Parser<T>::match(TokenType::MINUS, TokenType::PLUS)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::factor();
    expr = lox::expr::Expr<T>::Binary(expr, op, right);
  }

  return expr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::factor() {
  lox::expr::Expr<T> expr = lox::parser::Parser<T>::unary();

  while (lox::parser::Parser<T>::match(TokenType::SLASH, TokenType::STAR)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::unary();
    expr = lox::expr::Expr<T>::Binary(expr, op, right);
  }

  return expr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::unary() {
  if (lox::parser::Parser<T>::match(TokenType::BANG, TokenType::MINUS)) {
    Token op = lox::parser::Parser<T>::previous();
    lox::expr::Expr<T> right = lox::parser::Parser<T>::unary();
    return lox::expr::Expr<T>::Unary(op, right);
  }

  // return lox::parser::Parser<T>::primary();
  return lox::parser::Parser<T>::call();
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::finishCall(
    const lox::expr::Expr<T>& callee) {
  std::vector<lox::expr::Expr<T>> arguments;

  if (!lox::parser::Parser<T>::check(TokenType::RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        lox::parser::Parser<T>::error(
            lox::parser::Parser<T>::peek(),
            "Can't have more than 255 arguments.");
      }

      arguments.push_back(lox::parser::Parser<T>::expression());
    } while (lox::parser::Parser<T>::match(TokenType::COMMA));
  }

  Token paren = lox::parser::Parser<T>::consume(
      TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

  return lox::expr::Expr<T>::Call(callee, paren, arguments);
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::call() {
  lox::expr::Expr<T> _expr = lox::parser::Parser<T>::primary();

  while (true) {
    if (lox::parser::Parser<T>::match(TokenType::LEFT_PAREN)) {
      _expr = lox::parser::Parser<T>::finishCall(_expr);

    } else if (lox::parser::Parser<T>::match(TokenType::DOT)) {
      Token name = lox::parser::Parser<T>::consume(
          TokenType::IDENTIFIER, "Expect property name after '.'.");
      _expr = lox::expr::Expr<T>::Get(_expr, name);
    } else {
      break;
    }
  }
  return _expr;
}


template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::primary() {
  if (lox::parser::Parser<T>::match(TokenType::FALSE)) {
    return lox::expr::Expr<T>::Literal(false);
  }

  if (lox::parser::Parser<T>::match(TokenType::TRUE)) {
    return lox::expr::Expr<T>::Literal(true);
  }

  if (lox::parser::Parser<T>::match(TokenType::NIL)) {
    return lox::expr::Expr<T>::Literal(NULL);
  }

  if (lox::parser::Parser<T>::match(TokenType::NUMBER, TokenType::STRING)) {
    return lox::expr::Expr<T>::Literal(
        lox::parser::Parser<T>::previous().literal());
  }

  if (lox::parser::Parser<T>::match(TokenType::SUPER)) {
    Token keyword = lox::parser::Parser<T>::pervious();
    lox::parser::Parser<T>::consume(
        TokenType::DOT, "Expect '.' after 'super'.");
    Token method = lox::parser::Parser<T>::consume(
        TokenType::IDENTIFIER, "Expect superclass method name.");
    return new lox::expr::Expr<T>::Super(keyword, method);
  }

  if (lox::parser::Parser<T>::match(TokenType::THIS)) {
    return new lox::expr::Expr<T>::This(lox::parser::Parser<T>::pervious());
  }

  if (lox::parser::Parser<T>::match(TokenType::IDENTIFIER)) {
    return new lox::expr::Expr<T>::Variable(lox::parser::Parser<T>::pervious());
  }

  if (lox::parser::Parser<T>::match(TokenType::LEFT_PAREN)) {
    lox::expr::Expr<T> expr = lox::parser::Parser<T>::expression();
    lox::parser::Parser<T>::consume(
        TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return lox::expr::Expr<T>::Grouping(expr);
  }

  throw lox::parser::Parser<T>::error(
      lox::parser::Parser<T>::peek(), "Expect expression.");
}


// entering panic mode

template <class T>
Token lox::parser::Parser<T>::consume(
    const TokenType& type,
    const std::string& message) {
  if (lox::parser::Parser<T>::check(type)) {
    return lox::parser::Parser<T>::advance();
  }
  throw lox::parser::Parser<T>::error(lox::parser::Parser<T>::peek(), message);
}


lox::parser::ParseError error(const Token& token, const std::string& message) {
  Lox::error(token, message);
  return lox::parser::ParseError(token, message);
}


// synchronizing the recursive decent parser

template <class T>
void lox::parser::Parser<T>::synchronize() {
  lox::parser::Parser<T>::advance();

  while (!lox::parser::Parser<T>::isAtEnd()) {
    if (lox::parser::Parser<T>::previous().type == TokenType::SEMICOLON) {
      return;
    }
    switch (lox::parser::Parser<T>::peek().type) {
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
    lox::parser::Parser<T>::advance();
  }
}

}  // namespace lox

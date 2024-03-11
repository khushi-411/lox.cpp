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
lox::expr::Expr<T> lox::parser::Parser<T>::parse() {
  try {
    return lox::parser::Parser<T>::expression();
  } catch (lox::parser::ParseError error) {
    // returns NULL because we want to take this to the interpreter
    return NULL;
  }
}


// below are the rules, converting themselves to the tree structure

template <class T>
lox::expr::Expr<T> lox::parser::Parser<T>::expression() {
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

  return lox::parser::Parser<T>::primary();
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


// synchronizing the rcursive decent parser

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

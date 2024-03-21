#include <string.h>
#include <cstddef>
#include <iostream>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Lox.h"
#include "Scanner.h"
#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

// TODO: use auto instead of bool datatype?
// TODO: maybe use static inline function during declaration?
// TODO: use std::unique_ptr, std::shared_ptr etc
// TODO: use constexpr?

// https://stackoverflow.com/questions/19918369
template <class T>
Scanner<T>::Scanner(const std::string& source) : source(source) {}


// reserved words

template <class T>
Scanner<T>::Scanner()
    : keywords{
          {"and", AND},
          {"class", CLASS},
          {"else", ELSE},
          {"false", FALSE},
          {"for", FOR},
          {"fun", FUN},
          {"if", IF},
          {"nil", NIL},
          {"or", OR},
          {"print", PRINT},
          {"return", RETURN},
          {"super", SUPER},
          {"this", THIS},
          {"true", TRUE},
          {"var", VAR},
          {"while", WHILE}} {}


// scan single character lexemes

template <class T>
std::vector<Token> Scanner<T>::scanTokens() {
  while (!Scanner::isAtEnd()) {
    start = current;
    Scanner<T>::scanToken();
  }
  tokens.push_back(Token(TokenType::_EOF, "", nullptr, line));
  return tokens;
}


// scan two or more number of characters

template <class T>
void Scanner<T>::scanToken() {
  char c = Scanner<T>::advance();
  switch (c) {
    case '(':
      Scanner<T>::addToken(TokenType::LEFT_PAREN);
      break;
    case ')':
      Scanner<T>::addToken(TokenType::RIGHT_PAREN);
      break;
    case '{':
      Scanner<T>::addToken(TokenType::LEFT_BRACE);
      break;
    case '}':
      Scanner<T>::addToken(TokenType::RIGHT_BRACE);
      break;
    case ',':
      Scanner<T>::addToken(TokenType::COMMA);
      break;
    case '.':
      Scanner<T>::addToken(TokenType::DOT);
      break;
    case '-':
      Scanner<T>::addToken(TokenType::MINUS);
      break;
    case '+':
      Scanner<T>::addToken(TokenType::PLUS);
      break;
    case ';':
      Scanner<T>::addToken(TokenType::SEMICOLON);
      break;
    case '*':
      Scanner<T>::addToken(TokenType::STAR);
      break;
    case '!':
      Scanner<T>::addToken(
          Scanner<T>::match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
      break;
    case '=':
      Scanner<T>::addToken(
          Scanner<T>::match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
      break;
    case '<':
      Scanner<T>::addToken(
          Scanner<T>::match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
      break;
    case '>':
      Scanner<T>::addToken(
          Scanner<T>::match('=') ? TokenType::GREATER_EQUAL
                                 : TokenType::GREATER);
      break;
    case '/':
      if (Scanner<T>::match('/')) {
        while (Scanner<T>::peek() != '\n' && !Scanner<T>::isAtEnd()) {
          Scanner<T>::advance();
        }
      } else {
        Scanner<T>::addToken(SLASH);
      }
      break;
    case ' ':
      break;
    case '\r':
      break;
    case '\t':
      break;
    case '"':
      Scanner<T>::string();
      break;
    default:
      if (Scanner<T>::isDigit(c)) {
        Scanner<T>::number();
      } else if (Scanner<T>::isAlpha(c)) {
        Scanner<T>::identifier();
      } else {
        Lox<T>::error(line, "Unexpected character");
      }
      break;
  }
}


// reserved words

template <class T>
void Scanner<T>::identifier() {
  while (Scanner<T>::isAlphaNumeric(Scanner<T>::peek())) {
    Scanner<T>::advance();
  }

  std::string text = this->source.substr(start, current);
  TokenType type = keywords[text];

  if (type == NULL) {
    type = TokenType::IDENTIFIER;
  }
  Scanner<T>::addToken(type);
}


// number literals

template <class T>
void Scanner<T>::number() {
  while (Scanner<T>::isDigit(peek())) {
    Scanner<T>::advance();
  }

  if (Scanner<T>::peek() == '.' &&
      Scanner<T>::isDigit(Scanner<T>::peekNext())) {
    Scanner<T>::advance();
    while (Scanner<T>::isDigit(Scanner<T>::peek())) {
      Scanner<T>::advance();
    }
  }

  Scanner<T>::addToken(
      TokenType::NUMBER, std::stod(source.substr(start, current)));  // todo
}


// string literals

template <class T>
void Scanner<T>::string() {
  while (Scanner<T>::peek() != '"' && !Scanner<T>::isAtEnd()) {
    if (Scanner<T>::peek() == '\n') {
      line++;
    }
    Scanner<T>::advance();
  }

  if (Scanner<T>::isAtEnd()) {
    Lox<T>::error(line, "Unterminated string");
    return;
  }

  Scanner<T>::advance();
  std::string value = source.substr(start + 1, current - 1);
  Scanner<T>::addToken(STRING, value);
}


// check for two-character lexeme

template <class T>
bool Scanner<T>::match(const char& expected) {
  if (Scanner<T>::isAtEnd()) {
    return false;
  }

  if (source.at(current) != expected) {
    return false;
  }
  current++;
  return true;
}


// longer lexeme

template <class T>
char Scanner<T>::peek() {
  if (Scanner<T>::isAtEnd()) {
    return '\0';
  }
  return source.at(current);
}


// used in number literals, because we don't want "." to consume

template <class T>
char Scanner<T>::peekNext() {
  if (current + 1 >= source.length()) {
    return '\0';
  }
  return source.at(current + 1);
}


template <class T>
bool Scanner<T>::isAlpha(const char& c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}


template <class T>
bool Scanner<T>::isAlphaNumeric(const char& c) {
  return Scanner<T>::isAlpha(c) || Scanner<T>::isDigit(c);
}


template <class T>
bool Scanner<T>::isDigit(const char& c) {
  return c >= '0' && c <= '9';
}


template <class T>
bool Scanner<T>::isAtEnd() {
  return current >= source.length();
}


// to consume the input character

template <class T>
char Scanner<T>::advance() {
  return source.at(current++);
}


// to produce the output

template <class T>
void Scanner<T>::addToken(const TokenType& type) {
  Scanner<T>::addToken(type, nullptr);
}


template <class T>
void Scanner<T>::addToken(const TokenType& type, const Object& literal) {
  std::string text = source.substr(start, current);
  tokens.push_back(Token(type, text, literal, line));
}


}  // namespace lox

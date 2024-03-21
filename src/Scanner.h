#ifndef SCANNER_H
#define SCANNER_H

#include <unordered_map>
#include <variant>
#include <vector>

#include "Token.h"
#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

inline std::vector<Token> tokens;
inline int start = 0;
inline int current = 0;
inline int line = 1;

// https://stackoverflow.com/questions/42056160

template <class T>
class Scanner {
 private:
  std::unordered_map<std::string, TokenType> keywords;
  std::string source;

 public:
  Scanner();

  Scanner(const std::string& source);

  std::vector<Token> scanTokens();
  // https://stackoverflow.com/questions/17391853
  void scanToken();
  void identifier();
  void number();
  void string();
  bool match(const char& expected);
  char peek();
  char peekNext();
  bool isAlpha(const char& c);
  bool isAlphaNumeric(const char& c);
  bool isDigit(const char& c);
  bool isAtEnd();
  char advance();
  void addToken(const TokenType& type);
  void addToken(const TokenType& type, const Object& literal);
};

}  // namespace lox

#endif

#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <unordered_map>

#include "Token.h"
#include "TokenType.h"


namespace lox {

std::vector<Token> tokens;
int start = 0;
int current = 0;
int line = 1;

// https://stackoverflow.com/questions/42056160

class Scanner {

private:
    std::unordered_map<std::string, TokenType> keywords;
    std::string source;

public:

    Scanner();

    Scanner(std::string source);

    std::vector<Token> scanTokens();
    // https://stackoverflow.com/questions/17391853
    static void scanToken();
    static void identifier();
    static void number();
    static void string();
    static bool match(char expected);
    static char peek();
    static char peekNext();
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);
    static bool isDigit(char c);
    static bool isAtEnd();
    static char advance();
    static void addToken(TokenType type);
    static void addToken(TokenType type, std::string literal);
    static void addToken(TokenType type, double literal);
};

// std::string Scanner::source;
// Scanner::Scanner(std::string source) : source(source) {}

}  // namespace lox

#endif

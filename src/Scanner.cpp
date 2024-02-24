#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cstddef>

#include "Lox.h"
#include "Scanner.h"
#include "TokenType.h"

namespace lox {


// https://stackoverflow.com/questions/19918369
Scanner::Scanner(std::string source) {
     this->source = source;
}

Scanner::Scanner() : keywords {
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
    {"while", WHILE}
} {}

std::vector<Token> Scanner::scanTokens() {
    while(!Scanner::isAtEnd()) {
        start = current;
        Scanner::scanToken();
    }
    tokens.push_back(Token(TokenType::_EOF, "", nullptr, line));
    return tokens;
}

void Scanner::scanToken() {
    char c = Scanner::advance();
    switch (c) {
        case '(':
            Scanner::addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            Scanner::addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            Scanner::addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            Scanner::addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            Scanner::addToken(TokenType::COMMA);
            break;
        case '.':
            Scanner::addToken(TokenType::DOT);
            break;
        case '-':
            Scanner::addToken(TokenType::MINUS);
            break;
        case '+':
            Scanner::addToken(TokenType::PLUS);
            break;
        case ';':
            Scanner::addToken(TokenType::SEMICOLON);
            break;
        case '*':
            Scanner::addToken(TokenType::STAR);
            break;
        case '!':
            Scanner::addToken(Scanner::match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            Scanner::addToken(Scanner::match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            Scanner::addToken(Scanner::match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            Scanner::addToken(Scanner::match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (Scanner::match('/')) {
                while (Scanner::peek() != '\n' && !Scanner::isAtEnd()) {
                    Scanner::advance();
                }
            } else {
                Scanner::addToken(SLASH);
            }
            break;
        case ' ':
            break;
        case '\r':
            break;
        case '\t':
            break;
        case '"':
            Scanner::string();
            break;
        default:
            if (Scanner::isDigit(c)) {
                Scanner::number();
            } else if (Scanner::isAlpha(c)) {
                Scanner::identifier();
            } else {
                Lox::error(line, "Unexpected character");
            }
            break;
    }
}

void Scanner::identifier() {
    while (Scanner::isAlphaNumeric(Scanner::peek())) {
        Scanner::advance();
    }

    static std::string text = this->source.substr(start, current);
    TokenType type = keywords[text];

    if (type == NULL) {
        type = TokenType::IDENTIFIER;
    }
    Scanner::addToken(type);
}

void Scanner::number() {
    while (Scanner::isDigit(peek())) {
        Scanner::advance();
    }

    if (Scanner::peek() == '.' && Scanner::isDigit(Scanner::peekNext())) {
        Scanner::advance();
        while (Scanner::isDigit(Scanner::peek())) {
            Scanner::advance();
        }
    }

    Scanner::addToken(TokenType::NUMBER, std::stod(source.substr(start, current))); // todo
}

void Scanner::string() {
    while (Scanner::peek() != '"' && !Scanner::isAtEnd()) {
        if (Scanner::peek() == '\n') {
            line++;
        }
        Scanner::advance();
    }

    if (Scanner::isAtEnd()) {
        Lox::error(line, "Unterminated string");
        return;
    }

    Scanner::advance();
    std::string value = source.substr(start + 1, current - 1);
    Scanner::addToken(STRING, value);
}

bool Scanner::match(char expected) {
    if (Scanner::isAtEnd()) {
        return false;
    }

    if (source.at(current) != expected) {
        return false;
    }
    current++;
    return true;
}

char Scanner::peek() {
    if (Scanner::isAtEnd()) {
        return '\0';
    }
    return source.at(current);
}

char Scanner::peekNext() {
    if (current + 1 >= source.length()) {
        return '\0';
    }
    return source.at(current + 1);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return Scanner::isAlpha(c) || Scanner::isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

char Scanner::advance() {
    return source.at(current++);
}

void Scanner::addToken(TokenType type) {
    Scanner::addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, std::string literal) {  // todo: Object type
    std::string text = source.substr(start, current);
    tokens.push_back(Token(type, text, literal, line));
}

void Scanner::addToken(TokenType type, double literal) {        // TODO: object type
    std::string text = source.substr(start, current);
    tokens.push_back(Token(type, text, std::to_string(literal), line));
}

}  // namespace lox

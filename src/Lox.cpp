#include <string.h>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "ASTPrinter.h"
#include "Expr.h"
#include "Interpreter.h"
#include "Lox.h"
#include "Parser.h"
#include "Resolver.h"
#include "RuntimeError.h"
#include "Scanner.h"
#include "Token.h"


namespace lox {

// static because we were getting:
// error: cannot call member function 'void lox::Lox::runFile()' without object
template <class T>
void Lox<T>::runFile(const std::string& path) {
  try {
    // https://stackoverflow.com/questions/38032800
    std::fstream bytes{path.c_str(), std::ios::binary};
    // https://stackoverflow.com/questions/2602013
    std::stringstream buffer;
    buffer << bytes.rdbuf();
    run(buffer.str());
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return;
  }

  if (hadError) {
    std::exit(1);
  }

  if (hadRuntimeError) {
    std::exit(1);
  }
}


// error: cannot declare member function 'static void lox::Lox::runPrompt()' to
// have static linkage [-fpermissive] same errors for runFile, run
template <class T>
void Lox<T>::runPrompt() {
  std::string input;
  std::cin >> input;
  std::ifstream file(input);

  for (;;) {
    std::cout << "> ";
    std::string line;
    std::getline(file, line);
    // https://stackoverflow.com/questions/462165
    if (line.empty()) {
      break;
    }
    run(line);
    // reseting the flag
    hadError = false;
  }
}


template <class T>
void Lox<T>::run(const std::string& source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();
  for (Token token : tokens) {
    // TODO: check another way (https://stackoverflow.com/questions/45172025)
    std::cout << token;
  }

  lox::parser::Parser<T> parser;
  parser(tokens);
  // lox::expr::Expr<T> expression = parser.parse();
  lox::stmt::Stmt<T> statements = parser.parse();

  // To ensure code has error and we have to return the program
  if (hadError) {
    return;
  }

  lox::Resolver<T> resolver;
  resolver(interpreter);
  resolver.resolve(statements);

  if (hadError) {
    return;
  }

  // std::cout << ASTPrinter().print(expression);
  interpreter.interpret(statements);
}


template <class T>
void Lox<T>::error(const int& line, const std::string& message) {
  report(line, "", message);
}


template <class T>
void Lox<T>::report(
    const int& line,
    const std::string& where,
    const std::string& message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message;
  hadError = true;
}


template <class T>
void Lox<T>::error(const Token& token, const std::string& message) {
  if (token.tokentype() == TokenType::_EOF) {
    report(token.getLine(), " at end", message);
  } else {
    report(token.getLine(), " at '" + token.getLexeme() + "'", message);
  }
}


template <class T>
void Lox<T>::runtimeError(const RuntimeError& error) {
  std::cerr << error.what() << "[" << error.getToken().getLine() << "]";
  hadRuntimeError = true;
  std::exit(1);
}


}  // namespace lox

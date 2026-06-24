#pragma once

#ifndef LOX_H
#define LOX_H

#include <string.h>
#include <cstddef>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

//#include "ASTPrinter.h"
#include "Expr.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Resolver.h"
#include "RuntimeError.h"
#include "Scanner.h"
#include "Stmt.h"
#include "Token.h"


namespace lox {

class Lox {
 private:
  lox::Interpreter interpreter;

 public:
  static inline bool hadError = false;
  static inline bool hadRuntimeError = false;

  void runFile(const std::string& path) {
    try {
      // https://stackoverflow.com/questions/38032800
      std::fstream bytes{path.c_str(), std::ios::in | std::ios::binary};
      // https://stackoverflow.com/questions/2602013
      std::stringstream buffer;
      buffer << bytes.rdbuf();
      run(buffer.str());
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      return;
    }

    if (hadError) {
      std::exit(65);
    }

    if (hadRuntimeError) {
      std::exit(70);
    }
  }

  void runPrompt() {
    for (;;) {
      std::cout << "> ";
      std::string line;
      if (!std::getline(std::cin, line)) {
        break;
      }
      run(line);
      // reseting the flag
      hadError = false;
    }
  }

  void run(const std::string& source) {
    lox::Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    lox::parser::Parser parser(tokens);
    std::vector<std::shared_ptr<lox::stmt::Stmt>> statements = parser.parse();

    // To ensure code has error and we have to return the program
    if (hadError) {
      return;
    }

    lox::Resolver resolver(interpreter);
    resolver.resolve(statements);

    if (hadError) {
      return;
    }

    interpreter.interpret(statements);
  }


  static void error(int line, const std::string& message) {
    report(line, "", message);
  }

  static void report(
      const int& line,
      const std::string& where,
      const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message
              << "\n";
    hadError = true;
  }

  static void error(const Token& token, const std::string& message) {
    if (token.tokentype() == TokenType::_EOF) {
      report(token.getLine(), " at end", message);
    } else {
      report(token.getLine(), " at '" + token.getLexeme() + "'", message);
    }
  }

  static void runtimeError(const RuntimeError& error) {
    std::cerr << error.what() << "\n[line " << error.getToken().getLine()
              << "]\n";
    hadRuntimeError = true;
  }
};


}  // namespace lox


// template class lox::Lox<double>;

#endif

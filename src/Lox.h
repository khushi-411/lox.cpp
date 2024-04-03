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
//#include "Interpreter.h"
//#include "Parser.h"
//#include "Resolver.h"
#include "RuntimeError.h"
#include "Scanner.h"
#include "Stmt.h"
#include "Token.h"


namespace lox {

class Lox {
 private:
  bool hadError = false;
  bool hadRuntimeError = false;
  // static lox::Interpreter interpreter;

 public:
  void runFile(const std::string& path) {
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

  void runPrompt() {
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

  void run(const std::string& source) {
    lox::Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    for (Token token : tokens) {
      // TODO: check another way (https://stackoverflow.com/questions/45172025)
      std::cout << token;
    }

    // lox::parser::Parser parser(tokens);
    // lox::expr::Expr expression = parser.parse();
    //  std::vector<lox::stmt::Stmt> statements = parser.parseStmt();

    // To ensure code has error and we have to return the program
    if (hadError) {
      return;
    }

    // lox::Resolver resolver(interpreter);
    //  resolver.resolve(statements);

    if (hadError) {
      return;
    }

    // std::cout << ASTPrinter().print(expression);
    // interpreter.interpret(statements);
  }


  void error(int line, const std::string& message) {
    report(line, "", message);
  }

  void report(
      const int& line,
      const std::string& where,
      const std::string& message) {
    std::cout << "[line " << line << "] Error" << where << ": " << message;
    hadError = true;
  }

  void error(const Token& token, const std::string& message) {
    if (token.tokentype() == TokenType::_EOF) {
      report(token.getLine(), " at end", message);
    } else {
      report(token.getLine(), " at '" + token.getLexeme() + "'", message);
    }
  }

  void runtimeError(const RuntimeError& error) {
    std::cerr << error.what() << "[" << error.getToken().getLine() << "]";
    hadRuntimeError = true;
    std::exit(1);
  }
};


}  // namespace lox


// template class lox::Lox<double>;

#endif

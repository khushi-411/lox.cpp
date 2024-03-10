#include <string.h>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//#include "Expr.h"
#include "Lox.h"
//#include "Parser.h"
#include "RuntimeError.h"
#include "Scanner.h"
#include "Token.h"


namespace lox {

// static because we were getting:
// error: cannot call member function 'void lox::Lox::runFile()' without object
void Lox::runFile(const std::string& path) {
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
}


// error: cannot declare member function 'static void lox::Lox::runPrompt()' to
// have static linkage [-fpermissive] same errors for runFile, run
void Lox::runPrompt() {
  std::string input;
  std::cin >> input;
  std::ifstream file(input);
  for (;;) {
    std::cout << "> ";
    std::string line;
    std::getline(file, line);
    // https://stackoverflow.com/questions/462165
    if (line.empty())
      break;
    run(line);
    // reseting the flag
    hadError = false;
  }
}


void Lox::run(const std::string& source) {
  // chatgpt
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();
  for (Token token : tokens) {
    // TODO: check another way (https://stackoverflow.com/questions/45172025)
    std::cout << token;
  }

  //    Parser::Parser parser;
  //    parser(tokens);
  //    Expr::Expr expression = parser.parse();

  // To ensure code has error and we have to return the program
  if (hadError) {
    return;
  }

  //    std::cout << ASTPrinter().print(expression);
}


// https://stackoverflow.com/questions/24288855
static void main(int argc, char** argv) {
  try {
    // https://stackoverflow.com/questions/18649547
    if (strlen(argv[0]) > 1) {
      std::cout << "Usage: " << argv[0] << std::endl;
      std::exit(1);
    } else if (strlen(argv[0]) == 1) {
      Lox::runFile(argv[0]);
    } else {
      Lox::runPrompt();
    }
  } catch (const std::exception& e) {
    // TODO: check std::nested_exception, std::throw_with_nested,
    // std::invalid_argument
    // https://stackoverflow.com/questions/8480640/how-to-throw-a-c-exception
    std::cerr << "Exception: " << e.what() << std::endl;
    std::exit(1);
  }
  return;
}


void Lox::error(const int& line, const std::string& message) {
  report(line, "", message);
}


void Lox::report(
    const int& line,
    const std::string& where,
    const std::string& message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message;
  hadError = true;
}


void Lox::error(const Token& token, const std::string& message) {
  if (token.type == TokenType::_EOF) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at '" + token.lexeme + "'", message);
  }
}


void Lox::runtimeError(const RuntimeError& error) {
  std::cerr << error.what() << "[" << error.token.line << "]";
  hadRuntimeError = true;
  std::exit(1);
}


}  // namespace lox

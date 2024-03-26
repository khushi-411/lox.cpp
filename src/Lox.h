#ifndef LOX_H
#define LOX_H

#include <functional>
#include <iostream>
#include <string>

#include "Interpreter.h"
#include "RuntimeError.h"
#include "Token.h"


namespace lox {

template <class T>
class Lox {
 private:
  bool hadError = false;
  bool hadRuntimeError = false;
  Interpreter<T> interpreter;

 public:
  void runFile(const std::string& path);
  void runPrompt(void);
  void run(const std::string& source);
  void error(const int& line, const std::string& message);
  void report(
      const int& line,
      const std::string& where,
      const std::string& message);
  void error(const Token& token, const std::string& message);
  void runtimeError(const RuntimeError& error);
};


}  // namespace lox


#endif

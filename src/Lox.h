#ifndef LOX_H
#define LOX_H

#include "Interpreter.h"
#include "RuntimeError.h"
#include "Token.h"


namespace lox {

template <class T>
class Lox {
 private:
  static bool hadError;
  static bool hadRuntimeError;
  static Interpreter<T> interpreter = new Interpreter();

 public:
  static void runFile(const std::string& path);
  static void runPrompt();
  static void run(const std::string& source);
  static void error(const int& line, const std::string& message);
  static void report(
      const int& line,
      const std::string& where,
      const std::string& message);
  static void error(const Token& token, const std::string& message);
  static void runtimeError(const RuntimeError& error);
};


}  // namespace lox

#endif

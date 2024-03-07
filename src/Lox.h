#ifndef LOX_H
#define LOX_H

#include "RuntimeError.h"
#include "Token.h"


namespace lox {

class Lox {
 private:
 public:
  // public not private
  static bool hadError;
  static bool hadRuntimeError;

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

// bool Lox::hadError = false;
// bool Lox::hadRuntimeError = false;

}  // namespace lox

#endif
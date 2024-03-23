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
  static Interpreter<T> interpreter;

 public:
  void runFile(const std::string& path);
  void runPrompt();
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


template <class T>
bool lox::Lox<T>::hadError = false;


template <class T>
bool lox::Lox<T>::hadRuntimeError = false;


template <class T>
lox::Interpreter<T> lox::Lox<T>::interpreter;


#endif

#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include "Interpreter.h"

namespace Lox {

class LoxCallable {
 public:
  int arity();
  std::string call(
      Interpreter::Interpreter interpreter,
      std::vector<std::string> arguments);
};

}  // namespace Lox

#endif

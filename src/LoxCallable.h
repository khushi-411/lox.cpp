#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include <string>
#include <variant>
#include <vector>

#include "Interpreter.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

class LoxCallable {
 public:
  virtual ~LoxCallable() = default;
  virtual int arity() = 0;
  virtual Object call(
      lox::Interpreter& interpreter,
      const std::vector<Object>& arguments) = 0;
};


}  // namespace lox

#endif

#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include <string>
#include <variant>
#include <vector>

#include "Interpreter.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

template <class T>
class LoxCallable {
 public:
  virtual int arity() = 0;
  virtual Object call(
      const lox::Interpreter<T>& interpreter,
      const std::vector<Object>& arguments) = 0;
};


}  // namespace lox

#endif

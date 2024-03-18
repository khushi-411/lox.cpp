#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include <string>
#include <vector>

#include "Interpreter.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace Lox {

template <class T>
class LoxCallable {
 public:
  int arity();
  Object call(lox::Interpreter<T> interpreter, std::vector<Object> arguments);
};


}  // namespace Lox

#endif

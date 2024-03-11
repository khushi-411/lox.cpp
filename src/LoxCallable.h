#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include "Interpreter.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace Lox {

template <class T>
class LoxCallable {
 public:
  int arity();
  Object call(
      Interpreter<T>::Interpreter interpreter,
      std::vector<Object> arguments);
};


}  // namespace Lox

#endif

#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

#include <string>
#include <variant>
#include <vector>

#include "Environment.h"
#include "Interpreter.h"
#include "LoxCallable.h"
#include "LoxInstance.h"
#include "Stmt.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

template <class T>
class LoxFunction : public LoxCallable<T> {
 private:
  lox::stmt::Stmt<T>::Function declaration;
  Environment closure;
  bool isInitializer;

 public:
  LoxFunction(
      const lox::stmt::Stmt<T>::Function& declaration,
      const Environment& closure,
      const bool& isInitializer);

  LoxFunction<T> bind(const LoxInstance<T>& instance);
  const std::string& to_string() const;
  int arity();
  Object call(
      const Interpreter<T>& interpreter,
      const std::vector<Object>& arguments);
};

}  // namespace lox

#endif

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
class LoxFunction : public LoxCallable {
 private:
  const lox::stmt::Stmt<T>::Function& declaration;
  const Environment::Environment& closure;
  const bool isInitializer;

 public:
  LoxFunction(
      const lox::stmt::Stmt<T>::Function& declaration,
      const Environment::Environment& closure,
      bool isInitializer);

  LoxFunction bind(const LoxInstance& instance);
  const std::string to_string() const;
  int arity();
  Object call(
      const Interpreter<T>& interpreter,
      const std::vector<Object>& arguments);
};

}  // namespace lox

#endif

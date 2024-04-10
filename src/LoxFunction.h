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

class LoxFunction : public LoxCallable {
 private:
  lox::stmt::Function declaration;
  Environment closure;
  bool isInitializer;

 public:
  friend bool operator==(const LoxFunction& _x, const LoxFunction& _y) {
    return _x == _y;
  }

  bool operator==(const std::nullptr_t& _y) const {
    return *this == _y;
  }

  friend bool operator!=(const LoxFunction& _x, const LoxFunction& _y) {
    return _x != _y;
  }

  bool operator!=(const std::nullptr_t& _y) const {
    return !(*this == _y);
  }

  LoxFunction(
      const lox::stmt::Function& declaration,
      const Environment& closure,
      const bool& isInitializer);

  LoxFunction bind(const LoxInstance& instance);
  const std::string& to_string() const;
  int arity();
  Object call(
      const Interpreter& interpreter,
      const std::vector<Object>& arguments);
};

}  // namespace lox

#endif

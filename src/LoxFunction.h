#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Environment.h"
#include "LoxCallable.h"
#include "LoxInstance.h"
#include "Stmt.h"

namespace lox {
class Interpreter;
}  // namespace lox

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {

class LoxFunction : public LoxCallable {
 private:
  lox::stmt::Function declaration;
  Environment closure;
  bool isInitializer;
  bool is_null_ = false;

 public:
  friend bool operator==(const LoxFunction& _x, const LoxFunction& _y) {
    return &_x == &_y;
  }

  bool operator==(const std::nullptr_t&) const {
    return is_null_;
  }

  friend bool operator!=(const LoxFunction& _x, const LoxFunction& _y) {
    return &_x != &_y;
  }

  bool operator!=(const std::nullptr_t&) const {
    return !is_null_;
  }

  LoxFunction(
      const lox::stmt::Function& declaration,
      const Environment& closure,
      const bool& isInitializer);

  LoxFunction bind(const LoxInstance& instance);
  std::string to_string() const;
  int arity();
  Object call(
      Interpreter& interpreter,
      const std::vector<Object>& arguments);
};

}  // namespace lox

#endif

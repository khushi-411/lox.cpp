#ifndef LOXCLASS_H
#define LOXCLASS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "LoxCallable.h"

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {


class LoxFunction;


class LoxClass : public lox::LoxCallable {
 private:
  std::string name;
  const LoxClass* superclass;
  std::unordered_map<std::string, LoxFunction> methods;
  bool is_null_ = false;

 public:
  friend bool operator==(const LoxClass& _x, const LoxClass& _y) {
    return &_x == &_y;
  }

  bool operator==(const std::nullptr_t&) const {
    return is_null_;
  }

  friend bool operator!=(const LoxClass& _x, const LoxClass& _y) {
    return &_x != &_y;
  }

  bool operator!=(const std::nullptr_t&) const {
    return !is_null_;
  }

  LoxClass(
      const std::string& name,
      const LoxClass* superclass,
      const std::unordered_map<std::string, LoxFunction>& methods);

  lox::LoxFunction findMethod(const std::string& name) const;
  std::string to_string();
  Object call(
      lox::Interpreter& interpreter,
      const std::vector<Object>& arguments) override;
  int arity() override;

  const std::string& getName() const;
};


}  // namespace lox

#endif

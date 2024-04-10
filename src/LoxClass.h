#ifndef LOXCLASS_H
#define LOXCLASS_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Interpreter.h"
#include "LoxCallable.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


class LoxFunction;


class LoxClass : public lox::LoxCallable {
 private:
  std::string name;
  const LoxClass& superclass;
  std::unordered_map<std::string, LoxFunction> methods;

 public:
  friend bool operator==(const LoxClass& _x, const LoxClass& _y) {
    return _x == _y;
  }

  bool operator==(const std::nullptr_t& _y) const {
    return *this == _y;
  }

  friend bool operator!=(const LoxClass& _x, const LoxClass& _y) {
    return _x != _y;
  }

  bool operator!=(const std::nullptr_t& _y) const {
    return !(*this == _y);
  }

  LoxClass(
      const std::string& name,
      const LoxClass& superclass,
      const std::unordered_map<std::string, LoxFunction>& methods);

  lox::LoxFunction findMethod(const std::string& name);
  std::string to_string();
  Object call(
      const lox::Interpreter& interpreter,
      const std::vector<Object>& arguments);
  int arity();

  const std::string& getName();
};


}  // namespace lox

#endif

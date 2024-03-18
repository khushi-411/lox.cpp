#ifndef LOXCLASS_H
#define LOXCLASS_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Interpreter.h"
#include "LoxCallable.h"
#include "LoxFunction.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


template <class T>
class LoxClass : public LoxCallable {
 private:
  std::string name;
  LoxClass<T> superclass;
  std::unordered_map<std::string, lox::LoxFunction<T>> methods;

 public:
  LoxClass(
      const std::string& name,
      const LoxClass<T>& superclass,
      const std::unordered_map<std::string, lox::LoxFunction<T>>& methods);

  lox::LoxFunction<T> findMethod(const std::string& name);
  const std::string& to_string() const;
  Object call(
      const lox::Interpreter<T>& interpreter,
      const std::vector<Object>& arguments);
  int arity();

  const std::string& getName() const;
};


}  // namespace lox

#endif

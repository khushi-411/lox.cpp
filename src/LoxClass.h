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


template <class T>
class LoxFunction;


template <class T>
class LoxClass : public lox::LoxCallable<T> {
 private:
  std::string name;
  LoxClass<T>* superclass;
  std::unordered_map<std::string, LoxFunction<T>> methods;

 public:
  LoxClass(
      const std::string& name,
      LoxClass<T>* superclass,
      const std::unordered_map<std::string, LoxFunction<T>>& methods);

  lox::LoxFunction<T> findMethod(const std::string& name);
  std::string to_string();
  Object call(
      const lox::Interpreter<T>& interpreter,
      const std::vector<Object>& arguments);
  int arity();

  const std::string& getName();
};


}  // namespace lox

#endif

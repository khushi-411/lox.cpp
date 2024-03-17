#ifndef LOXCLASS_H
#define LOXCLASS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Interpreter.h"
#include "LoxCallable.h"
#include "LoxFunction.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


template <class T>
class LoxClass : public LoxCallable {
 private:
  const std::string& name;
  const LoxClass<T>& superclass;
  const std::unordered_map<std::string, LoxFunction<T>>& methods;

 public:
  LoxClass(
      const std::string& name,
      const LoxClass<T>& superclass,
      const std::unordered_map<std::string, LoxFunction<T>>& methods);

  LoxFunction<T> findMethod(const std::string& name);
  const std::string to_string() const;
  Object call(
      const Interpreter<T>& interpreter,
      const std::vector<Object>& arguments);
  int arity();
};


}  // namespace lox

#endif

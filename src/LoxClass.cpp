#include <string>
#include <unordered_map>
#include <vector>

#include "Interpreter.h"
#include "LoxCallable.h"
#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


template <class T>
LoxClass<T>::LoxClass(
    const std::string& name,
    const LoxClass<T>& superclass,
    const std::unordered_map<std::string, LoxFunction<T>>& methods)
    : superclass(superclass), name(name), methods(methods) {}


LoxFunction<T>::LoxFunction findMethod(const std::string& name) {
  auto it = methods.find(name);
  if (it != methods.end()) {
    return superclass.findMethod(name);
  }
  return nullptr;
}


const std::string to_string() const {
  return name;
}


template <class T>
Object call(
    const Interpreter<T>& interpreter,
    const std::vector<Object>& arguments) {
  LoxInstance<T> instance = new LoxInstance(*this);
  LoxFunction<T> initializer = LoxClass<T>::findMethod("init");

  if (initializer != nullptr) {
    initializer.bind(instance).call(interpreter, arguments);
  }

  return instance;
}


template <class T>
int arity() {
  LoxFunction<T> initializer = LoxClass<T>::findMethod("init");

  if (initializer == nullptr) {
    return 0;
  }

  return initializer.arity();
}

}  // namespace lox

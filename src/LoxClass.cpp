#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Interpreter.h"
#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


template <class T>
LoxClass<T>::LoxClass(
    const std::string& name,
    LoxClass<T>* superclass,
    const std::unordered_map<std::string, LoxFunction<T>>& methods)
    : superclass(superclass), name(name), methods(methods) {}


template <class T>
LoxFunction<T> LoxClass<T>::findMethod(const std::string& name) {
  auto it = methods.find(name);

  if (it != methods.end()) {
    return methods[name];
  }

  if (superclass != nullptr) {
    return superclass.findMethod(name);
  }
  return nullptr;
}


template <class T>
std::string LoxClass<T>::to_string() {
  return name;
}


template <class T>
Object LoxClass<T>::call(
    const Interpreter<T>& interpreter,
    const std::vector<Object>& arguments) {
  LoxInstance<T>* instance = new LoxInstance<T>(*this);
  LoxFunction<T> initializer = LoxClass<T>::findMethod("init");

  if (initializer != nullptr) {
    initializer.bind(instance).call(interpreter, arguments);
  }

  return instance;
}


template <class T>
int LoxClass<T>::arity() {
  LoxFunction<T> initializer = LoxClass<T>::findMethod("init");

  if (initializer == nullptr) {
    return 0;
  }

  return initializer.arity();
}


template <class T>
const std::string& LoxClass<T>::getName() {
  return name;
}

}  // namespace lox

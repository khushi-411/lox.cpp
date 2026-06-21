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


LoxClass::LoxClass(
    const std::string& name,
    const LoxClass* superclass,
    const std::unordered_map<std::string, LoxFunction>& methods)
    : name(name), superclass(superclass), methods(methods) {}


lox::LoxFunction LoxClass::findMethod(const std::string& name) const {
  auto it = methods.find(name);

  if (it != methods.end()) {
    return methods.at(name);
  }

  if (superclass != nullptr) {
    return superclass->findMethod(name);
  }

  // Return a null LoxFunction
  throw std::runtime_error("Method not found: " + name);
}


std::string LoxClass::to_string() {
  return name;
}


Object LoxClass::call(
    Interpreter& interpreter,
    const std::vector<Object>& arguments) {
  LoxInstance* instance = new LoxInstance(*this);

  try {
    LoxFunction initializer = LoxClass::findMethod("init");
    if (initializer != nullptr) {
      initializer.bind(*instance).call(interpreter, arguments);
    }
  } catch (const std::runtime_error&) {
    // ...
  }

  // TODO: Return instance as Object
  return nullptr;
}


int LoxClass::arity() {
  LoxFunction initializer = LoxClass::findMethod("init");

  if (initializer == nullptr) {
    return 0;
  }

  return initializer.arity();
}


const std::string& LoxClass::getName() const {
  return name;
}

}  // namespace lox

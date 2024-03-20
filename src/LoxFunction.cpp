#include <string>
#include <variant>
#include <vector>

#include "Environment.h"
#include "Interpreter.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Return.h"
#include "Stmt.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


template <class T>
LoxFunction<T>::LoxFunction(
    const lox::stmt::Stmt<T>::Function& declaration,
    const Environment& closure,
    const bool& isInitializer)
    : isInitializer(isInitializer),
      closure(closure),
      declaration(declaration) {}


template <class T>
LoxFunction<T> LoxFunction<T>::bind(const LoxInstance<T>& instance) {
  Environment environment = new Environment(closure);
  environment.define("this", instance);
  return LoxFunction<T>(declaration, environment, isInitializer);
}


template <class T>
const std::string& LoxFunction<T>::to_string() const {
  return "<fn " + declaration.name.getLexeme() + ">";
}


template <class T>
int LoxFunction<T>::arity() {
  return declaration.params.size();
}


template <class T>
Object LoxFunction<T>::call(
    const Interpreter<T>& interpreter,
    const std::vector<Object>& arguments) {
  Environment environment = new Environment(closure);

  for (int i = 0; i < declaration.params.size(); i++) {
    environment.define(
        declaration.params[i].getLexeme(), arguments[i]);  // TODO
  }

  try {
    interpreter.executeBlock(declaration.body, environment);
  } catch (Return& returnValue) {
    if (isInitializer) {
      return closure.getAt(0, "this");
    }
    return returnValue.value;
  }

  if (isInitializer) {
    closure.getAt(0, "this");
  }

  return nullptr;
}


}  // namespace lox

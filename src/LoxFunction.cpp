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
    const Environment::Environment& closure,
    const bool& isInitializer)
    : isInitializer(isInitializer),
      closure(closure),
      declaration(declaration) {}


template <class T>
LoxFunction<T>::LoxFunction bind(const LoxInstance<T> instance) {
  Environment::Environment environment = new Environment(closure);
  environment.define("this", instance);
  return new LoxFunction<T>::LoxFunction(
      declaration, environment, isInitializer);
}


const std::string to_strint() const {
  return "<fn " + declaration.name.lexeme + ">";
}


int arity() {
  return declaration.params.size();
}


template <class T>
Object call(
    const Interpreter<T>& interpreter,
    const std::vector<Object>& arguments) {
  Environment::Environment = new Environment(closure);

  for (int i = 0; i < declaration.params.size(); i++) {
    environment.define(declaration.params[i].lexeme, arguments[i]);  // TODO
  }

  try {
    interpreter.executeBlock(declaration.body, environment);
  } catch (Return::Return returnValue) {
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

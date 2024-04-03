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


LoxFunction::LoxFunction(
    const lox::stmt::Function& declaration,
    const Environment& closure,
    const bool& isInitializer)
    : isInitializer(isInitializer),
      closure(closure),
      declaration(declaration) {}


LoxFunction LoxFunction::bind(const LoxInstance& instance) {
  Environment environment = new Environment(closure);
  environment.define("this", instance);
  return LoxFunction(declaration, environment, isInitializer);
}


const std::string& LoxFunction::to_string() const {
  return "<fn " + declaration.name.getLexeme() + ">";
}


int LoxFunction::arity() {
  return declaration.params.size();
}


Object LoxFunction::call(
    const Interpreter& interpreter,
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

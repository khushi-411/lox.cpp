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


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {


LoxFunction::LoxFunction(
    const lox::stmt::Function& declaration,
    const Environment& closure,
    const bool& isInitializer)
    : declaration(declaration),
      closure(closure),
      isInitializer(isInitializer) {}


LoxFunction LoxFunction::bind(const LoxInstance& instance) {
  Environment environment(closure);
  environment.define("this", std::make_shared<LoxInstance>(instance));
  return LoxFunction(declaration, environment, isInitializer);
}


std::string LoxFunction::to_string() const {
  return "<fn " + declaration.getName().getLexeme() + ">";
}


int LoxFunction::arity() {
  return declaration.getParams().size();
}


Object LoxFunction::call(
    Interpreter& interpreter,
    const std::vector<Object>& arguments) {
  Environment environment(closure);

  for (size_t i = 0; i < declaration.getParams().size(); i++) {
    environment.define(
        declaration.getParams()[i].getLexeme(), arguments[i]);
  }

  try {
    interpreter.executeBlock(declaration.getBody(), environment);
  } catch (Return& returnValue) {
    if (isInitializer) {
      return closure.getAt(0, "this");
    }
    return returnValue.getValue();
  }

  if (isInitializer) {
    return closure.getAt(0, "this");
  }

  return nullptr;
}


}  // namespace lox

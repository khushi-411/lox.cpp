#include <string>
#include <utility>
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
    std::shared_ptr<Environment> closure,
    const bool& isInitializer)
    : declaration(declaration),
      closure(std::move(closure)),
      isInitializer(isInitializer) {}


LoxFunction LoxFunction::bind(const std::shared_ptr<LoxInstance>& instance) {
  auto environment = std::make_shared<Environment>(closure);
  environment->define("this", instance);
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
  auto environment = std::make_shared<Environment>(closure);

  for (size_t i = 0; i < declaration.getParams().size(); i++) {
    environment->define(
        declaration.getParams()[i].getLexeme(), arguments[i]);
  }

  try {
    interpreter.executeBlock(declaration.getBody(), environment);
  } catch (Return& returnValue) {
    if (isInitializer) {
      return closure->getAt(0, "this");
    }
    return returnValue.getValue();
  }

  if (isInitializer) {
    return closure->getAt(0, "this");
  }

  return nullptr;
}


}  // namespace lox

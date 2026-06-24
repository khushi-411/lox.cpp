#include <string>
#include <variant>

#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "RuntimeError.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {

LoxInstance::LoxInstance(const LoxClass& klass) : klass(&klass) {}


Object LoxInstance::get(const Token& name) {
  if (fields.find(name.getLexeme()) != fields.end()) {
    return fields[name.getLexeme()];
  }

  try {
    LoxFunction method = klass->findMethod(name.getLexeme());
    auto bound = std::make_shared<LoxFunction>(method.bind(shared_from_this()));
    return std::static_pointer_cast<LoxCallable>(bound);
  } catch (const std::runtime_error&) {
    // Method not found — fall through to error
  }

  throw RuntimeError(
      name, "Undefined property '" + name.getLexeme() + "'.");
}


void LoxInstance::set(const Token& name, const Object& value) {
  fields[name.getLexeme()] = value;
}


std::string LoxInstance::to_string() const {
  return klass->getName() + " instance";
}


const LoxClass* LoxInstance::getKlass() const {
  return klass;
}

}  // namespace lox

#include <string>
#include <variant>

#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "RuntimeError.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

LoxInstance::LoxInstance(const auto& klass) : klass(klass) {}


Object LoxInstance::get(const Token& name) {
  if (fields.find(name.getLexeme()) != fields.end()) {
    return fields[name.getLexeme()];
  }

  LoxFunction method = klass.findMethod(name.getLexeme());

  if (method != nullptr) {
    return method.bind(*this);
  }

  throw new RuntimeError(
      name, "Undefined property '" + name.getLexeme() + "'.");
}


void LoxInstance::set(const Token& name, const Object& value) {
  fields[name.getLexeme()] = value;
}


const std::string& LoxInstance::to_string() const {
  return klass.getName() + " instance";
}


const Object& LoxInstance::getKlass() const {
  return klass;
}

}  // namespace lox

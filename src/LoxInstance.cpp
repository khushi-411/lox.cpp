#include <string>
#include <variant>

#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "RuntimeError.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

LoxInstance::LoxInstance(const LoxClass& klass) : klass(&klass) {}


Object LoxInstance::get(const Token& name) {
  if (fields.find(name.getLexeme()) != fields.end()) {
    return fields[name.getLexeme()];
  }

  try {
    LoxFunction method = klass->findMethod(name.getLexeme());
    if (method != nullptr) {
      // TODO: Return bound method properly
      // return method.bind(*this);
    }
  } catch (const std::runtime_error&) {
    // Method not found
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

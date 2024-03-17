#include <string>

#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "RuntimeError.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

LocInstance::LoxInstance(const LoxClass::LoxClass& klass) : klass(klass);


Object get(const Token& name) {
  if (fields.find(name.lexeme) != fields.end()) {
    return fields[name.lexeme];
  }

  LoxFunction::LoxFunction method =  // TODO

      if (method != nullptr) {
    return;  // TODO

    throw new RuntimeError::RuntimeError(
        name, "Undefined property '" + name.lexeme + "'.");
  }


  void set(const Token& name, const Object& value) {
    fields[name.lexeme] = value;
  }


  const std::string LoxInstance::to_string() const {
    return klass.name + " instance";
  }


}  // namespace lox

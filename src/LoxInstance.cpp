#include <string>
#include <variant>

#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "RuntimeError.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

template <class T>
LoxInstance<T>::LoxInstance(const lox::LoxClass<T>& klass) : klass(klass) {}


template <class T>
Object LoxInstance<T>::get(const Token& name) {
  if (fields.find(name.getLexeme()) != fields.end()) {
    return fields[name.getLexeme()];
  }

  LoxFunction<T> method = klass.findMethod(name.getLexeme());

  if (method != nullptr) {
    return method.bind(*this);
  }

  throw new RuntimeError::RuntimeError(
      name, "Undefined property '" + name.getLexeme() + "'.");
}


template <class T>
void LoxInstance<T>::set(const Token& name, const Object& value) {
  fields[name.getLexeme()] = value;
}


template <class T>
const std::string& LoxInstance<T>::to_string() const {
  return klass.getName() + " instance";
}


template <class T>
const lox::LoxClass<T>& LoxInstance<T>::getKlass() const {
  return klass;
}

}  // namespace lox

#include <string>
#include <unordered_map>
#include <variant>

#include "Environment.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(const Environment& enclosing) : enclosing(enclosing) {}


Object Environment::get(const Token& name) {
  if (values.find(name.lexeme) != values.end()) {
    return values.find(name.lexeme)->second;
  }

  if (enclosing != nullptr) {
    return enclosing->get(name);
  }

  throw new RuntimeError::RuntimeError(
      name, "Undefined variable '" + name.lexeme + "'.");
}


void Environment::assign(const Token& name, const Object& value) {
  if (values.find(name.lexeme) != values.end()) {
    values[name.lexeme] = value;
    return;
  }

  if (enclosing != nullptr) {
    enclosing->assign(name, value);
    return;
  }

  throw new RuntimeError::RuntimeError(
      name, "Undefined variable '" + name.lexeme + "'.");
}


void Environment::define(const std::string& name, const Object& value) {
  values[name] = value;
}


Environment Environment::ancestor(const int& distance) {
  Environment* environment = this;

  for (int i = 0; i < distance; i++) {
    environment = environment->enclosing;  // TODO
  }

  return *environment;
}


Object Environment::getAt(const int& distance, const std::string& name) {
  return Environment::ancestor(distance).values.get(name);
}


void Environment::assignAt(
    const int& distance,
    const Token& name,
    const Object& value) {
  ancestor(distance).values[name.lexeme] = value;
}


const std::string Environment::to_string() const {
  std::string result = values.to_string();
  if (enclosing != nullptr) {
    result += " -> " + enclosing->to_string();
  }

  return result;
}

}  // namespace lox

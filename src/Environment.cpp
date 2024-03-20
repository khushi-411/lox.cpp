#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

#include "Environment.h"
#include "RuntimeError.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(const Environment& enclosing)
    : enclosing(&enclosing) {}


Object Environment::get(const Token& name) {
  auto it = values.find(name.getLexeme());
  if (it != values.end()) {
    return it->second;
  }

  if (enclosing != nullptr) {
    return Environment::get(name);  // TODO
  }

  throw RuntimeError(name, "Undefined variable '" + name.getLexeme() + "'.");
}


void Environment::assign(const Token& name, const Object& value) {
  auto it = values.find(name.getLexeme());
  if (it != values.end()) {
    values[name.getLexeme()] = value;
    return;
  }

  if (enclosing != nullptr) {
    Environment::assign(name, value);
    return;
  }

  throw RuntimeError(name, "Undefined variable '" + name.getLexeme() + "'.");
}


void Environment::define(const std::string& name, const Object& value) {
  values[name] = value;
}


Environment Environment::ancestor(const int& distance) {
  const Environment* environment = this;

  for (int i = 0; i < distance; i++) {
    environment = (*environment).enclosing;  // TODO
  }

  return *environment;
}


Object Environment::getAt(const int& distance, const std::string& name) {
  return Environment::ancestor(distance).values.at(name);
}


void Environment::assignAt(
    const int& distance,
    const Token& name,
    const Object& value) {
  ancestor(distance).values[name.getLexeme()] = value;
}


std::string object_to_string(const Object& values) {
  if (std::holds_alternative<std::nullptr_t>(values)) {
    return "nullptr";

  } else if (std::holds_alternative<std::string>(values)) {
    return std::get<std::string>(values);

  } else if (std::holds_alternative<double>(values)) {
    return std::to_string(std::get<double>(values));

  } else if (std::holds_alternative<bool>(values)) {
    return std::get<bool>(values) ? "true" : "false";
  }

  return "Encountered unknown data type.";
}


const std::string Environment::to_string() const {
  std::ostringstream result;

  result << "{ ";
  for (const auto& pair : values) {
    result << pair.first << ": " << object_to_string(pair.second) << ", ";
  }
  result << " }";

  if (enclosing != nullptr) {
    result << " -> " + enclosing->to_string();
  }

  return result.str();
}

}  // namespace lox

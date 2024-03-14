#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <string>
#include <unordered_map>
#include <variant>

#include "RuntimeError.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

class Environment {
 private:
  Environment enclosing;
  std::unordered_map<std::string, Object> values;

 public:
  Environment();
  Environment(const Environment& enclosing);

  Object get(const Token& name);
  void assign(const Token& name, const Object& value);
  void define(const std::string& name, const Object& value);
  Environment ancestor(const int& distance);
  Object getAt(const int& distance, const std::string& name);
  void assignAt(const int& distance, const Token& name, const Object& value);
  const std::string to_string() const;
};


}  // namespace lox

#endif

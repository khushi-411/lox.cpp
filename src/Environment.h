#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

#include "RuntimeError.h"
#include "Token.h"

namespace lox {
class LoxCallable;
class LoxInstance;
}  // namespace lox

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

class Environment {
 private:
  const Environment* enclosing;
  std::unordered_map<std::string, Object> values;

 public:
  Environment();
  Environment(const Environment& enclosing);

  Object get(const Token& name) const;
  void assign(const Token& name, const Object& value);
  void define(const std::string& name, const Object& value);
  Environment& ancestor(const int& distance);
  const Environment& ancestor(const int& distance) const;
  Object getAt(const int& distance, const std::string& name) const;
  void assignAt(const int& distance, const Token& name, const Object& value);
  const std::string to_string() const;
};


}  // namespace lox

#endif

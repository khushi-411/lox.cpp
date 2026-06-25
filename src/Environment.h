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
  std::shared_ptr<Environment> enclosing;
  std::unordered_map<std::string, Object> values;

 public:
  Environment();
  explicit Environment(std::shared_ptr<Environment> enclosing);

  Object get(const Token& name) const;
  void assign(const Token& name, const Object& value);
  void define(const std::string& name, const Object& value);
  Environment* ancestor(int distance);
  Object getAt(int distance, const std::string& name);
  void assignAt(int distance, const Token& name, const Object& value);
  const std::string to_string() const;
};


}  // namespace lox

#endif

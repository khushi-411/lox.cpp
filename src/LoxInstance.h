#ifndef LOXINSTAMCE_H
#define LOXINSTANCE_H

#include <string>
#include <unordered_map>

#include "LoxClass.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

class LoxInstance {
 private:
  LoxClass klass;
  std::unordered_map<std::string, Object> fields;

 public:
  LoxInstance(const LoxClass& klass);

  Object get(const Token& name);
  void set(const Token& name, const Object& value);
  const std::string to_string() const;
};


}  // namespace lox

#endif

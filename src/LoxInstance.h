#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

#include "LoxClass.h"
#include "Token.h"

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {

class LoxClass;


class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
 private:
  const LoxClass* klass;
  std::unordered_map<std::string, Object> fields;

 public:
  LoxInstance(const LoxClass& klass);

  Object get(const Token& name);
  void set(const Token& name, const Object& value);
  std::string to_string() const;
  const LoxClass* getKlass() const;
};


}  // namespace lox

#endif

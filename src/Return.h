#ifndef RETURN_H
#define RETURN_H

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

namespace lox {
class LoxCallable;
class LoxInstance;
}  // namespace lox

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {

class Return : public std::runtime_error {
 private:
  Object value;

 public:
  Return(const Object& value);

  const Object& getValue() const {
    return value;
  }
};

}  // namespace lox

#endif

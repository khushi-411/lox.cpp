#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H

#include <memory>
#include <string>
#include <variant>
#include <vector>

// Forward declarations (no Interpreter.h needed — only its type is referenced)
namespace lox {
class Interpreter;
class LoxCallable;
class LoxInstance;
}  // namespace lox

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;


namespace lox {

class LoxCallable {
 public:
  virtual ~LoxCallable() = default;
  virtual int arity() = 0;
  virtual Object call(
      lox::Interpreter& interpreter,
      const std::vector<Object>& arguments) = 0;
};


}  // namespace lox

#endif

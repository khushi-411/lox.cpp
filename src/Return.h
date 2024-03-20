#ifndef RETURN_H
#define RETURN_H

#include <stdexcept>
#include <string>
#include <variant>


using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {

class Return : public std::runtime_error {
 private:
  Object value;

 public:
  Return(const Object& value);
};

}  // namespace lox

#endif

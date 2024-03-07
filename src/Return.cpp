#include <stdexcept>
#include <string>
#include <variant>

#include "Return.h"

typedef std::runtime_error super;


namespace lox {


class Return : public std::runtime_error {
 public:
  const std::variant<std::nullptr_t, std::string, double, bool> value;
  Return(const std::variant<std::nullptr_t, std::string, double, bool>& value);
};


Return::Return(
    const std::variant<std::nullptr_t, std::string, double, bool>& value)
    : super(NULL), value(value) {}  // TODO


}  // namespace lox

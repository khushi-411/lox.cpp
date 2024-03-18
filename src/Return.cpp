#include <stdexcept>
#include <string>
#include <variant>

#include "Return.h"

typedef std::runtime_error super;

using Object = std::variant<std::nullptr_t, std::string, double, bool>;


namespace lox {


class Return : public std::runtime_error {
 private:
  const Object& value;

 public:
  Return(const Object& value);
};


Return::Return(const Object& value) : super(nullptr), value(value) {}  // TODO


}  // namespace lox

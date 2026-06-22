#include <stdexcept>
#include <string>
#include <variant>

#include "Return.h"


typedef std::runtime_error super;

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

Return::Return(const Object& value) : super(nullptr), value(value) {}


}  // namespace lox

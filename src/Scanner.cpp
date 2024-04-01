#include <string.h>
#include <cstddef>
#include <iostream>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Lox.h"
#include "Scanner.h"
#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

// TODO: use auto instead of bool datatype?
// TODO: maybe use static inline function during declaration?
// TODO: use std::unique_ptr, std::shared_ptr etc
// TODO: use constexpr?


}  // namespace lox

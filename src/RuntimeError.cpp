#include "RuntimeError.h"

namespace lox {

RuntimeError::RuntimeError(Token token, std::string message)
    : std::runtime_error(message), token(token);

}  // namespace lox

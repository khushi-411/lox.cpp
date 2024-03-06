#include <stdexcept>
#include <string>

#include "RuntimeError.h"
#include "Token.h"


// https://stackoverflow.com/questions/4424216
typedef std::runtime_error super;


namespace lox {

RuntimeError::RuntimeError(const Token &token, const std::string &message)
    : super(message), token(token) {}

}  // namespace lox

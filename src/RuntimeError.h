#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

#include <stdexcept>
#include <string>

#include "Token.h"

namespace lox {

class RuntimeError : public std::runtime_error {
 public:
  const Token token;
  RuntimeError(const Token& token, const std::string& message);
};

}  // namespace lox


#endif

#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

#include <stdexcept>
#include <string>

#include "Token.h"

namespace lox {

class RuntimeError : public std::runtime_error {
 private:
  Token token;

 public:
  RuntimeError(const Token& token, const std::string& message);

  const Token& getToken() const;
};

}  // namespace lox


#endif

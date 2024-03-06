#ifndef RETURN_H
#define RETURN_H

namespace Lox {

class Return : public std::runtime_error {
 public:
  const std::string value;

  Return::Return(std::string value);
};

}  // namespace Lox

#endif

#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H


namespace lox {

class RuntimeError : public std::runtime_error {

public:
    const Token token;
    RuntimeError(Token token, std::string message);
};

}  // namespace lox


#endif

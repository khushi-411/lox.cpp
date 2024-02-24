#ifndef LOX_H
#define LOX_H

#include "Token.h"

namespace lox {

class Lox {

private:

public:

    // public not private
    static bool hadError;
    static bool hadRuntimeError;

    static void runFile(const std::string& path);
    static void runPrompt();
    static void run(const std::string& source);
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
    static void error(Token token, std::string message);
    // void runtimeError();

};

// bool Lox::hadError = false;
// bool Lox::hadRuntimeError = false;

}  // namespace lox

#endif

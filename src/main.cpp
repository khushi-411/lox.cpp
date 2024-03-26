#include <string.h>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Lox.h"

typedef double T;


int main(int argc, char** argv) {
  lox::Lox<T> _lox;
  try {
    // https://stackoverflow.com/questions/18649547
    if (argc > 2) {
      std::cout << "Usage: " << argv[0] << " [script]\n";
      std::exit(1);
    } else if (argc == 2) {
      _lox.runFile(argv[1]);
    } else {
      _lox.runPrompt();
    }

  } catch (const std::exception& e) {
    // TODO: check std::nested_exception, std::throw_with_nested,
    // std::invalid_argument
    // https://stackoverflow.com/questions/8480640/how-to-throw-a-c-exception
    std::cerr << "Exception: " << e.what() << std::endl;
    std::exit(1);
  }

  return 0;
}

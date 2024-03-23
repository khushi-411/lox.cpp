#include <string.h>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Lox.h"

typedef double T;


int main(int argc, char** argv) {
  try {
    // https://stackoverflow.com/questions/18649547
    if (strlen(argv[0]) > 1) {
      std::cout << "Usage: " << argv[0] << std::endl;
      std::exit(1);
    } else if (strlen(argv[0]) == 1) {
      lox::Lox<T>::runFile(argv[0]);
    } else {
      lox::Lox<T>::runPrompt();
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

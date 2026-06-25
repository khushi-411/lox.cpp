#include <string.h>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Lox.h"

// typedef double T;


int main(int argc, char** argv) {
  lox::Lox _lox;
  try {
    // Parse optional --ast flag
    bool dumpAst = false;
    int scriptArgIdx = -1;

    for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i], "--ast") == 0) {
        dumpAst = true;
      } else {
        scriptArgIdx = i;
      }
    }

    int nonFlagArgs = (scriptArgIdx != -1) ? 1 : 0;
    if (argc - (dumpAst ? 1 : 0) > 2) {
      std::cout << "Usage: " << argv[0] << " [--ast] [script]\n";
      std::exit(1);
    } else if (scriptArgIdx != -1) {
      _lox.runFile(argv[scriptArgIdx], dumpAst);
    } else {
      _lox.runPrompt(dumpAst);
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

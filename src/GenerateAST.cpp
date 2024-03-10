#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Expr.h"
#include "GenerateAST.h"


// pretty printer

namespace lox {


// provides description of each type and its fields

void GenerateAST::defineAST(
    const std::string& outputDir,
    const std::string& baseName,
    const std::vector<std::string>& types) {
  try {
    std::string path = outputDir + "/" + baseName + ".cpp";
    // https://stackoverflow.com/questions/15080231
    std::ofstream writer(path);
    writer << "#include <iostream>\n";
    writer << "#include <string.h>\n";
    writer << "#include <vector>\n\n";
    writer << "namespace lox {\n";
    writer << "\n";
    writer << "class " << baseName << " {\n";
    writer << "public:\n";
    GenerateAST::defineVisitor(baseName, types);
    writer << "};\n";
    writer << "}\n";

    for (std::string type : types) {
      // https://stackoverflow.com/questions/216823
      std::string className = type.substr(0, type.find(":"));
      size_t start = className.find_first_not_of(" \t");
      size_t end = className.find_last_not_of(" \t");
      className = className.substr(start, end - start + 1);

      std::string fields = type.substr(0, type.find(":"));
      size_t start2 = fields.find_first_not_of(" \t");
      size_t end2 = fields.find_last_not_of(" \t");
      fields = fields.substr(start2, end2 - start2 + 1);

      GenerateAST::defineType(baseName, className, fields);
    }

    writer << "\n";
    writer << "template <typename T>";
    writer << "T accept(const Visitor<T>& visitor);";

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return;
  }
}


void GenerateAST::defineType(
    const std::string& baseName,
    const std::string& className,
    const std::string& fieldList) {
  std::cout << "template <class T>\n";
  std::cout << "class " << baseName << " : "
            << "public " << className << " {\n";
  std::cout << "    " << baseName << "(" << fieldList << ") \n";
  std::string fields = fieldList.substr(0, fieldList.find(","));

  // chatgpt
  // TODO: figure out different
  std::istringstream iss(fields);
  std::string token;
  while (std::getline(iss, token, ' ')) {
    if (!token.empty()) {
      std::cout << "  :  this." + token + " = " + token + ";";
    }
  }
  std::cout << "\n";

  std::cout << "    };";
  std::cout << "\n";
  std::cout << "    T accept(const Visitor<T>& visitor) {";
  std::cout << "        return visitor.visit" + className + "(*this);";
  std::cout << "    }";
  std::cout << "};\n";
}


void Beignet::accept(PastryVisitor& visitor) {
  visitor.visitBeignet(*this);
}


void Cruller::accept(PastryVisitor& visitor) {
  visitor.visitCruller(*this);
}


void GenerateAST::defineVisitor(
    const std::string& baseName,
    const std::vector<std::string>& types) {
  // TODO: maybe try with writer
  std::cout << "template <class T>\n";
  std::cout << "class Visitor : public Expr<T> {\n";
  std::cout << " public:\n";

  for (std::string type : types) {
    // https://stackoverflow.com/questions/216823
    std::string typeName = type.substr(0, type.find(":"));
    size_t start = typeName.find_first_not_of(" \t");
    size_t end = typeName.find_last_not_of(" \t");
    typeName = type.substr(start, end - start + 1);

    std::cout << "  virtual T visit" + typeName + baseName + "(const " +
            typeName + "<T>& " + baseName + ") = 0;";
  }

  std::cout << "};";
}


void main(int argc, char** argv) {
  try {
    if (argc != 1) {
      std::cerr << "Usage: generate_ast <output directory>";
      std::exit(1);
    }
    std::string outputDir = argv[0];

    GenerateAST generator;

    std::vector<std::string> expr_types;
    expr_types.push_back("Assign    : Token name, Expr value");
    expr_types.push_back("Binary    : Expr left, Token operator, Expr right");
    expr_types.push_back(
        "Call      : Expr callee, Token paren, std::vector<Expr> arguments");
    expr_types.push_back("Get       : Expr object, Token name");
    expr_types.push_back("Grouping  : Expr expression");
    expr_types.push_back(
        "Literal   : std::string value");  // todo: object in java
    expr_types.push_back("Logical   : Expr left, Token op, Expr right");
    expr_types.push_back("Set       : Expr object, Token name, Expr value");
    expr_types.push_back("Super     : Token keyword, Token method");
    expr_types.push_back("This      : Token keyword");
    expr_types.push_back("Unary     : Token operator, Expr right");
    expr_types.push_back("Variable  : Token name");

    generator.defineAST(outputDir, "Expr", expr_types);

    std::vector<std::string> stmt_types;
    stmt_types.push_back("Block       : std::vector<Stmt> statements");
    // stmt_types.push_back("Class       : Token name, Expr::Variable
    // superclass, std::vector<Stmt::Function> methods");
    stmt_types.push_back("Expression  : Expr expression");
    stmt_types.push_back(
        "Function    : Token name, std::vector<Token> params, std::vector<Stmt> body");
    stmt_types.push_back(
        "If          : Expr condition, Stmt thenBranch, Stmt elseBranch");
    stmt_types.push_back("Print       : Expr expression");
    stmt_types.push_back("Return      : Token keyword, Expr value");
    stmt_types.push_back("Var         : Token name, Expr intializer");
    stmt_types.push_back("While       : Expr condition, Stmt body");

    generator.defineAST(outputDir, "Stmt", stmt_types);

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return;
  }
}


}  // namespace lox

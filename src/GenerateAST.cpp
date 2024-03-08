#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Expr.h"
#include "GenerateAST.h"


namespace lox {

void GenerateAST::defineAST(
    const std::string& outputDir,
    const std::string& baseName,
    const std::vector<std::string>& types) {
  try {
    std::string path = outputDir + "/" + baseName + ".cpp";
    // https://stackoverflow.com/questions/15080231
    std::ofstream writer(path);
    writer << "#include <iostream>\n";
    writer << "#include <vector>\n";
    writer << "namespace lox {\n";
    writer << "\n";
    writer << "class " << baseName << " {\n";
    writer << "public:\n";
    GenerateAST::defineVisitor(baseName, types);
    writer << "};\n";
    writer << "}\n";

    for (std::string type : types) {
      // https://stackoverflow.com/questions/216823
      //  https://stackoverflow.com/questions/216823
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
    writer << "R accept(Visitor<R> visitor);";

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return;
  }
}


void GenerateAST::defineType(
    const std::string& baseName,
    const std::string& className,
    const std::string& fieldList) {
  std::cout << "class " << baseName << " : "
            << "public " << className << " {\n";
  std::cout << "    " << className << "(" << fieldList << ") {\n";
  std::string fields = fieldList.substr(0, fieldList.find(","));

  for (char field : fields) {
    char name = strtok(field, " ");  // field.substr(0, field.find(" "));
    std::cout << "    this." + name + " = " + name + ";";
  }
  std::cout << "\n";

  for (std::string field : fields) {
    std::cout << " const " + field + ";";
  }

  std::cout << "    };";
  std::cout << "\n";
  std::cout << "    R accept(Visitor<R> visitor) {";
  std::cout << "        return visitor.visit" + className + "(this);";
  std::cout << "    }";
  std::cout << "};\n";
}


void Beignet::accept(const PastryVisitor& visitor) {
  visitor.visitBeignet(*this);
}


void Cruller::accept(const PastryVisitor& visitor) {
  visitor.visitCruller(*this);
}


void GenerateAST::defineVisitor(
    const std::string& baseName,
    const std::vector<std::string>& types) {
  std::cout << " virtual Visitor<R> {";

  for (std::string type : types) {
    // https://stackoverflow.com/questions/216823
    std::string typeName = type.substr(0, type.find(":"));
    size_t start = typeName.find_first_not_of(" \t");
    size_t end = typeName.find_last_not_of(" \t");
    typeName = type.substr(start, end - start + 1);

    std::cout << "R visit" + typeName + baseName + "(" + typeName + " " +
            baseName /*.tolower()*/ + ");";
  }
  std::cout << " }";
}


void main(int argc, char** argv) {
  try {
    if (argc != 1) {
      std::cerr << "Usage: generate_ast <output directory>";
      std::exit(1);
    }
    std::string outputDir = argv[0];

    std::vector<std::string> types;
    GenerateAST::defineAST(outputDir, "Expr", types);

    types.push_back("Binary    : Expr left, Token operator, Expr right");
    types.push_back("Grouping  : Expr expression");
    types.push_back("Literal   : std::string value");  // todo: object in java
    types.push_back("Unary     : Token operator, Expr right");

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return;
  }
}

}  // namespace lox

#include <string.h>
#include <vector.h>

#include "GenerateAST.h"

namespace lox {

static void main(std::string args) {
  try {
    if (args.length() != 1) {
      std::cerr << "Usage: generate_ast <output directory>";
      std::exit(1);
    }
    std::string outputDir = args[0];

    std::vector<std::string> types;
    defineAST(outputDir, "Expr", types);

    types.push_back("Binary    : Expr left, Token operator, Expr right");
    types.push_back("Grouping  : Expr expression");
    types.push_back("Literal   : std::string value");  // todo: object in java
    types.push_back("Unary     : Token operator, Expr right");

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
}

static void GenerateAST::defineAST(
    std::string outputDir,
    std::string baseName,
    std::vector<std::string> types) {
  try {
    std::string path = outputDir + "/" + baseName + ".cpp";
    std::ofstream writer(path);
    writer << "#include <iostream>\n";
    writer << "#include <vector>\n";
    writer << "namespace lox {\n";
    writer << "\n";
    writer << "class " << baseName << " {\n";
    writer << "public:\n";
    writer << "};\n";
    writer << "}\n";

    for (std::string type : types) {
      std::string className = ;  // TODO
      std::string fields = ;  // TODO
      defineType(writer, baseName, className, fields);
    }
    writer << "\n";
    writer << "R accept(Visitor<R> visitor);";
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
}

static void GenerateAST::defineType(
    /*todo*/ writer,
    std::string baseName,
    std::string className,
    std::string fieldList) {
  std::cout << "class " << baseName << " : "
            << "public " << className << " {\n";
  writer << "    " << className << "(" << fieldList << ") {\n";
  std::vector<std::string> fields = strtok(fieldList, ", ");
  for (std::string field : fields) {
    std::string name = strtok(filed, " ");
    writer << "    this." + name + " = " + name + ";";
  }
  writer << "\n";
  for (std::string field : fields) {
    writer << " const " + field + ";";
  }
  writer << "    };";
  writer << "\n";
  writer << "    R accept(Visitor<R> visitor) {";
  writer << "        return visitor.visit" + className + "(this);";
  writer << "    }";
  std::cout << "};\n";
}

Pastry::Beignet::accept(PastryVisitor visitor) {
  visitor.visitBeignet(this);
}

Pastry::Cruller::accept(PastryVisitor visitor) {
  visitor.visitCruller(this);
}

static void GenerateAST::defineVisitor(
    /*todo type*/ writer,
    std::string baseName,
    std::vector<std::string> types) {
  std::cout << " virtual Visitor<R> {";
  for (std::string type : types) {
    // https://stackoverflow.com/questions/216823
    std::string typeName = type.split(":")[0]  // TODO
                           std::cout
        << "R visit" + typeName + baseName + "(" + typeName + " " +
            baseName.tolower() + ");";
  }
  std::cout << " }";
}

}  // namespace lox

#ifndef GENERATEAST_H
#define GENERATEAST_H

namespace lox {

class GenerateAST {
 public:
  static void main(std::string args);
  static void defineAST(
      std::string outputDir,
      std::string baseName,
      std::vector<std::string> types);
  static void defineType(
      /*todo*/ writer,
      std::string baseName,
      std::string className,
      std::string fieldList);
  static void defineVisitor(
      /*todo type*/ writer,
      std::string baseName,
      std::vector<std::string> types);
};

class Pastry {
 public:
  void visitBeignet(Beignet beignet);
  void visitCruller(Cruller cruller);
  void accept(PastryVisitor visitor);
};

class Beignet : public Pastry {
 public:
  void accept(PastryVisitor visitor);
};

class Cruller : class Pastry {
 public:
  void accept(PastryVisitor visitor);
};

}  // namespace lox

#endif

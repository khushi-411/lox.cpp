#ifndef GENERATEAST_H
#define GENERATEAST_H

#include <string>
#include <vector>


namespace lox {


class GenerateAST {
 public:
  void defineAST(
      const std::string& outputDir,
      const std::string& baseName,
      const std::vector<std::string>& types);
  void defineType(
      const std::string& baseName,
      const std::string& className,
      const std::string& fieldList);
  void defineVisitor(
      const std::string& baseName,
      const std::vector<std::string>& types);
};


class Beignet;
class Cruller;


class PastryVisitor {
 public:
  virtual void visitBeignet(const Beignet& beignet) = 0;
  virtual void visitCruller(const Cruller& cruller) = 0;
};


class Pastry {
 public:
  virtual void accept(PastryVisitor& visitor) = 0;
};


class Beignet : public Pastry {
 public:
  void accept(PastryVisitor& visitor);
};


class Cruller : public Pastry {
 public:
  void accept(PastryVisitor& visitor);
};


}  // namespace lox

#endif

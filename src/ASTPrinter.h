#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <string.h>

#include "Expr.h"

namespace lox {

class ASTPrinter {};

class Expr::Visitor<std::string> {
  std::string print(Expr expr);

 public:
  std::string visitBinaryExpr(Expr::Binary expr);
  std::string visitGroupingExpr(Expr::Grouping expr);
  std::string visitLiteralExpr(Expr::Literal expr);
  std::string visitUnaryExpr(Expr::Unary expr);

 private:
  template <typename T, typename... Expr>
  std::string parenthesize(std::string name, Expr... exprs);
};

}  // namespace lox

#endif

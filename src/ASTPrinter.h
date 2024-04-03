#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <string>
#include <variant>

#include "Expr.h"
#include "Stmt.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {


class ASTPrinter : public lox::expr::Visitor<std::string>,
                   lox::stmt::Visitor<std::string> {
 public:
  std::string visitAssignExpr(const lox::expr::Assign& _expr);
  std::string visitBinaryExpr(const lox::expr::Binary& _expr);
  std::string visitCallExpr(const lox::expr::Call& _expr);
  std::string visitGetExpr(const lox::expr::Get& _expr);
  std::string visitGroupingExpr(const lox::expr::Grouping& _expr);
  std::string visitLiteralExpr(const lox::expr::Literal& _expr);
  std::string visitLogicalExpr(const lox::expr::Logical& _expr);
  std::string visitSetExpr(const lox::expr::Set& _expr);
  std::string visitSuperExpr(const lox::expr::Super& _expr);
  std::string visitThisExpr(const lox::expr::This& _expr);
  std::string visitUnaryExpr(const lox::expr::Unary& _expr);
  std::string visitVariableExpr(const lox::expr::Variable& _expr);

  std::string visitBlockStmt(const lox::stmt::Block& _stmt);
  std::string visitClassStmt(const lox::stmt::Class& _stmt);
  std::string visitExpressionStmt(const lox::stmt::Expression& _stmt);
  std::string visitFunctionStmt(const lox::stmt::Function& _stmt);
  std::string visitIfStmt(const lox::stmt::If& _stmt);
  std::string visitPrintStmt(const lox::stmt::Print& _stmt);
  std::string visitReturnStmt(const lox::stmt::Return& _stmt);
  std::string visitVarStmt(const lox::stmt::Var& _stmt);
  std::string visitWhileStmt(const lox::stmt::While& _stmt);

  std::string print(const lox::expr::Expr& _expr);
  std::string print(const lox::stmt::Stmt& _stmt);
  std::string parenthesize(
      const std::string& name,
      const lox::expr::Expr& exprs);
  std::string parenthesize2(const std::string& name, const Object& exprs);
};


}  // namespace lox

#endif

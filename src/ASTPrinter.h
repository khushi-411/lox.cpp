#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <string>
#include <variant>

#include "Expr.h"
#include "Stmt.h"


namespace lox {


template <class T>
class ASTPrinter : public lox::expr::Visitor<std::string>,
                   lox::stmt::Visitor<std::string> {
 public:
  std::string visitAssignExpr(const lox::expr::Assign<T>& _expr);
  std::string visitBinaryExpr(const lox::expr::Binary<T>& _expr);
  std::string visitCallExpr(const lox::expr::Call<T>& _expr);
  std::string visitGetExpr(const lox::expr::Get<T>& _expr);
  std::string visitGroupingExpr(const lox::expr::Grouping<T>& _expr);
  std::string visitLiteralExpr(const lox::expr::Literal<T>& _expr);
  std::string visitLogicalExpr(const lox::expr::Logical<T>& _expr);
  std::string visitSetExpr(const lox::expr::Set<T>& _expr);
  std::string visitSuperExpr(const lox::expr::Super<T>& _expr);
  std::string visitThisExpr(const lox::expr::This<T>& _expr);
  std::string visitUnaryExpr(const lox::expr::Unary<T>& _expr);
  std::string visitVariableExpr(const lox::expr::Variable<T>& _expr);

  std::string visitBlockStmt(const lox::stmt::Block<T>& _stmt);
  std::string visitClassStmt(const lox::stmt::Class<T>& _stmt);
  std::string visitExpressionStmt(const lox::stmt::Expression<T>& _stmt);
  std::string visitFunctionStmt(const lox::stmt::Function<T>& _stmt);
  std::string visitIfStmt(const lox::stmt::If<T>& _stmt);
  std::string visitPrintStmt(const lox::stmt::Print<T>& _stmt);
  std::string visitReturnStmt(const lox::stmt::Return<T>& _stmt);
  std::string visitVarStmt(const lox::stmt::Var<T>& _stmt);
  std::string visitWhileStmt(const lox::stmt::While<T>& _stmt);

  std::string print(const lox::expr::Expr<T>& _expr);
  std::string print(const lox::stmt::Stmt<T>& _stmt);
  std::string parenthesize(
      const std::string& name,
      const lox::expr::Expr<T>& exprs);
  std::string parenthesize2(
      const std::string& name,
      const std::variant<std::nullptr_t, std::string, double, bool>& exprs);
};


}  // namespace lox

#endif

#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <string>
#include <variant>

#include "Expr.h"
#include "Stmt.h"


namespace lox {


template <class T>
class ASTPrinter : public lox::expr::Expr<T>::Visitor<std::string>,
                   lox::stmt::Stmt<T>::Visitor<std::string> {
 public:
  std::string visitAssignExpr(const lox::expr::Expr<T>::Assign& _expr);
  std::string visitBinaryExpr(const lox::expr::Expr<T>::Binary& _expr);
  std::string visitCallExpr(const lox::expr::Expr<T>::Call& _expr);
  std::string visitGetExpr(const lox::expr::Expr<T>::Get& _expr);
  std::string visitGroupingExpr(const lox::expr::Expr<T>::Grouping& _expr);
  std::string visitLiteralExpr(const lox::expr::Expr<T>::Literal& _expr);
  std::string visitLogicalExpr(const lox::expr::Expr<T>::Logical& _expr);
  std::string visitSetExpr(const lox::expr::Expr<T>::Set& _expr);
  std::string visitSuperExpr(const lox::expr::Expr<T>::Super& _expr);
  std::string visitThisExpr(const lox::expr::Expr<T>::This& _expr);
  std::string visitUnaryExpr(const lox::expr::Expr<T>::Unary& _expr);
  std::string visitVariableExpr(const lox::expr::Expr<T>::Variable& _expr);

  std::string visitBlockStmt(const lox::stmt::Stmt<T>::Block& _stmt);
  std::string visitClassStmt(const lox::stmt::Stmt<T>::Class& _stmt);
  std::string visitExpressionStmt(const lox::stmt::Stmt<T>::Expression& _stmt);
  std::string visitFunctionStmt(const lox::stmt::Stmt<T>::Function& _stmt);
  std::string visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt);
  std::string visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt);
  std::string visitReturnStmt(const lox::stmt::Stmt<T>::Return& _stmt);
  std::string visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt);
  std::string visitWhileStmt(const lox::stmt::Stmt<T>::While& _stmt);

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

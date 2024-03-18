#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <vector>

#include "Environment.h"
#include "Expr.h"
#include "Stmt.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

template <class T>
class Interpreter : public lox::expr::Expr<T>::Visitor<Object>,
                    public lox::stmt::Stmt<T>::Visitor<void> {
 public:
  void visitBlockStmt(const lox::stmt::Stmt<T>::Block& _stmt);
  // void visitClassStmt(const lox::stmt::Stmt<T>::Class& _stmt);
  void visitExpressionStmt(const lox::stmt::Stmt<T>::Expression& _stmt);
  void visitFunctionStmt(const lox::stmt::Stmt<T>::Function& _stmt);
  void visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt);
  void visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt);
  void visitReturnStmt(const lox::stmt::Stmt<T>::Return& _stmt);
  void visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt);
  void visitWhileStmt(const lox::stmt::Stmt<T>::While& _stmt);

  void interpret(const std::vector<lox::stmt::Stmt<T>>& statements);
  void execute(const lox::stmt::Stmt<T>& _stmt);
  void evaluate(const lox::stmt::Stmt<T>& _stmt);
  void resolve(const lox::expr::Expr<T>& _expr, const int& depth);
  void executeBlock(
      const std::vector<lox::stmt::Stmt<T>>& statements,
      const Environment& environment);

  Object visitAssignExpr(const lox::expr::Expr<T>::Assign& _expr);
  Object visitBinaryExpr(const lox::expr::Expr<T>::Binary& _expr);
  Object visitCallExpr(const lox::expr::Expr<T>::Call& _expr);
  Object visitGetExpr(const lox::expr::Expr<T>::Get& _expr);
  Object visitGroupingExpr(const lox::expr::Expr<T>::Grouping& _expr);
  Object visitLiteralExpr(const lox::expr::Expr<T>::Literal& _expr);
  Object visitLogicalExpr(const lox::expr::Expr<T>::Logical& _expr);
  Object visitSetExpr(const lox::expr::Expr<T>::Set& _expr);
  Object visitSuperExpr(const lox::expr::Expr<T>::Super& _expr);
  Object visitThisExpr(const lox::expr::Expr<T>::This& _expr);
  Object visitUnaryExpr(const lox::expr::Expr<T>::Unary& _expr);
  Object visitVariableExpr(const lox::expr::Expr<T>::Variable& _expr);

  Object evaluate(const lox::expr::Expr<T>& _expr);

  void checkNumberOperand(const Token& op, const std::string& operand);
  bool isTruthy(const std::string& object);
  void checkNumberOperands(
      const Token& op,
      const std::string& left,
      const std::string& right);
  bool isEqual(const std::string& a, const std::string& b);

  void interpret(const lox::expr::Expr<T>& expression);
  std::string stringify(const std::string& object);
};


}  // namespace lox

#endif

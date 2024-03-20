#ifndef RESOLVER_H
#define RESOLVER_H

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "Expr.h"
#include "Interpreter.h"
#include "Stmt.h"


namespace lox {

enum FunctionType {
  NONE,
  FUNCTION,
  INITIALIZER,
  METHOD,
};


enum ClassType {
  _NONE,
  _CLASS,
  SUBCLASS,
};


template <class T>
class Resolver : public lox::expr::Expr<T>::Visitor<void>,
                 public lox::stmt::Stmt<T>::Visitor<void> {
 private:
  lox::Interpreter<T> interpreter;
  std::stack<std::unordered_map<std::string, bool>> scopes;
  FunctionType currentFunction = FunctionType::NONE;
  ClassType currentClass = ClassType::_NONE;

 public:
  Resolver(const lox::Interpreter<T>& interpreter);
  void resolve(const std::vector<lox::stmt::Stmt<T>>& statements);

  void visitBlockStmt(const lox::stmt::Stmt<T>::Block& _stmt);
  void visitClassStmt(const lox::stmt::Stmt<T>::Class& _stmt);
  void visitExpressionStmt(const lox::stmt::Stmt<T>::Expression& _stmt);
  void visitFunctionStmt(const lox::stmt::Stmt<T>::Function& _stmt);
  void visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt);
  void visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt);
  void visitReturnStmt(const lox::stmt::Stmt<T>::Return& _stmt);
  void visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt);
  void visitWhileStmt(const lox::stmt::Stmt<T>::While& _stmt);

  void visitAssignExpr(const lox::expr::Expr<T>::Assign& _expr);
  void visitBinaryExpr(const lox::expr::Expr<T>::Binary& _expr);
  void visitCallExpr(const lox::expr::Expr<T>::Call& _expr);
  void visitGetExpr(const lox::expr::Expr<T>::Get& _expr);
  void visitGroupingExpr(const lox::expr::Expr<T>::Grouping& _expr);
  void visitLiteralExpr(const lox::expr::Expr<T>::Literal& _expr);
  void visitLogicalExpr(const lox::expr::Expr<T>::Logical& _expr);
  void visitSetExpr(const lox::expr::Expr<T>::Set& _expr);
  void visitSuperExpr(const lox::expr::Expr<T>::Super& _expr);
  void visitThisExpr(const lox::expr::Expr<T>::This& _expr);
  void visitUnaryExpr(const lox::expr::Expr<T>::Unary& _expr);
  void visitVariableExpr(const lox::expr::Expr<T>::Variable& _expr);

  void resolve(const lox::stmt::Stmt<T>& _stmt);
  void resolve(const lox::expr::Expr<T>& _expr);
  void resolveFunction(
      const lox::stmt::Stmt<T>::Function& function,
      const FunctionType& type);
  void beginScope();
  void endScope();
  void declare(const Token& name);
  void define(const Token& name);
  void resolveLocal(const lox::expr::Expr<T>& _expr, const Token& name);
};

}  // namespace lox

#endif

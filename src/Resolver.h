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
class Resolver : public lox::expr::Visitor<void>,
                 public lox::stmt::Visitor<void> {
 private:
  lox::Interpreter<T> interpreter;
  std::stack<std::unordered_map<std::string, bool>> scopes;
  FunctionType currentFunction = FunctionType::NONE;
  ClassType currentClass = ClassType::_NONE;

 public:
  Resolver(const lox::Interpreter<T>& interpreter);
  void resolve(const std::vector<lox::stmt::Stmt<T>>& statements);

  void visitBlockStmt(const lox::stmt::Block<T>& _stmt);
  void visitClassStmt(const lox::stmt::Class<T>& _stmt);
  void visitExpressionStmt(const lox::stmt::Expression<T>& _stmt);
  void visitFunctionStmt(const lox::stmt::Function<T>& _stmt);
  void visitIfStmt(const lox::stmt::If<T>& _stmt);
  void visitPrintStmt(const lox::stmt::Print<T>& _stmt);
  void visitReturnStmt(const lox::stmt::Return<T>& _stmt);
  void visitVarStmt(const lox::stmt::Var<T>& _stmt);
  void visitWhileStmt(const lox::stmt::While<T>& _stmt);

  void visitAssignExpr(const lox::expr::Assign<T>& _expr);
  void visitBinaryExpr(const lox::expr::Binary<T>& _expr);
  void visitCallExpr(const lox::expr::Call<T>& _expr);
  void visitGetExpr(const lox::expr::Get<T>& _expr);
  void visitGroupingExpr(const lox::expr::Grouping<T>& _expr);
  void visitLiteralExpr(const lox::expr::Literal<T>& _expr);
  void visitLogicalExpr(const lox::expr::Logical<T>& _expr);
  void visitSetExpr(const lox::expr::Set<T>& _expr);
  void visitSuperExpr(const lox::expr::Super<T>& _expr);
  void visitThisExpr(const lox::expr::This<T>& _expr);
  void visitUnaryExpr(const lox::expr::Unary<T>& _expr);
  void visitVariableExpr(const lox::expr::Variable<T>& _expr);

  void resolve(const lox::stmt::Stmt<T>& _stmt);
  void resolve(const lox::expr::Expr<T>& _expr);
  void resolveFunction(
      const lox::stmt::Function<T>& function,
      const FunctionType& type);
  void beginScope();
  void endScope();
  void declare(const Token& name);
  void define(const Token& name);
  void resolveLocal(const lox::expr::Expr<T>& _expr, const Token& name);
};

}  // namespace lox

#endif

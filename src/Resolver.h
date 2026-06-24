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


class Resolver : public lox::expr::ExprVisitor,
                 public lox::stmt::StmtVisitor {
 private:
  const lox::Interpreter& interpreter;
  std::stack<std::unordered_map<std::string, bool>> scopes;
  FunctionType currentFunction = FunctionType::NONE;
  ClassType currentClass = ClassType::_NONE;

 public:
  Resolver(const lox::Interpreter& interpreter);
  void resolve(const std::vector<std::shared_ptr<lox::stmt::Stmt>>& statements);

  void visitBlockStmt(const lox::stmt::Block& _stmt) override;
  void visitClassStmt(const lox::stmt::Class& _stmt) override;
  void visitExpressionStmt(const lox::stmt::Expression& _stmt) override;
  void visitFunctionStmt(const lox::stmt::Function& _stmt) override;
  void visitIfStmt(const lox::stmt::If& _stmt) override;
  void visitPrintStmt(const lox::stmt::Print& _stmt) override;
  void visitReturnStmt(const lox::stmt::Return& _stmt) override;
  void visitVarStmt(const lox::stmt::Var& _stmt) override;
  void visitWhileStmt(const lox::stmt::While& _stmt) override;

  Object visitAssignExpr(const lox::expr::Assign& _expr) override;
  Object visitBinaryExpr(const lox::expr::Binary& _expr) override;
  Object visitCallExpr(const lox::expr::Call& _expr) override;
  Object visitGetExpr(const lox::expr::Get& _expr) override;
  Object visitGroupingExpr(const lox::expr::Grouping& _expr) override;
  Object visitLiteralExpr(const lox::expr::Literal& _expr) override;
  Object visitLogicalExpr(const lox::expr::Logical& _expr) override;
  Object visitSetExpr(const lox::expr::Set& _expr) override;
  Object visitSuperExpr(const lox::expr::Super& _expr) override;
  Object visitThisExpr(const lox::expr::This& _expr) override;
  Object visitUnaryExpr(const lox::expr::Unary& _expr) override;
  Object visitVariableExpr(const lox::expr::Variable& _expr) override;

  void resolve(const std::shared_ptr<lox::stmt::Stmt>& _stmt);
  void resolve(const std::shared_ptr<lox::expr::Expr>& _expr);
  void resolveFunction(
      const lox::stmt::Function& function,
      const FunctionType& type);
  void beginScope();
  void endScope();
  void declare(const Token& name);
  void define(const Token& name);
  void resolveLocal(const lox::expr::Expr& _expr, const Token& name);

  lox::Interpreter& getInterpreter() {
    return const_cast<lox::Interpreter&>(interpreter);
  }
};

}  // namespace lox

// template class lox::Resolver<double>;

#endif

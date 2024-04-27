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


class Resolver : public lox::expr::Visitor<void>,
                 public lox::stmt::Visitor<void> {
 private:
  const lox::Interpreter& interpreter;
  std::stack<std::unordered_map<std::string, bool>> scopes;
  FunctionType currentFunction = FunctionType::NONE;
  ClassType currentClass = ClassType::_NONE;

 public:
  Resolver(const lox::Interpreter& interpreter);
  void resolve(const std::vector<lox::stmt::Stmt>& statements);

  void visitBlockStmt(const lox::stmt::Block& _stmt);
  //  void visitClassStmt(const lox::stmt::Class& _stmt);
  void visitExpressionStmt(const lox::stmt::Expression& _stmt);
  //  void visitFunctionStmt(const lox::stmt::Function& _stmt);
  void visitIfStmt(const lox::stmt::If& _stmt);
  void visitPrintStmt(const lox::stmt::Print& _stmt);
  void visitReturnStmt(const lox::stmt::Return& _stmt);
  void visitVarStmt(const lox::stmt::Var& _stmt);
  void visitWhileStmt(const lox::stmt::While& _stmt);

  void visitAssignExpr(const lox::expr::Assign& _expr);
  void visitBinaryExpr(const lox::expr::Binary& _expr);
  //  void visitCallExpr(const lox::expr::Call& _expr);
  //  void visitGetExpr(const lox::expr::Get& _expr);
  void visitGroupingExpr(const lox::expr::Grouping& _expr);
  void visitLiteralExpr(const lox::expr::Literal& _expr);
  void visitLogicalExpr(const lox::expr::Logical& _expr);
  //  void visitSetExpr(const lox::expr::Set& _expr);
  //  void visitSuperExpr(const lox::expr::Super& _expr);
  //  void visitThisExpr(const lox::expr::This& _expr);
  void visitUnaryExpr(const lox::expr::Unary& _expr);
  void visitVariableExpr(const lox::expr::Variable& _expr);

  void resolve(const lox::stmt::Stmt& _stmt);
  void resolve(const lox::expr::Expr& _expr);
  //  void resolveFunction(
  //      const lox::stmt::Function& function,
  //      const FunctionType& type);
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

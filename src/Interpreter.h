#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <string.h>
#include <map>
#include <vector>

#include "Environment.h"
#include "Expr.h"
#include "Stmt.h"

// Forward declarations for extended Object type
namespace lox {
class LoxCallable;
class LoxInstance;
}  // namespace lox

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

class Interpreter : public lox::expr::ExprVisitor,
                    public lox::stmt::StmtVisitor {
 private:
  std::shared_ptr<Environment> globals;
  std::shared_ptr<Environment> environment;
  std::map<const lox::expr::Expr*, int> locals;

 public:
  Interpreter()
      : globals(std::make_shared<Environment>()), environment(globals) {}

  void visitBlockStmt(const lox::stmt::Block& _stmt) override;
  void visitClassStmt(const lox::stmt::Class& _stmt) override;
  void visitExpressionStmt(const lox::stmt::Expression& _stmt) override;
  void visitFunctionStmt(const lox::stmt::Function& _stmt) override;
  void visitIfStmt(const lox::stmt::If& _stmt) override;
  void visitPrintStmt(const lox::stmt::Print& _stmt) override;
  void visitReturnStmt(const lox::stmt::Return& _stmt) override;
  void visitVarStmt(const lox::stmt::Var& _stmt) override;
  void visitWhileStmt(const lox::stmt::While& _stmt) override;

  void interpret(const std::vector<std::shared_ptr<lox::stmt::Stmt>>& statements);
  void execute(const std::shared_ptr<lox::stmt::Stmt>& _stmt);
  void resolve(const lox::expr::Expr& _expr, int depth);
  void executeBlock(
      const std::vector<std::shared_ptr<lox::stmt::Stmt>>& statements,
      std::shared_ptr<Environment> environment);
  Object lookUpVariable(const Token& name, const lox::expr::Expr& _expr);

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

  Object evaluate(const std::shared_ptr<lox::expr::Expr>& _expr);

  void checkNumberOperand(const Token& op, const Object& operand);
  bool isTruthy(const Object& object);
  void checkNumberOperands(
      const Token& op,
      const Object& left,
      const Object& right);
  bool isEqual(const Object& a, const Object& b);

  std::string stringify(const Object& object);
};


}  // namespace lox

#endif

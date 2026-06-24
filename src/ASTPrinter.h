#pragma once

#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <memory>
#include <string>
#include <variant>

#include "Expr.h"
#include "Stmt.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {


class ASTPrinter : public lox::expr::ExprVisitor,
                   public lox::stmt::StmtVisitor {
 public:
  std::string print(const std::shared_ptr<lox::expr::Expr>& _expr);
  std::string print(const std::shared_ptr<lox::stmt::Stmt>& _stmt);

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

  void visitBlockStmt(const lox::stmt::Block& _stmt) override;
  void visitClassStmt(const lox::stmt::Class& _stmt) override;
  void visitExpressionStmt(const lox::stmt::Expression& _stmt) override;
  void visitFunctionStmt(const lox::stmt::Function& _stmt) override;
  void visitIfStmt(const lox::stmt::If& _stmt) override;
  void visitPrintStmt(const lox::stmt::Print& _stmt) override;
  void visitReturnStmt(const lox::stmt::Return& _stmt) override;
  void visitVarStmt(const lox::stmt::Var& _stmt) override;
  void visitWhileStmt(const lox::stmt::While& _stmt) override;

 private:
  std::string result;

  std::string stringify(const std::shared_ptr<lox::expr::Expr>& _expr);
  std::string stringify(const std::shared_ptr<lox::stmt::Stmt>& _stmt);
  std::string literalToString(const Object& value);
};


}  // namespace lox

#endif

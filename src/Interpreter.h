#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <unordered_map>
#include <vector>

#include "Environment.h"
#include "Expr.h"
#include "Stmt.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

class Interpreter : public lox::expr::Visitor<Object>,
                    public lox::stmt::Visitor<void> {
 private:
  Environment globals;  // = new Environment();
  Environment environment;  // = globals;
  std::unordered_map<lox::expr::Expr, int> locals;

 public:
  // Interpreter();

  void visitBlockStmt(const lox::stmt::Block& _stmt);
  void visitClassStmt(const lox::stmt::Class& _stmt);
  void visitExpressionStmt(const lox::stmt::Expression& _stmt);
  void visitFunctionStmt(const lox::stmt::Function& _stmt);
  void visitIfStmt(const lox::stmt::If& _stmt);
  void visitPrintStmt(const lox::stmt::Print& _stmt);
  void visitReturnStmt(const lox::stmt::Return& _stmt);
  void visitVarStmt(const lox::stmt::Var& _stmt);
  void visitWhileStmt(const lox::stmt::While& _stmt);

  void interpret(const std::vector<lox::stmt::Stmt>& statements);
  void execute(const lox::stmt::Stmt& _stmt);
  // void evaluate(const lox::stmt::Stmt& _stmt);
  void resolve(const lox::expr::Expr& _expr, const int& depth);
  void executeBlock(
      const std::vector<lox::stmt::Stmt>& statements,
      const Environment& environment);
  Object lookUpVariable(const Token& name, const lox::expr::Expr& _expr);

  Object visitAssignExpr(const lox::expr::Assign& _expr);
  Object visitBinaryExpr(const lox::expr::Binary& _expr);
  Object visitCallExpr(const lox::expr::Call& _expr);
  Object visitGetExpr(const lox::expr::Get& _expr);
  Object visitGroupingExpr(const lox::expr::Grouping& _expr);
  Object visitLiteralExpr(const lox::expr::Literal& _expr);
  Object visitLogicalExpr(const lox::expr::Logical& _expr);
  Object visitSetExpr(const lox::expr::Set& _expr);
  Object visitSuperExpr(const lox::expr::Super& _expr);
  Object visitThisExpr(const lox::expr::This& _expr);
  Object visitUnaryExpr(const lox::expr::Unary& _expr);
  Object visitVariableExpr(const lox::expr::Variable& _expr);

  Object evaluate(const lox::expr::Expr& _expr);

  void checkNumberOperand(const Token& op, const std::string& operand);
  bool isTruthy(const std::string& object);
  void checkNumberOperands(
      const Token& op,
      const std::string& left,
      const std::string& right);
  bool isEqual(const std::string& a, const std::string& b);

  void interpret(const lox::expr::Expr& expression);
  std::string stringify(const std::string& object);
};


}  // namespace lox

#endif

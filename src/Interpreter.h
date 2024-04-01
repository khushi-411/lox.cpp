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

template <class T>
class Interpreter : public lox::expr::Visitor<Object>,
                    public lox::stmt::Visitor<void> {
 private:
  Environment globals;  // = new Environment();
  Environment environment;  // = globals;
  std::unordered_map<lox::expr::Expr<T>, int> locals;

 public:
  // Interpreter();

  void visitBlockStmt(const lox::stmt::Block<T>& _stmt);
  void visitClassStmt(const lox::stmt::Class<T>& _stmt);
  void visitExpressionStmt(const lox::stmt::Expression<T>& _stmt);
  void visitFunctionStmt(const lox::stmt::Function<T>& _stmt);
  void visitIfStmt(const lox::stmt::If<T>& _stmt);
  void visitPrintStmt(const lox::stmt::Print<T>& _stmt);
  void visitReturnStmt(const lox::stmt::Return<T>& _stmt);
  void visitVarStmt(const lox::stmt::Var<T>& _stmt);
  void visitWhileStmt(const lox::stmt::While<T>& _stmt);

  void interpret(const std::vector<lox::stmt::Stmt<T>>& statements);
  void execute(const lox::stmt::Stmt<T>& _stmt);
  // void evaluate(const lox::stmt::Stmt<T>& _stmt);
  void resolve(const lox::expr::Expr<T>& _expr, const int& depth);
  void executeBlock(
      const std::vector<lox::stmt::Stmt<T>>& statements,
      const Environment& environment);
  Object lookUpVariable(const Token& name, const lox::expr::Expr<T>& _expr);

  Object visitAssignExpr(const lox::expr::Assign<T>& _expr);
  Object visitBinaryExpr(const lox::expr::Binary<T>& _expr);
  Object visitCallExpr(const lox::expr::Call<T>& _expr);
  Object visitGetExpr(const lox::expr::Get<T>& _expr);
  Object visitGroupingExpr(const lox::expr::Grouping<T>& _expr);
  Object visitLiteralExpr(const lox::expr::Literal<T>& _expr);
  Object visitLogicalExpr(const lox::expr::Logical<T>& _expr);
  Object visitSetExpr(const lox::expr::Set<T>& _expr);
  Object visitSuperExpr(const lox::expr::Super<T>& _expr);
  Object visitThisExpr(const lox::expr::This<T>& _expr);
  Object visitUnaryExpr(const lox::expr::Unary<T>& _expr);
  Object visitVariableExpr(const lox::expr::Variable<T>& _expr);

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

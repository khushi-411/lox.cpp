#include <memory>
#include <sstream>
#include <string>
#include <variant>

#include "ASTPrinter.h"
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

// convert tree to the string

namespace lox {


std::string ASTPrinter::print(const std::shared_ptr<lox::expr::Expr>& _expr) {
  return ASTPrinter::stringify(_expr);
}


std::string ASTPrinter::print(const std::shared_ptr<lox::stmt::Stmt>& _stmt) {
  return ASTPrinter::stringify(_stmt);
}


// dispatch helpers — accept() writes into result, which we return

std::string ASTPrinter::stringify(
    const std::shared_ptr<lox::expr::Expr>& _expr) {
  if (_expr == nullptr) {
    return "nil";
  }
  _expr->accept(*this);
  return result;
}


std::string ASTPrinter::stringify(
    const std::shared_ptr<lox::stmt::Stmt>& _stmt) {
  if (_stmt == nullptr) {
    return "nil";
  }
  _stmt->accept(*this);
  return result;
}


std::string ASTPrinter::literalToString(const Object& value) {
  if (std::holds_alternative<std::string>(value)) {
    return std::get<std::string>(value);
  }
  if (std::holds_alternative<bool>(value)) {
    return std::get<bool>(value) ? "true" : "false";
  }
  if (std::holds_alternative<double>(value)) {
    std::ostringstream oss;
    oss << std::get<double>(value);
    return oss.str();
  }
  return "nil";
}


// assign expr

Object ASTPrinter::visitAssignExpr(const lox::expr::Assign& _expr) {
  result = "(= " + _expr.getName().getLexeme() + " " +
      stringify(_expr.getValue()) + ")";
  return nullptr;
}


// binary expr

Object ASTPrinter::visitBinaryExpr(const lox::expr::Binary& _expr) {
  result = "(" + _expr.getOp().getLexeme() + " " +
      stringify(_expr.getLeft()) + " " + stringify(_expr.getRight()) + ")";
  return nullptr;
}


// call expr

Object ASTPrinter::visitCallExpr(const lox::expr::Call& _expr) {
  std::string out = "(call " + stringify(_expr.getCallee());
  for (const auto& argument : _expr.getArguments()) {
    out += " " + stringify(argument);
  }
  out += ")";
  result = out;
  return nullptr;
}


// get expr

Object ASTPrinter::visitGetExpr(const lox::expr::Get& _expr) {
  result =
      "(. " + stringify(_expr.getObject()) + " " +
      _expr.getName().getLexeme() + ")";
  return nullptr;
}


// grouping expr

Object ASTPrinter::visitGroupingExpr(const lox::expr::Grouping& _expr) {
  result = "(group " + stringify(_expr.getExpression()) + ")";
  return nullptr;
}


// literal expr

Object ASTPrinter::visitLiteralExpr(const lox::expr::Literal& _expr) {
  result = literalToString(_expr.getValue());
  return nullptr;
}


// logical expr

Object ASTPrinter::visitLogicalExpr(const lox::expr::Logical& _expr) {
  result = "(" + _expr.getOp().getLexeme() + " " +
      stringify(_expr.getLeft()) + " " + stringify(_expr.getRight()) + ")";
  return nullptr;
}


// set expr

Object ASTPrinter::visitSetExpr(const lox::expr::Set& _expr) {
  result =
      "(= " + stringify(_expr.getObject()) + " " +
      _expr.getName().getLexeme() + " " + stringify(_expr.getValue()) + ")";
  return nullptr;
}


// super expr

Object ASTPrinter::visitSuperExpr(const lox::expr::Super& _expr) {
  result = "(super " + _expr.getMethod().getLexeme() + ")";
  return nullptr;
}


// this expr

Object ASTPrinter::visitThisExpr(const lox::expr::This& _expr) {
  result = "this";
  return nullptr;
}


// unary expr

Object ASTPrinter::visitUnaryExpr(const lox::expr::Unary& _expr) {
  result =
      "(" + _expr.getOp().getLexeme() + " " + stringify(_expr.getRight()) + ")";
  return nullptr;
}


// variable expr

Object ASTPrinter::visitVariableExpr(const lox::expr::Variable& _expr) {
  result = _expr.getName().getLexeme();
  return nullptr;
}


// block stmt

void ASTPrinter::visitBlockStmt(const lox::stmt::Block& _stmt) {
  std::string out = "(block";
  for (const auto& statement : _stmt.getStatements()) {
    out += " " + stringify(statement);
  }
  out += ")";
  result = out;
}


// class stmt

void ASTPrinter::visitClassStmt(const lox::stmt::Class& _stmt) {
  std::string out = "(class " + _stmt.getName().getLexeme();

  if (_stmt.getSuperclass() != nullptr) {
    out += " < " + stringify(_stmt.getSuperclass());
  }

  for (const auto& method : _stmt.getMethods()) {
    out += " " + stringify(method);
  }

  out += ")";
  result = out;
}


// expression stmt

void ASTPrinter::visitExpressionStmt(const lox::stmt::Expression& _stmt) {
  result = "(; " + stringify(_stmt.getExpression()) + ")";
}


// function stmt

void ASTPrinter::visitFunctionStmt(const lox::stmt::Function& _stmt) {
  std::string out = "(fun " + _stmt.getName().getLexeme() + " (";

  bool first = true;
  for (const auto& param : _stmt.getParams()) {
    if (!first) {
      out += " ";
    }
    out += param.getLexeme();
    first = false;
  }
  out += ")";

  for (const auto& body : _stmt.getBody()) {
    out += " " + stringify(body);
  }

  out += ")";
  result = out;
}


// if stmt

void ASTPrinter::visitIfStmt(const lox::stmt::If& _stmt) {
  if (_stmt.getElseBranch() == nullptr) {
    result = "(if " + stringify(_stmt.getCondition()) + " " +
        stringify(_stmt.getThenBranch()) + ")";
    return;
  }

  result = "(if-else " + stringify(_stmt.getCondition()) + " " +
      stringify(_stmt.getThenBranch()) + " " +
      stringify(_stmt.getElseBranch()) + ")";
}


// print stmt

void ASTPrinter::visitPrintStmt(const lox::stmt::Print& _stmt) {
  result = "(print " + stringify(_stmt.getExpression()) + ")";
}


// return stmt

void ASTPrinter::visitReturnStmt(const lox::stmt::Return& _stmt) {
  if (_stmt.getValue() == nullptr) {
    result = "(return)";
    return;
  }

  result = "(return " + stringify(_stmt.getValue()) + ")";
}


// var stmt

void ASTPrinter::visitVarStmt(const lox::stmt::Var& _stmt) {
  if (_stmt.getInitializer() == nullptr) {
    result = "(var " + _stmt.getName().getLexeme() + ")";
    return;
  }

  result = "(var " + _stmt.getName().getLexeme() + " = " +
      stringify(_stmt.getInitializer()) + ")";
}


// while stmt

void ASTPrinter::visitWhileStmt(const lox::stmt::While& _stmt) {
  result = "(while " + stringify(_stmt.getCondition()) + " " +
      stringify(_stmt.getBody()) + ")";
}


}  // namespace lox

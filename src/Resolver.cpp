#include <deque>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "Expr.h"
#include "Interpreter.h"
#include "Lox.h"
#include "Resolver.h"
#include "Stmt.h"


namespace lox {

Resolver::Resolver(const lox::Interpreter& interpreter)
    : interpreter(interpreter) {}


void lox::Resolver::resolve(
    const std::vector<std::shared_ptr<lox::stmt::Stmt>>& statements) {
  for (const auto& statement : statements) {
    lox::Resolver::resolve(statement);
  }
}


// block stmt

void lox::Resolver::visitBlockStmt(const lox::stmt::Block& _stmt) {
  lox::Resolver::beginScope();
  lox::Resolver::resolve(_stmt.getStatements());
  lox::Resolver::endScope();
  return;
}


// class stmt

void lox::Resolver::visitClassStmt(const lox::stmt::Class& _stmt) {
  ClassType enclosingClass = currentClass;
  currentClass = ClassType::_CLASS;

  lox::Resolver::declare(_stmt.getName());
  lox::Resolver::define(_stmt.getName());

  if (_stmt.getSuperclass() != nullptr &&
      (_stmt.getName().getLexeme() ==
       _stmt.getSuperclass()->getName().getLexeme())) {
    Lox::error(
        _stmt.getSuperclass()->getName(), "A class can't inherit from itself.");
  }

  if (_stmt.getSuperclass() != nullptr) {
    currentClass = ClassType::SUBCLASS;
    lox::Resolver::resolve(_stmt.getSuperclass());
  }

  if (_stmt.getSuperclass() != nullptr) {
    lox::Resolver::beginScope();
    scopes.top()["super"] = true;
  }

  lox::Resolver::beginScope();
  scopes.top()["this"] = true;

  for (const auto& method : _stmt.getMethods()) {
    FunctionType declaration = FunctionType::METHOD;

    if (method->getName().getLexeme() == "init") {
      declaration = FunctionType::INITIALIZER;
    }
    lox::Resolver::resolveFunction(*method, declaration);
  }

  lox::Resolver::endScope();

  if (_stmt.getSuperclass() != nullptr) {
    lox::Resolver::endScope();
  }

  currentClass = enclosingClass;
  return;
}


// expression stmt

void lox::Resolver::visitExpressionStmt(const lox::stmt::Expression& _stmt) {
  lox::Resolver::resolve(_stmt.getExpression());
  return;
}


// function stmt

void lox::Resolver::visitFunctionStmt(const lox::stmt::Function& _stmt) {
  lox::Resolver::declare(_stmt.getName());
  lox::Resolver::define(_stmt.getName());

  lox::Resolver::resolveFunction(_stmt, FunctionType::FUNCTION);
  return;
}


// if stmt

void lox::Resolver::visitIfStmt(const lox::stmt::If& _stmt) {
  lox::Resolver::resolve(_stmt.getCondition());
  lox::Resolver::resolve(_stmt.getThenBranch());

  if (_stmt.getElseBranch() != nullptr) {
    lox::Resolver::resolve(_stmt.getElseBranch());
  }

  return;
}


// print stmt

void lox::Resolver::visitPrintStmt(const lox::stmt::Print& _stmt) {
  lox::Resolver::resolve(_stmt.getExpression());
  return;
}


// return stmt

void lox::Resolver::visitReturnStmt(const lox::stmt::Return& _stmt) {
  if (currentFunction == FunctionType::NONE) {
    Lox::error(_stmt.getKeyword(), "Can't return from top-level code.");
  }

  if (_stmt.getValue() != nullptr) {
    if (currentFunction == FunctionType::INITIALIZER) {
      Lox::error(
          _stmt.getKeyword(), "Can't return a value from an initializer.");
    }
    lox::Resolver::resolve(_stmt.getValue());
  }

  return;
}


// var stmt

void lox::Resolver::visitVarStmt(const lox::stmt::Var& _stmt) {
  lox::Resolver::declare(_stmt.getName());

  if (_stmt.getInitializer() != nullptr) {
    lox::Resolver::resolve(_stmt.getInitializer());
  }

  lox::Resolver::define(_stmt.getName());
  return;
}


// while stmt

void lox::Resolver::visitWhileStmt(const lox::stmt::While& _stmt) {
  lox::Resolver::resolve(_stmt.getCondition());
  lox::Resolver::resolve(_stmt.getBody());
  return;
}


// assign expr

Object lox::Resolver::visitAssignExpr(const lox::expr::Assign& _expr) {
  lox::Resolver::resolve(_expr.getValue());
  lox::Resolver::resolveLocal(_expr, _expr.getName());
  return nullptr;
}


// binary expr

Object lox::Resolver::visitBinaryExpr(const lox::expr::Binary& _expr) {
  lox::Resolver::resolve(_expr.getLeft());
  lox::Resolver::resolve(_expr.getRight());
  return nullptr;
}


// call expr

Object lox::Resolver::visitCallExpr(const lox::expr::Call& _expr) {
  lox::Resolver::resolve(_expr.getCallee());

  for (const auto& argument : _expr.getArguments()) {
    lox::Resolver::resolve(argument);
  }

  return nullptr;
}


// get expr

Object lox::Resolver::visitGetExpr(const lox::expr::Get& _expr) {
  lox::Resolver::resolve(_expr.getObject());
  return nullptr;
}


// grouping expr

Object lox::Resolver::visitGroupingExpr(const lox::expr::Grouping& _expr) {
  lox::Resolver::resolve(_expr.getExpression());
  return nullptr;
}


// literal expr

Object lox::Resolver::visitLiteralExpr(const lox::expr::Literal& _expr) {
  return nullptr;
}


// logical expr

Object lox::Resolver::visitLogicalExpr(const lox::expr::Logical& _expr) {
  lox::Resolver::resolve(_expr.getLeft());
  lox::Resolver::resolve(_expr.getRight());
  return nullptr;
}


// set expr

Object lox::Resolver::visitSetExpr(const lox::expr::Set& _expr) {
  lox::Resolver::resolve(_expr.getValue());
  lox::Resolver::resolve(_expr.getObject());
  return nullptr;
}


// super expr

Object lox::Resolver::visitSuperExpr(const lox::expr::Super& _expr) {
  if (currentClass == ClassType::_NONE) {
    Lox::error(_expr.getKeyword(), "Can't use 'super' outside of a class.");

  } else if (currentClass != ClassType::SUBCLASS) {
    Lox::error(
        _expr.getKeyword(), "Can't use 'super' in a class with no superclass.");
  }

  lox::Resolver::resolveLocal(_expr, _expr.getKeyword());
  return nullptr;
}


// this expr

Object lox::Resolver::visitThisExpr(const lox::expr::This& _expr) {
  if (currentClass == ClassType::_NONE) {
    Lox::error(_expr.getKeyword(), "Can't use 'this' outside of a class.");
    return nullptr;
  }

  lox::Resolver::resolveLocal(_expr, _expr.getKeyword());
  return nullptr;
}


// unary expr

Object lox::Resolver::visitUnaryExpr(const lox::expr::Unary& _expr) {
  lox::Resolver::resolve(_expr.getRight());
  return nullptr;
}


// variable expr

Object lox::Resolver::visitVariableExpr(const lox::expr::Variable& _expr) {
  if (!scopes.empty()) {
    auto& scope = scopes.top();
    auto it = scope.find(_expr.getName().getLexeme());
    if (it != scope.end() && it->second == false) {
      Lox::error(
          _expr.getName(), "Can't read local variable in its own initializer.");
    }
  }

  lox::Resolver::resolveLocal(_expr, _expr.getName());
  return nullptr;
}


// resolving the lists of statements

void lox::Resolver::resolve(const std::shared_ptr<lox::stmt::Stmt>& _stmt) {
  _stmt->accept(*this);
}


// resolving the expressions

void lox::Resolver::resolve(const std::shared_ptr<lox::expr::Expr>& _expr) {
  _expr->accept(*this);
}


void lox::Resolver::resolveFunction(
    const lox::stmt::Function& function,
    const lox::FunctionType& type) {
  FunctionType enclosingFunction = currentFunction;
  currentFunction = type;

  lox::Resolver::beginScope();

  for (Token param : function.getParams()) {
    lox::Resolver::declare(param);
    lox::Resolver::define(param);
  }

  lox::Resolver::resolve(function.getBody());
  lox::Resolver::endScope();

  currentFunction = enclosingFunction;
}


// to create new block scope

void lox::Resolver::beginScope() {
  scopes.push(std::unordered_map<std::string, bool>());
}


// exiting from stack

void lox::Resolver::endScope() {
  scopes.pop();
}


void lox::Resolver::declare(const Token& name) {
  if (scopes.empty()) {
    return;
  }

  auto& scope = scopes.top();

  if (scope[name.getLexeme()]) {
    Lox _lox;
    _lox.error(name, "Already a variable with this name in this scope.");
  }

  scope[name.getLexeme()] = false;
}


void lox::Resolver::define(const Token& name) {
  if (scopes.empty()) {
    return;
  }
  scopes.top()[name.getLexeme()] = true;
}


void lox::Resolver::resolveLocal(
    const lox::expr::Expr& _expr,
    const Token& name) {
  std::stack<std::unordered_map<std::string, bool>> temp = scopes;

  for (int i = scopes.size() - 1; i >= 0; i--) {
    auto& scope = temp.top();
    auto it = scope.find(name.getLexeme());
    if (it != scope.end()) {
      getInterpreter().resolve(_expr, scopes.size() - 1 - i);
      return;
    }
    temp.pop();
  }
}

}  // namespace lox

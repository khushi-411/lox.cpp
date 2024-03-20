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

template <class T>
Resolver<T>::Resolver(const lox::Interpreter<T>& interpreter)
    : interpreter(interpreter) {}


template <class T>
void lox::Resolver<T>::resolve(
    const std::vector<lox::stmt::Stmt<T>>& statements) {
  for (lox::stmt::Stmt<T> statement : statements) {
    lox::Resolver<T>::resolve(statement);
  }
}


// block stmt

template <class T>
void lox::Resolver<T>::visitBlockStmt(const lox::stmt::Stmt<T>::Block& _stmt) {
  lox::Resolver<T>::beginScope();
  lox::Resolver<T>::resolve(_stmt.statements);
  lox::Resolver<T>::endScope();
  return;
}


// class stmt

template <class T>
void lox::Resolver<T>::visitClassStmt(const lox::stmt::Stmt<T>::Class& _stmt) {
  ClassType enclosingClass = currentClass;
  currentClass = ClassType::_CLASS;

  lox::Resolver<T>::declare(_stmt.name);
  lox::Resolver<T>::define(_stmt.name);

  if (_stmt.superclass != nullptr &&
      (_stmt.name.getLexeme() == _stmt.superclass.name.getLexeme())) {
    Lox::error(_stmt.superclass.name, "A class can't inherit from itself.");
  }

  if (_stmt.superclass != nullptr) {
    currentClass = ClassType::SUBCLASS;
    lox::Resolver<T>::resolve(_stmt.superclass);
  }

  if (_stmt.superclass != nullptr) {
    lox::Resolver<T>::beginScope();
    scopes.top()["super"] = true;
  }

  lox::Resolver<T>::beginScope();
  scopes.top()["this"] = true;

  for (typename lox::stmt::Stmt<T>::Function method : _stmt.methods) {
    FunctionType declaration = FunctionType::METHOD;

    if (method.name.getLexeme() == "init") {
      declaration = FunctionType::INITIALIZER;
    }
    lox::Resolver<T>::resolveFunction(method, declaration);
  }

  lox::Resolver<T>::endScope();

  if (_stmt.superclass != nullptr) {
    lox::Resolver<T>::endScope();
  }

  currentClass = enclosingClass;
  return;
}


// expression stmt

template <class T>
void lox::Resolver<T>::visitExpressionStmt(
    const lox::stmt::Stmt<T>::Expression& _stmt) {
  lox::Resolver<T>::resolve(_stmt.expression);
  return;
}


// function stmt

template <class T>
void lox::Resolver<T>::visitFunctionStmt(
    const lox::stmt::Stmt<T>::Function& _stmt) {
  lox::Resolver<T>::declare(_stmt.name);
  lox::Resolver<T>::define(_stmt.name);

  lox::Resolver<T>::resolveFunction(_stmt, FunctionType::FUNCTION);
  return;
}


// if stmt

template <class T>
void lox::Resolver<T>::visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt) {
  lox::Resolver<T>::resolve(_stmt.condition);
  lox::Resolver<T>::resolve(_stmt.thenBranch);

  if (_stmt.elseBranch != nullptr) {
    lox::Resolver<T>::resolve(_stmt.elseBranch);
  }

  return;
}


// print stmt

template <class T>
void lox::Resolver<T>::visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt) {
  lox::Resolver<T>::resolve(_stmt.expression);
  return;
}


// return stmt

template <class T>
void lox::Resolver<T>::visitReturnStmt(
    const lox::stmt::Stmt<T>::Return& _stmt) {
  if (currentFunction == FunctionType::NONE) {
    Lox::error(_stmt.keyword, "Can't return from top-level code.");
  }

  if (_stmt.value != nullptr) {
    if (currentFunction == FunctionType::INITIALIZER) {
      Lox::error(_stmt.keyword, "Can't return a value from an initializer.");
    }
    lox::Resolver<T>::resolve(_stmt.value);
  }

  return;
}


// var stmt

template <class T>
void lox::Resolver<T>::visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt) {
  lox::Resolver<T>::declare(_stmt.name);

  if (_stmt.initializer != nullptr) {
    lox::Resolver<T>::resolve(_stmt.initializer);
  }

  lox::Resolver<T>::define(_stmt.name);
  return;
}


// while stmt

template <class T>
void lox::Resolver<T>::visitWhileStmt(const lox::stmt::Stmt<T>::While& _stmt) {
  lox::Resolver<T>::resolve(_stmt.condition);
  lox::Resolver<T>::resolver(_stmt.body);
  return;
}


// assign expr

template <class T>
void lox::Resolver<T>::visitAssignExpr(
    const lox::expr::Expr<T>::Assign& _expr) {
  lox::Resolver<T>::resolve(_expr.value);
  lox::Resolver<T>::resolveLocal(_expr, _expr.name);
  return;
}


// binary expr

template <class T>
void lox::Resolver<T>::visitBinaryExpr(
    const lox::expr::Expr<T>::Binary& _expr) {
  lox::Resolver<T>::resolve(_expr.left);
  lox::Resolver<T>::resolve(_expr.right);
  return;
}


// call expr

template <class T>
void lox::Resolver<T>::visitCallExpr(const lox::expr::Expr<T>::Call& _expr) {
  lox::Resolver<T>::resolve(_expr.callee);

  for (lox::expr::Expr<T> argument : _expr.arguments) {
    lox::Resolver<T>::resolve(argument);
  }

  return;
}


// get expr

template <class T>
void lox::Resolver<T>::visitGetExpr(const lox::expr::Expr<T>::Get& _expr) {
  lox::Resolver<T>::resolve(_expr.object);
  return;
}


// grouping expr

template <class T>
void lox::Resolver<T>::visitGroupingExpr(
    const lox::expr::Expr<T>::Grouping& _expr) {
  lox::Resolver<T>::resolve(_expr.expression);
  return;
}


// literal expr

template <class T>
void lox::Resolver<T>::visitLiteralExpr(
    const lox::expr::Expr<T>::Literal& _expr) {
  return;
}


// logical expr

template <class T>
void lox::Resolver<T>::visitLogicalExpr(
    const lox::expr::Expr<T>::Logical& _expr) {
  lox::Resolver<T>::resolve(_expr.left);
  lox::Resolver<T>::resolve(_expr.right);
  return;
}


// set expr

template <class T>
void lox::Resolver<T>::visitSetExpr(const lox::expr::Expr<T>::Set& _expr) {
  lox::Resolver<T>::resolve(_expr.value);
  lox::Resolver<T>::resolve(_expr.object);
  return;
}


// super expr

template <class T>
void lox::Resolver<T>::visitSuperExpr(const lox::expr::Expr<T>::Super& _expr) {
  if (currentClass == ClassType::_NONE) {
    Lox::error(_expr.keyword, "Can't use 'super' outside of a class.");

  } else if (currentClass != ClassType::SUBCLASS) {
    Lox::error(
        _expr.keyword, "Can't use 'super' in a class with no superclass.");
  }

  lox::Resolver<T>::resolveLocal(_expr, _expr.keyword);
  return;
}


// this expr

template <class T>
void lox::Resolver<T>::visitThisExpr(const lox::expr::Expr<T>::This& _expr) {
  if (currentClass == ClassType::_NONE) {
    Lox::error(_expr.keyword, "Can't use 'this' outside of a class.");
    return;
  }

  lox::Resolver<T>::resolverLocal(_expr, _expr.keyword);
  return;
}


// unary expr

template <class T>
void lox::Resolver<T>::visitUnaryExpr(const lox::expr::Expr<T>::Unary& _expr) {
  lox::Resolver<T>::resolve(_expr.right);
  return;
}


// variable expr

template <class T>
void lox::Resolver<T>::visitVariableExpr(
    const lox::expr::Expr<T>::Variable& _expr) {
  if (!scopes.empty() && scopes.top()[_expr.name.getLexeme()] == false) {
    Lox::error(_expr.name, "Can't read local variable in its own initializer.");
  }

  lox::Resolver<T>::resolveLocal(_expr, _expr.name);
  return;
}


// resolving the lists of statements

template <class T>
void lox::Resolver<T>::resolve(const lox::stmt::Stmt<T>& _stmt) {
  _stmt.accept(*this);
}


// resolving the expressions

template <class T>
void lox::Resolver<T>::resolve(const lox::expr::Expr<T>& _expr) {
  _expr.accept(*this);
}


template <class T>
void lox::Resolver<T>::resolveFunction(
    const lox::stmt::Stmt<T>::Function& function,
    const lox::FunctionType& type) {
  FunctionType enclosingFunction = currentFunction;
  currentFunction = type;

  lox::Resolver<T>::beginScope();

  for (Token param : function.params) {
    lox::Resolver<T>::declare(param);
    lox::Resolver<T>::define(param);
  }

  lox::Resolver<T>::resolve(function.body);
  lox::Resolver<T>::endScope();

  currentFunction = enclosingFunction;
}


// to create new block scope

template <class T>
void lox::Resolver<T>::beginScope() {
  scopes.push(std::unordered_map<std::string, bool>());
}


// exiting from stack

template <class T>
void lox::Resolver<T>::endScope() {
  scopes.pop();
}


template <class T>
void lox::Resolver<T>::declare(const Token& name) {
  if (scopes.empty()) {
    return;
  }

  std::unordered_map<std::string, bool> scope = scopes.top();

  if (scope[name.getLexeme()]) {
    Lox::error(name, "Already a variable with this name in this scope.");
  }

  scope[name.getLexeme()] = false;
}


template <class T>
void lox::Resolver<T>::define(const Token& name) {
  if (scopes.empty()) {
    return;
  }
  scopes.top()[name.getLexeme()] = true;
}


template <class T>
void lox::Resolver<T>::resolveLocal(
    const lox::expr::Expr<T>& _expr,
    const Token& name) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    auto& scope = scopes.top();
    auto it = scope.find(name.getLexeme());
    if (it != scope.end()) {
      Interpreter<T>::interpreter.resolve(_expr, scopes.size() - 1 - i);
      return;
    }
  }
}

}  // namespace lox

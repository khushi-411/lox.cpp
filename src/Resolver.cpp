#include <string>
#include <unordered_map>
#include <vector>

#include "Expr.h"
#include "Interpreter.h"
#include "Resolver.h"
#include "Stmt.h"


namespace lox {

Resolver::Resolver(const lox::Interpreter<T>::&interpreter)
    : interpreter(interpreter) {}


template <class T>
void lox::Resolver<T>::resolve(
    const std::vector < lox::stmt::Stmt<T> & statements) {
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
  return nullptr;
}


// class stmt

template <class T>
void lox::Resolver<T>::visitClassStmt(const lox::stmt::Stmt<T>::Class& _stmt) {
  ClassType enclosingClass = currentClass;
  currentClass = ClassType::CLASS;

  lox::Resolver<T>::declare(_stmt.name);
  lox::Resolver<T>::define(_stmt.name);

  if (_stmt.superclass != nullptr &&
      (_stmt.name.lexeme == _stmt.superclass.name.lexeme)) {
    Lox::error(_stmt.superclass.name, "A class can't inherit from itself.");
  }

  if (_stmt.superclass != nullptr) {
    currentClass = ClassType::SUBCLASS;
    lox::Resolver<T>::resolve(_stmt.superclass);
  }

  if (_stmt.superclass != nullptr) {
    lox::Resolver<T>::beginScope();
    scopes.peek()["super"] = true;
  }

  lox::Resolver<T>::beginScope();
  scopes.peek()["this"] = true;

  for (lox::stmt::Stmt<T>::Function method : _stmt.methods) {
    FunctionType declaration = FunctionType::METHOD;

    if (method.name.lexeme == "init") {
      declaration = FunctionType::INITIALIZER;
    }
    lox::Resolver<T>::resolveFunction(method, declaration);
  }

  lox::Resolver<T>::endScope();

  if (_stmt.superclass != nullptr) {
    lox::Resolver<T>::endScope();
  }

  currentClass = enclosingClass;
  return nullptr;
}


// expression stmt

template <class T>
void lox::Resolver<T>::visitExpressionStmt(
    const lox::stmt::Stmt<T>::Expression& _stmt) {
  lox::Resolver<T>::resolve(_stmt.expression);
  return nullptr;
}


// function stmt

template <class T>
void lox::Resolver<T>::visitFunctionStmt(
    const lox::stmt::Stmt<T>::Function& _stmt) {
  lox::Resolver<T>::declare(_stmt.name);
  lox::Resolver<T>::define(_stmt.name);

  lox::Resolver<T>::resolveFunction(_stmt, FunctionType::FUNCTION);
  return nullptr;
}


// if stmt

template <class T>
void lox::Resolver<T>::visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt) {
  lox::Resolver<T>::resolve(_stmt.condition);
  lox::Resolver<T>::resolve(_stmt.thenBranch);

  if (_stmt.elseBranch != nullptr) {
    lox::Resolver<T>::resolve(_stmt.elseBranch);
  }

  return nullptr;
}


// print stmt

template <class T>
void lox::Resolver<T>::visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt) {
  lox::Resolver<T>::resolve(_stmt.expression);
  return nullptr;
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

  return nullptr;
}


// var stmt

template <class T>
void lox::Resolver<T>::visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt) {
  lox::Resolver<T>::declare(_stmt.name);

  if (_stmt.initializer != nullptr) {
    lox::Resolver<T>::resolve(_stmt.initializer);
  }

  lox::Resolver<T>::define(_stmt.name);
  return nullptr;
}


// while stmt

template <class T>
void lox::Resolver<T>::visitWhileStmt(const lox::stmt::Stmt<T>::While& _stmt) {
  lox::Resolver<T>::resolve(_stmt.condition);
  lox::Resolver<T>::resolver(_stmt.body);
  return nullptr;
}


// assign expr

template <class T>
void lox::Resolver<T>::visitAssignExpr(
    const lox::expr::Expr<T>::Assign& _expr) {
  lox::Resolver<T>::resolve(_expr.value);
  lox::Resolver<T>::resolveLocal(_expr, _expr.name);
  return nullptr;
}


// binary expr

template <class T>
void lox::Resolver<T>::visitBinaryExpr(
    const lox::expr::Expr<T>::Binary& _expr) {
  lox::Resolver<T>::resolve(_expr.left);
  lox::Resolver<T>::resolve(_expr.right);
  return nullptr;
}


// call expr

template <class T>
void lox::Resolver<T>::visitCallExpr(const lox::expr::Expr<T>::Call& _expr) {
  lox::Resolver<T>::resolve(_expr.callee);

  for (lox::expr::Expr<T> argument : _expr.arguments) {
    lox::Resolver<T>::resolve(argument);
  }

  return nullptr;
}


// get expr

template <class T>
void lox::Resolver<T>::visitGetExpr(const lox::expr::Expr<T>::Get& _expr) {
  lox::Resolver<T>::resolve(_expr.object);
  return nullptr;
}


// grouping expr

template <class T>
void lox::Resolver<T>::visitGroupingExpr(
    const lox::expr::Expr<T>::Grouping& _expr) {
  lox::Resolver<T>::resolve(_expr.expression);
  return nullptr;
}


// literal expr

template <class T>
void lox::Resolver<T>::visitLiteralExpr(
    const lox::expr::Expr<T>::Literal& _expr) {
  return nullptr;
}


// logical expr

template <class T>
void lox::Resolver<T>::visitLogicalExpr(
    const lox::expr::Expr<T>::Logical& _expr) {
  lox::Resolver<T>::resolve(_expr.left);
  lox::Resolver<T>::resolve(_expr.right);
  return nullptr;
}


// set expr

template <class T>
void lox::Resolver<T>::visitSetExpr(const lox::expr::Expr<T>::Set& _expr) {
  lox::Resolver<T>::resolve(_expr.value);
  lox::Resolver<T>::resolve(_expr.object);
  return nullptr;
}


// super expr

template <class T>
void lox::Resolver<T>::visitSuperExpr(const lox::expr::Expr<T>::Super& _expr) {
  if (currentClass == ClassType::NONE) {
    Lox::error(_expr.keyword, "Can't use 'super' outside of a class.");

  } else if (currentClass != ClassType::SUBCLASS) {
    Lox::error(
        _expr.keyword, "Can't use 'super' in a class with no superclass.");
  }

  lox::Resolver<T>::resolveLocal(_expr, _expr.keyword);
  return nullptr;
}


// this expr

template <class T>
void lox::Resolver<T>::visitThisExpr(const lox::expr::Expr<T>::This& _expr) {
  if (currentClass == ClassType::NONE) {
    Lox::error(_expr.keyword, "Can't use 'this' outside of a class.");
    return nullptr;
  }

  lox::Resolver<T>::resolverLocal(_expr, _expr.keyword);
  return nullptr;
}


// unary expr

template <class T>
void lox::Resolver<T>::visitUnaryExpr(const lox::expr::Expr<T>::Unary& _expr) {
  lox::Resolver<T>::resolve(_expr.right);
  return nullptr;
}


// variable expr

template <class T>
void lox::Resolver<T>::visitVariableExpr(
    const lox::expr::Expr<T>::Variable& _expr) {
  if (!scopes.empty() && scopes.peek()[_expr.name.lexeme] == false) {  // TODO
    Lox::error(_expr.name, "Can't read local variable in its own initializer.");
  }

  lox::Resolver<T>::resolveLocal(_expr, _expr.name);
  return nullptr;
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
    const Resolver<T>::FunctionTypes& type) {
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

void lox::Resolver<T>::beginScope() {
  scopes.push(new std::unordered_map<std::string, bool>);
}


// exiting from stack

void lox::Resolver<T>::endScope() {
  scopes.pop();
}


void lox::Resolver<T>::declare(const Token& name) {
  if (scopes.empty()) {
    return;
  }

  std::unordered_map<std::string, bool> scope = scopes.top();

  if (scope[name.lexeme]) {
    Lox::error(name, "Already a variable with this name in this scope.");
  }

  scope[name.lexeme] = false;
}


void lox::Resolver<T>::define(const Token& name) {
  if (scopes.empty()) {
    return;
  }
  scopes.top()[name.lexeme] = true;
}


template <class T>
void lox::Resolver<T>::resolveLocal(
    const lox::expr::Expr<T>& _expr,
    const Token& name) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    auto it = scopes[i].find(name.lexeme);
    if (it != scopes[i].end()) {  // TODO
      Interpreter<T>::interpreter.resolve(_expr, scopes.size() - 1 - i);
      return;
    }
  }
}

}  // namespace lox

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Environment.h"
#include "Expr.h"
#include "Interpreter.h"
#include "Lox.h"
#include "LoxFunction.h"
#include "Return.h"
#include "RuntimeError.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {


// block stmt

template <class T>
void Interpreter<T>::visitBlockStmt(const lox::stmt::Stmt<T>::Block& _stmt) {
  // TODO
  lox::Interpreter<T>::executeBlock(
      _stmt.statements, new Environment::Environment(environment));
  return nullptr;
}


// class stmt

// template <class T>
// void Interpreter<T>::visitClassStmt(const lox::stmt::Stmt<T>::Class& _stmt)
// {}


// expression stmt

template <class T>
void Interpreter<T>::visitExpressionStmt(
    const lox::stmt::Stmt<T>::Expression& _stmt) {
  Interpreter<T>::evaluate(_stmt.expression);
  return;
}


// function stmt

template <class T>
void Interpreter<T>::visitFunctionStmt(
    const lox::stmt::Stmt<T>::Function& _stmt) {
  LoxFunction::LoxFunction function =
      new LoxFunction::LoxFunction(_stmt, environment, false);
  environment.define(_stmt.name.lexeme, function);
  return nullptr;
}


// if stmt

template <class T>
void Interpreter<T>::visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt) {
  if (lox::Interpreter<T>::isTruthy(
          lox::Interpreter<T>::evaluate(_stmt.condition))) {
    lox::Interpreter<T>::execute(_stmt.thenBranch);

  } else if (_stmt.elseBranch != nullptr) {
    lox::Interpreter<T>::execute(_stmt.elseBranch);
  }

  return nullptr;
}


// print stmt

template <class T>
void Interpreter<T>::visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt) {
  Object value = evaluate(_stmt.expression);
  std::cout << stringify(value);
  return;
}


// return stmt

template <class T>
void Interpreter<T>::visitReturnStmt(const lox::stmt::Stmt<T>::Return& _stmt) {
  Object value = nullptr;

  if (_stmt.value != nullptr) {
    value = lox::Interpreter<T>::evaluate(_stmt.value);
  }

  throw new Return::Return(value);
}


// var stmt

template <class T>
void Interpreter<T>::visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt) {
  Object value = nullptr;

  if (_stmt.initializer != nullptr) {
    value = lox::Interpreter<T>::evaluate(_stmt.initializer);
  }

  environment.define(_stmt.name.lexeme, value);
  return nullptr;
}


// while stmt

template <class T>
void Interpreter<T>::visitWhileStmt(const lox::stmt::Stmt<T>::While& _stmt) {
  while (lox::Interpreter<T>::isTruthy(
      lox::Interpreter<T>::evaluate(_stmt.condition))) {
    lox::Interpreter<T>::execute(_stmt.body);
  }

  return nullptr;
}


// interpret

template <class T>
void Interpreter<T>::interpret(
    const std::vector<lox::stmt::Stmt<T>>& statements) {
  try {
    for (lox::stmt::Stmt<T> statement : statements) {
      Interpreter<T>::execute(statement);
    }
  } catch (RuntimeError error) {
    Lox::runtimeError(error);
  }
}


// evaluate

template <class T>
void Interpreter<T>::evaluate(const lox::stmt::Stmt<T>& _stmt) {
  return _stmt.accept(*this);
}


// execute

template <class T>
void Interpreter<T>::execute(const lox::stmt::Stmt<T>& _stmt) {
  _stmt.accept(*this);
}


// execute block

void executeBlock(
    const std::vector < lox::stmt::Stmt<T> & statements,
    const Environment::Environment& environment) {
  Environment::Environment previous = this->environment;

  try {
    this->environment = environment;

    for (lox::stmt::Stmt<T> statement : statements) {
      lox::Interpreter<T>::execute(statement);
    }
  } catch {
    this->environment = previous;
  }
}


// assign expr

template <class T>
Object Interpreter<T>::visitAssignExpr(
    const lox::expr::Expr<T>::Assign& _expr) {
  Object value = lox::Interpreter<T>::evaluate(_expr.value);
  int distance = locals[_expr];

  if (distance != nullptr) {
    environment.assignAt(distance, _expr.name, value);
  } else {
    globals.assign(_expr.name, value);
  }

  return value;
}


// binary expr

template <class T>
Object Interpreter<T>::visitBinaryExpr(
    const lox::expr::Expr<T>::Binary& _expr) {
  std::string left = Interpreter<T>::evaluate(_expr.left);
  std::string right = Interpreter<T>::evaluate(_expr.right);

  switch (_expr.op.type) {
    case TokenType::MINUS:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return (double)left - (double)right;

    case TokenType::PLUS:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      if ((std::is_same<decltype(left), double>::value) &&
          (std::is_same<decltype(right), double>::value)) {
        return (double)left + (double)right;
      }
      if ((std::is_same<decltype(left), std::string>::value) &&
          (std::is_same<decltype(right), std::string>::value)) {
        return (std::string)left + (std::string)right;
      }
      break;

    case TokenType::GREATER:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);

      return (double)left > (double)right;

    case TokenType::GREATER_EQUAL:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return (double)left >= (double)right;

    case TokenType::LESS:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return (double)left < (double)right;

    case TokenType::LESS_EQUAL:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return (double)left <= (double)right;

    case TokenType::BANG_EQUAL:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return !Interpreter<T>::isEqual(left, right);

    case TokenType::EQUAL_EQUAL:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return !Interpreter<T>::isEqual(left, right);

    case TokenType::SLASH:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return (double)left / (double)right;

    case TokenType::STAR:
      Interpreter<T>::checkNumberOperands(_expr.op, left, right);
      return (double)left * (double)right;
  }

  return nullptr;
}


// call expr

template <class T>
Object Interpreter<T>::visitCallExpr(const lox::expr::Expr<T>::Call& _expr) {}


// get expr

template <class T>
Object Interpreter<T>::visitGetExpr(const lox::expr::Expr<T>::Get& _expr) {
  Object object = lox::Interpreter<T>::evaluate(_expr.object);

  if () {
  }

  throw new RuntimeError::RuntimeError(
      _expr.name, "Only instances have properties.");
}


// grouping expr

template <class T>
Object Interpreter<T>::visitGroupingExpr(
    const lox::expr::Expr<T>::Grouping& _expr) {
  return Interpreter<T>::evaluate(_expr.expression);
}


// literal expr

template <class T>
Object Interpreter<T>::visitLiteralExpr(
    const lox::expr::Expr<T>::Literal& _expr) {
  return _expr.value;
}


// logical expr

template <class T>
Object Interpreter<T>::visitLogicalExpr(
    const lox::expr::Expr<T>::Logical& _expr) {
  Object left = lox::Interpreter<T>::evaluate(_expr.left);

  if (_expr.op.type == TokenType::OR) {
    if (lox::Interpreter<T>::isTruthy(left)) {
      return left;
    }
  } else {
    if (!lox::Interpreter<T>::isTruthy(left)) {
      return left;
    }
  }

  return lox::Interpreter<T>::evaluate(_expr.right);
}


// set expr

template <class T>
Object Interpreter<T>::visitSetExpr(const lox::expr::Expr<T>::Set& _expr) {
  Object object = lox::Interpreter<T>::evaluate(_expr.object);

  if (!()) {
    throw new RuntimeError::RuntimeError(
        _expr.name, "Only instances have fields.");
  }

  Object value = lox::Interpreter<T>::evaluate(_expr.value);
  ((LoxInstance)object).set(_expr.name, value);  // TODO

  return value;
}


// super expr

template <class T>
Object Interpreter<T>::visitSuperExpr(const lox::expr::Expr<T>::Super& _expr) {}


// this expr

template <class T>
Object Interpreter<T>::visitThisExpr(const lox::expr::Expr<T>::This& _expr) {
  return lookUpVaraible(_expr.keyword, _expr);
}


// unary expr

template <class T>
Object Interpreter<T>::visitUnaryExpr(const lox::expr::Expr<T>::Unary& _expr) {
  std::string right = Interpreter<T>::evaluate(_expr.right);

  switch (_expr.op.type) {
    case TokenType::BANG:
      Interpreter<T>::checkNumberOperands(_expr.op, right);
      return !Interpreter<T>::isTruthy(right);

    case TokenType::MINUS:
      Interpreter<T>::checkNumberOperands(_expr.op, right);
      return -(double)right;
  }

  return nullptr;
}


// variable expr

template <class T>
Object Interpreter<T>::visitVariableExpr(
    const lox::expr::Expr<T>::Variable& _expr) {
  return lookUpVariable(_expr.name, _expr);
}


// resolving and binding look-up-variable

Object lookUpVariable(const Token& name, lox::expr::Expr<T> _expr) {
  int distance = locals[_expr];  // TODO

  if (distance != nullptr) {
    return environment.getAt(distance, name.lexeme);
  } else {
    return globals[name];
  }
}


// helper function


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}


// evaluate

template <class T>
Object Interpreter<T>::evaluate(const lox::expr::Expr<T>& _expr) {
  return _expr.accept(*this);
}


// check number of operand

template <class T>
void Interpreter<T>::checkNumberOperand(
    const Token& op,
    const std::string& operand) {
  if (std::is_same<decltype(operand), double>::value) {
    return;
  }

  throw RuntimeError(op, "Operand must be a number.");
}


// check truth value

template <class T>
bool Interpreter<T>::isTruthy(const std::string& object) {
  if (object.empty()) {
    return false;
  }

  if (std::is_same<decltype(object), bool>::value) {
    return bool(object);
  }

  return true;
}


// check number of operands

template <class T>
void Interpreter<T>::checkNumberOperands(
    const Token& op,
    const std::string& left,
    const std::string& right) {
  if ((std::is_same<decltype(left), double>::value) &&
      (std::is_same<decltype(right), double>::value)) {
    return;
  }
  throw RuntimeError(op, "Operands must be numbers.");
}


// check if equal

template <class T>
bool Interpreter<T>::isEqual(const std::string& a, const std::string& b) {
  if (a.empty() && b.empty()) {
    return true;
  }
  if (a.empty()) {
    return false;
  }
  return a == b;
}


// interpret

template <class T>
void Interpreter<T>::interpret(const lox::expr::Expr<T>& expression) {
  try {
    std::string value = Interpreter<T>::evaluate(expression);
    std::cout << Interpreter<T>::stringify(value);
  } catch (RuntimeError error) {
    Lox::runtimeError(error);
  }
}


// convert to string

template <class T>
std::string Interpreter<T>::stringify(const std::string& object) {
  if (object.empty()) {
    return NULL;
  }

  if (std::is_same<decltype(object), double>::value) {
    std::string text = std::string(object);
    if (text.ends_with(".0")) {
      text = text.substr(0, text.length() - 2);
    }
    return text;
  }

  return std::string(object);
}


}  // namespace lox

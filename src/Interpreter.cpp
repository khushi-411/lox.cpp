#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Expr.h"
#include "Interpreter.h"
#include "Lox.h"
#include "RuntimeError.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {


template <class T>
void Interpreter<T>::visitExpressionStmt(
    const lox::stmt::Stmt<T>::Expression& _stmt) {
  Interpreter<T>::evaluate(_stmt.expression);
  return;
}


template <class T>
void Interpreter<T>::visitPrintStmt(const lox::stmt::Stmt<T>::Print& _stmt) {
  Object value = evaluate(_stmt.expression);
  std::cout << stringify(value);
  return;
}


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


template <class T>
void Interpreter<T>::evaluate(const lox::stmt::Stmt<T>& _stmt) {
  return _stmt.accept(*this);
}


template <class T>
void Interpreter<T>::execute(const lox::stmt::Stmt<T>& _stmt) {
  _stmt.accept(*this);
}


template <class T>
Object Interpreter<T>::visitLiteralExpr(
    const lox::expr::Expr<T>::Literal& _expr) {
  return _expr.value;
}


template <class T>
Object Interpreter<T>::visitGroupingExpr(
    const lox::expr::Expr<T>::Grouping& _expr) {
  return Interpreter<T>::evaluate(_expr.expression);
}


template <class T>
Object Interpreter<T>::evaluate(const lox::expr::Expr<T>& _expr) {
  return _expr.accept(*this);
}


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


template <class T>
void Interpreter<T>::checkNumberOperand(
    const Token& op,
    const std::string& operand) {
  if (std::is_same<decltype(operand), double>::value) {
    return;
  }

  throw RuntimeError(op, "Operand must be a number.");
}


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


template <class T>
void Interpreter<T>::interpret(const lox::expr::Expr<T>& expression) {
  try {
    std::string value = Interpreter<T>::evaluate(expression);
    std::cout << Interpreter<T>::stringify(value);
  } catch (RuntimeError error) {
    Lox::runtimeError(error);
  }
}


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

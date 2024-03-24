#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Environment.h"
#include "Expr.h"
#include "Interpreter.h"
#include "Lox.h"
#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Return.h"
#include "RuntimeError.h"
#include "Stmt.h"
#include "Token.h"
#include "TokenType.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {


// block stmt

template <class T>
void Interpreter<T>::visitBlockStmt(const lox::stmt::Block<T>& _stmt) {
  lox::Interpreter<T>::executeBlock(
      _stmt.statements, new Environment(environment));
  return;
}


// class stmt

template <class T>
void Interpreter<T>::visitClassStmt(const lox::stmt::Class<T>& _stmt) {
  Object superclass = nullptr;

  if (_stmt.superclass != nullptr) {
    superclass = lox::Interpreter<T>::evaluate(_stmt.superclass);
    if (!(instanceof <LoxClass<T>>(superclass))) {
      throw new RuntimeError(
          _stmt.superclass.name, "Superclass must be a class.");
    }
  }

  environment.define(_stmt.name.getLexeme(), nullptr);

  if (_stmt.superclass != nullptr) {
    environment = new Environment(environment);
    environment.define("super", superclass);
  }

  std::unordered_map<std::string, LoxFunction<T>> methods;

  for (lox::stmt::Function<T> method : _stmt.methods) {
    LoxFunction<T> function = new LoxFunction(
        method, environment, method.name.getLexeme().equals("init"));
    methods[method.name.getLexeme()] = function;
  }

  LoxClass<T> klass =
      new LoxClass(_stmt.name.getLexeme(), (LoxClass<T>)superclass, methods);

  if (std::holds_alternative<Object>(superclass)) {
    environment = environment.enclosing;
  }

  environment.assign(_stmt.name, klass);

  return;
}


// expression stmt

template <class T>
void Interpreter<T>::visitExpressionStmt(
    const lox::stmt::Expression<T>& _stmt) {
  Interpreter<T>::evaluate(_stmt.expression);
  return;
}


// function stmt

template <class T>
void Interpreter<T>::visitFunctionStmt(const lox::stmt::Function<T>& _stmt) {
  LoxFunction<T> function = new LoxFunction(_stmt, environment, false);
  environment.define(_stmt.name.getLexeme(), function);
  return;
}


// if stmt

template <class T>
void Interpreter<T>::visitIfStmt(const lox::stmt::If<T>& _stmt) {
  if (lox::Interpreter<T>::isTruthy(
          lox::Interpreter<T>::evaluate(_stmt.condition))) {
    lox::Interpreter<T>::execute(_stmt.thenBranch);

  } else if (_stmt.elseBranch != nullptr) {
    lox::Interpreter<T>::execute(_stmt.elseBranch);
  }

  return;
}


// print stmt

template <class T>
void Interpreter<T>::visitPrintStmt(const lox::stmt::Print<T>& _stmt) {
  Object value = evaluate(_stmt.expression);
  std::cout << stringify(value);
  return;
}


// return stmt

template <class T>
void Interpreter<T>::visitReturnStmt(const lox::stmt::Return<T>& _stmt) {
  Object value = nullptr;

  if (_stmt.value != nullptr) {
    value = lox::Interpreter<T>::evaluate(_stmt.value);
  }

  throw new Return(value);
}


// var stmt

template <class T>
void Interpreter<T>::visitVarStmt(const lox::stmt::Var<T>& _stmt) {
  Object value = nullptr;

  if (_stmt.initializer != nullptr) {
    value = lox::Interpreter<T>::evaluate(_stmt.initializer);
  }

  environment.define(_stmt.name.getLexeme(), value);
  return;
}


// while stmt

template <class T>
void Interpreter<T>::visitWhileStmt(const lox::stmt::While<T>& _stmt) {
  while (lox::Interpreter<T>::isTruthy(
      lox::Interpreter<T>::evaluate(_stmt.condition))) {
    lox::Interpreter<T>::execute(_stmt.body);
  }

  return;
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
    Lox<T>::runtimeError(error);
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

template <class T>
void executeBlock(
    const std::vector<lox::stmt::Stmt<T>>& statements,
    const Environment& environment) {
  Environment previous = environment;

  try {
    environment = environment;

    for (lox::stmt::Stmt<T> statement : statements) {
      lox::Interpreter<T>::execute(statement);
    }
  } catch (...) {
    environment = previous;
  }
}


// assign expr

template <class T>
Object Interpreter<T>::visitAssignExpr(const lox::expr::Assign<T>& _expr) {
  Object value = lox::Interpreter<T>::evaluate(_expr.value);
  int distance = locals[_expr];

  if (distance != NULL) {
    environment.assignAt(distance, _expr.name, value);
  } else {
    globals.assign(_expr.name, value);
  }

  return value;
}


// binary expr

template <class T>
Object Interpreter<T>::visitBinaryExpr(const lox::expr::Binary<T>& _expr) {
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
Object Interpreter<T>::visitCallExpr(const lox::expr::Call<T>& _expr) {
  Object callee = lox::Interpreter<T>::evaluate(_expr.callee);

  std::vector<Object> arguments;
  for (lox::expr::Expr<T> argument : _expr.arguments) {
    arguments.push_back(lox::Interpreter<T>::evaluate(argument));
  }

  if (! instanceof <LoxCallable<T>>(callee)) {
    throw new RuntimeError(_expr.paren, "Can only call functions and classes.");
  }

  LoxCallable<T> function = (LoxCallable<T>)callee;

  if (arguments.size() != function.arity()) {
    throw RuntimeError(
        _expr.paren,
        "Expected " + function.arity() + " arguments but got " +
            arguments.size() + ".");
  }

  return function.call(this, arguments);
}


// get expr

template <class T>
Object Interpreter<T>::visitGetExpr(const lox::expr::Get<T>& _expr) {
  Object object = lox::Interpreter<T>::evaluate(_expr.object);

  if (instanceof <LoxInstance<T>>(object)) {
    return ((LoxInstance<T>)object).get(_expr.name);
  }

  throw new RuntimeError(_expr.name, "Only instances have properties.");
}


// grouping expr

template <class T>
Object Interpreter<T>::visitGroupingExpr(const lox::expr::Grouping<T>& _expr) {
  return Interpreter<T>::evaluate(_expr.expression);
}


// literal expr

template <class T>
Object Interpreter<T>::visitLiteralExpr(const lox::expr::Literal<T>& _expr) {
  return _expr.value;
}


// logical expr

template <class T>
Object Interpreter<T>::visitLogicalExpr(const lox::expr::Logical<T>& _expr) {
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
Object Interpreter<T>::visitSetExpr(const lox::expr::Set<T>& _expr) {
  Object object = lox::Interpreter<T>::evaluate(_expr.object);

  if (!(instanceof <LoxInstance<T>>(object))) {
    throw new RuntimeError(_expr.name, "Only instances have fields.");
  }

  Object value = lox::Interpreter<T>::evaluate(_expr.value);
  ((LoxInstance<T>)object).set(_expr.name, value);

  return value;
}


// super expr

template <class T>
Object Interpreter<T>::visitSuperExpr(const lox::expr::Super<T>& _expr) {
  int distance = locals.get(_expr);

  LoxClass<T> superclass = (LoxClass<T>)environment.getAt(distance, "super");

  LoxInstance<T> object =
      (LoxInstance<T>)environment.getAt(distance - 1, "this");

  LoxFunction<T> method = superclass.findMethod(_expr.method.getLexeme);

  if (method == nullptr) {
    throw new RuntimeError(
        _expr.method, "Undefined property '" + _expr.method.getLexeme() + "'.");
  }

  return method.bind(object);
}


// this expr

template <class T>
Object Interpreter<T>::visitThisExpr(const lox::expr::This<T>& _expr) {
  return lookUpVaraible(_expr.keyword, _expr);
}


// unary expr

template <class T>
Object Interpreter<T>::visitUnaryExpr(const lox::expr::Unary<T>& _expr) {
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
Object Interpreter<T>::visitVariableExpr(const lox::expr::Variable<T>& _expr) {
  return Interpreter<T>::lookUpVariable(_expr.name, _expr);
}


// resolving and binding look-up-variable

template <class T>
Object Interpreter<T>::lookUpVariable(
    const Token& name,
    const lox::expr::Expr<T>& _expr) {
  int distance = locals.find(_expr);

  if (distance != NULL) {
    return environment.getAt(distance, name.getLexeme());
  } else {
    return globals.get(name);
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
    Lox<T>::runtimeError(error);
  }
}


// convert to string

template <class T>
std::string Interpreter<T>::stringify(const std::string& object) {
  if (object.empty()) {
    return nullptr;
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

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


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

// block stmt


void Interpreter::visitBlockStmt(const lox::stmt::Block& _stmt) {
  Interpreter::executeBlock(
      _stmt.getStatements(), std::make_shared<Environment>(environment));
}


// class stmt


void lox::Interpreter::visitClassStmt(const lox::stmt::Class& _stmt) {
  Object superclass = nullptr;

  if (_stmt.getSuperclass() != nullptr) {
    superclass = lox::Interpreter::evaluate(_stmt.getSuperclass());
    if (!std::holds_alternative<std::shared_ptr<LoxCallable>>(superclass) ||
        !dynamic_cast<LoxClass*>(
            std::get<std::shared_ptr<LoxCallable>>(superclass).get())) {
      throw RuntimeError(
          _stmt.getSuperclass()->getName(), "Superclass must be a class.");
    }
  }

  environment->define(_stmt.getName().getLexeme(), nullptr);

  // Save environment so we can restore it after pushing the "super" scope
  std::shared_ptr<Environment> previous = environment;
  if (_stmt.getSuperclass() != nullptr) {
    environment = std::make_shared<Environment>(environment);
    environment->define("super", superclass);
  }

  std::unordered_map<std::string, LoxFunction> methods;
  for (const auto& method : _stmt.getMethods()) {
    LoxFunction function(
        *method, environment, method->getName().getLexeme() == "init");
    methods.emplace(method->getName().getLexeme(), std::move(function));
  }

  LoxClass* superclassPtr = nullptr;
  if (std::holds_alternative<std::shared_ptr<LoxCallable>>(superclass)) {
    superclassPtr = dynamic_cast<LoxClass*>(
        std::get<std::shared_ptr<LoxCallable>>(superclass).get());
  }

  auto klass = std::make_shared<LoxClass>(
      _stmt.getName().getLexeme(), superclassPtr, methods);

  if (_stmt.getSuperclass() != nullptr) {
    environment = previous;
  }

  environment->assign(
      _stmt.getName(), std::static_pointer_cast<LoxCallable>(klass));
}


// expression stmt


void lox::Interpreter::visitExpressionStmt(const lox::stmt::Expression& _stmt) {
  lox::Interpreter::evaluate(_stmt.getExpression());
  return;
}


// function stmt


void lox::Interpreter::visitFunctionStmt(const lox::stmt::Function& _stmt) {
  auto function = std::make_shared<LoxFunction>(_stmt, environment, false);
  environment->define(
      _stmt.getName().getLexeme(),
      std::static_pointer_cast<LoxCallable>(function));
}

// if stmt


void lox::Interpreter::visitIfStmt(const lox::stmt::If& _stmt) {
  if (lox::Interpreter::isTruthy(
          lox::Interpreter::evaluate(_stmt.getCondition()))) {
    lox::Interpreter::execute(_stmt.getThenBranch());

  } else if (_stmt.getElseBranch() != nullptr) {
    lox::Interpreter::execute(_stmt.getElseBranch());
  }
}


// print stmt


void lox::Interpreter::visitPrintStmt(const lox::stmt::Print& _stmt) {
  Object value = evaluate(_stmt.getExpression());
  std::cout << stringify(value) << "\n";
}


// return stmt


void lox::Interpreter::visitReturnStmt(const lox::stmt::Return& _stmt) {
  Object value = nullptr;

  if (_stmt.getValue() != nullptr) {
    value = lox::Interpreter::evaluate(_stmt.getValue());
  }

  throw Return(value);
}


// var stmt


void lox::Interpreter::visitVarStmt(const lox::stmt::Var& _stmt) {
  Object value = nullptr;

  if (_stmt.getInitializer() != nullptr) {
    value = lox::Interpreter::evaluate(_stmt.getInitializer());
  }

  environment->define(_stmt.getName().getLexeme(), value);
}


// while stmt


void lox::Interpreter::visitWhileStmt(const lox::stmt::While& _stmt) {
  while (lox::Interpreter::isTruthy(
      lox::Interpreter::evaluate(_stmt.getCondition()))) {
    lox::Interpreter::execute(_stmt.getBody());
  }
}


// interpret


void lox::Interpreter::interpret(
    const std::vector<std::shared_ptr<lox::stmt::Stmt>>& statements) {
  try {
    for (const auto& statement : statements) {
      lox::Interpreter::execute(statement);
    }
  } catch (const RuntimeError& error) {
    Lox::runtimeError(error);
  }
}


// evaluate


// void lox::Interpreter::evaluate(const lox::stmt::Stmt& _stmt) {
//   return _stmt.accept(*this);
// }


// execute


void lox::Interpreter::execute(
    const std::shared_ptr<lox::stmt::Stmt>& _stmt) {
  _stmt->accept(*this);
}


// execute block

void lox::Interpreter::executeBlock(
    const std::vector<std::shared_ptr<lox::stmt::Stmt>>& statements,
    std::shared_ptr<Environment> newEnvironment) {
  std::shared_ptr<Environment> previous = this->environment;

  try {
    this->environment = newEnvironment;

    for (const auto& statement : statements) {
      lox::Interpreter::execute(statement);
    }
  } catch (...) {
    this->environment = previous;
    throw;
  }

  this->environment = previous;
}

// assign expr


Object lox::Interpreter::visitAssignExpr(const lox::expr::Assign& _expr) {
  Object value = lox::Interpreter::evaluate(_expr.getValue());

  auto it = locals.find(&_expr);
  if (it != locals.end()) {
    environment->assignAt(it->second, _expr.getName(), value);
  } else {
    globals->assign(_expr.getName(), value);
  }

  return value;
}

// binary expr

Object lox::Interpreter::visitBinaryExpr(const lox::expr::Binary& _expr) {
  Object left = lox::Interpreter::evaluate(_expr.getLeft());
  Object right = lox::Interpreter::evaluate(_expr.getRight());

  switch (_expr.getOp().tokentype()) {
    case TokenType::MINUS:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) - std::get<double>(right);

    case TokenType::PLUS:
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) + std::get<double>(right);
      }
      if (std::holds_alternative<std::string>(left) &&
          std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) + std::get<std::string>(right);
      }
      throw RuntimeError(
          _expr.getOp(), "Operands must be two numbers or two strings.");

    case TokenType::GREATER:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) > std::get<double>(right);

    case TokenType::GREATER_EQUAL:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) >= std::get<double>(right);

    case TokenType::LESS:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) < std::get<double>(right);

    case TokenType::LESS_EQUAL:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) <= std::get<double>(right);

    case TokenType::BANG_EQUAL:
      return !lox::Interpreter::isEqual(left, right);

    case TokenType::EQUAL_EQUAL:
      return lox::Interpreter::isEqual(left, right);

    case TokenType::SLASH:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) / std::get<double>(right);

    case TokenType::STAR:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::get<double>(left) * std::get<double>(right);

    default:
      throw RuntimeError(_expr.getOp(), "Unknown binary operator.");
  }

  return nullptr;
}


// call expr


Object lox::Interpreter::visitCallExpr(const lox::expr::Call& _expr) {
  Object callee = lox::Interpreter::evaluate(_expr.getCallee());

  std::vector<Object> arguments;
  for (const auto& argument : _expr.getArguments()) {
    arguments.push_back(lox::Interpreter::evaluate(argument));
  }

  if (!std::holds_alternative<std::shared_ptr<LoxCallable>>(callee)) {
    throw RuntimeError(
        _expr.getParen(), "Can only call functions and classes.");
  }

  auto& function = std::get<std::shared_ptr<LoxCallable>>(callee);

  if (arguments.size() != static_cast<size_t>(function->arity())) {
    throw RuntimeError(
        _expr.getParen(),
        "Expected " + std::to_string(function->arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");
  }

  return function->call(*this, arguments);
}


// get expr


Object lox::Interpreter::visitGetExpr(const lox::expr::Get& _expr) {
  Object object = lox::Interpreter::evaluate(_expr.getObject());

  if (std::holds_alternative<std::shared_ptr<LoxInstance>>(object)) {
    return std::get<std::shared_ptr<LoxInstance>>(object)->get(_expr.getName());
  }

  throw RuntimeError(_expr.getName(), "Only instances have properties.");
}

// grouping expr

Object lox::Interpreter::visitGroupingExpr(const lox::expr::Grouping& _expr) {
  return lox::Interpreter::evaluate(_expr.getExpression());
}


// literal expr

Object lox::Interpreter::visitLiteralExpr(const lox::expr::Literal& _expr) {
  return _expr.getValue();
}


// logical expr

Object lox::Interpreter::visitLogicalExpr(const lox::expr::Logical& _expr) {
  Object left = lox::Interpreter::evaluate(_expr.getLeft());

  if (_expr.getOp().tokentype() == TokenType::OR) {
    if (lox::Interpreter::isTruthy(left)) {
      return left;
    }
  } else {
    if (!lox::Interpreter::isTruthy(left)) {
      return left;
    }
  }

  return lox::Interpreter::evaluate(_expr.getRight());
}


// set expr


Object lox::Interpreter::visitSetExpr(const lox::expr::Set& _expr) {
  Object object = lox::Interpreter::evaluate(_expr.getObject());

  if (!std::holds_alternative<std::shared_ptr<LoxInstance>>(object)) {
    throw RuntimeError(_expr.getName(), "Only instances have fields.");
  }

  Object value = lox::Interpreter::evaluate(_expr.getValue());
  std::get<std::shared_ptr<LoxInstance>>(object)->set(_expr.getName(), value);

  return value;
}


// super expr


Object lox::Interpreter::visitSuperExpr(const lox::expr::Super& _expr) {
  auto it = locals.find(&_expr);
  int distance = it->second;

  Object superObj = environment->getAt(distance, "super");
  Object thisObj = environment->getAt(distance - 1, "this");

  auto* superclass = dynamic_cast<LoxClass*>(
      std::get<std::shared_ptr<LoxCallable>>(superObj).get());
  auto instance = std::get<std::shared_ptr<LoxInstance>>(thisObj);

  try {
    LoxFunction method = superclass->findMethod(_expr.getMethod().getLexeme());
    auto bound = std::make_shared<LoxFunction>(method.bind(instance));
    return std::static_pointer_cast<LoxCallable>(bound);
  } catch (const std::runtime_error&) {
    throw RuntimeError(
        _expr.getMethod(),
        "Undefined property '" + _expr.getMethod().getLexeme() + "'.");
  }
}


// this expr


Object lox::Interpreter::visitThisExpr(const lox::expr::This& _expr) {
  return lookUpVariable(_expr.getKeyword(), _expr);
}

// unary expr


Object lox::Interpreter::visitUnaryExpr(const lox::expr::Unary& _expr) {
  Object right = lox::Interpreter::evaluate(_expr.getRight());

  switch (_expr.getOp().tokentype()) {
    case TokenType::BANG:
      return !lox::Interpreter::isTruthy(right);

    case TokenType::MINUS:
      lox::Interpreter::checkNumberOperand(_expr.getOp(), right);
      return -std::get<double>(right);

    default:
      throw RuntimeError(_expr.getOp(), "Unknown unary operator.");
  }

  return nullptr;
}


// variable expr


Object lox::Interpreter::visitVariableExpr(const lox::expr::Variable& _expr) {
  return lox::Interpreter::lookUpVariable(_expr.getName(), _expr);
}


// resolving and binding look-up-variable


Object lox::Interpreter::lookUpVariable(
    const Token& name,
    const lox::expr::Expr& _expr) {
  auto it = locals.find(&_expr);

  if (it != locals.end()) {
    int distance = it->second;
    return environment->getAt(distance, name.getLexeme());
  } else {
    return globals->get(name);
  }
}

// resolve - bind variable to scope depth

void lox::Interpreter::resolve(const lox::expr::Expr& _expr, int depth) {
  locals[&_expr] = depth;
}

// helper function


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}


// evaluate


Object lox::Interpreter::evaluate(
    const std::shared_ptr<lox::expr::Expr>& _expr) {
  return _expr->accept(*this);
}


// check number of operand


void lox::Interpreter::checkNumberOperand(
    const Token& op,
    const Object& operand) {
  if (std::holds_alternative<double>(operand)) {
    return;
  }

  throw RuntimeError(op, "Operand must be a number.");
}


// check truth value


bool lox::Interpreter::isTruthy(const Object& object) {
  if (std::holds_alternative<std::nullptr_t>(object)) {
    return false;
  }

  if (std::holds_alternative<bool>(object)) {
    return std::get<bool>(object);
  }

  return true;
}


// check number of operands


void lox::Interpreter::checkNumberOperands(
    const Token& op,
    const Object& left,
    const Object& right) {
  if (std::holds_alternative<double>(left) &&
      std::holds_alternative<double>(right)) {
    return;
  }
  throw RuntimeError(op, "Operands must be numbers.");
}


// check if equal


bool lox::Interpreter::isEqual(const Object& a, const Object& b) {
  return a == b;
}


// convert to string


std::string lox::Interpreter::stringify(const Object& object) {
  if (std::holds_alternative<std::nullptr_t>(object)) {
    return "nil";
  }

  if (std::holds_alternative<double>(object)) {
    std::string text = std::to_string(std::get<double>(object));
    text.erase(text.find_last_not_of('0') + 1, std::string::npos);
    if (!text.empty() && text.back() == '.') {
      text.pop_back();
    }
    return text;
  }

  if (std::holds_alternative<bool>(object)) {
    return std::get<bool>(object) ? "true" : "false";
  }

  if (std::holds_alternative<std::string>(object)) {
    return std::get<std::string>(object);
  }

  if (std::holds_alternative<std::shared_ptr<LoxCallable>>(object)) {
    auto& callable = std::get<std::shared_ptr<LoxCallable>>(object);
    if (auto* fn = dynamic_cast<LoxFunction*>(callable.get())) {
      return fn->to_string();
    }
    if (auto* cls = dynamic_cast<LoxClass*>(callable.get())) {
      return cls->to_string();
    }
    return "<callable>";
  }

  if (std::holds_alternative<std::shared_ptr<LoxInstance>>(object)) {
    return std::get<std::shared_ptr<LoxInstance>>(object)->to_string();
  }

  return "nil";
}


}  // namespace lox


// template class lox::Interpreter<double>;

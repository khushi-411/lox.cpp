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
  lox::Interpreter::executeBlock(
      _stmt.getStatements(), Environment(environment));
  return;
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
          _stmt.getSuperclass().getName(), "Superclass must be a class.");
    }
  }

  environment.define(_stmt.getName().getLexeme(), nullptr);

  // Save environment so we can restore it after pushing the "super" scope
  Environment previous = environment;
  if (_stmt.getSuperclass() != nullptr) {
    environment = Environment(environment);
    environment.define("super", superclass);
  }

  std::unordered_map<std::string, LoxFunction> methods;
  for (const lox::stmt::Function& method : _stmt.getMethods()) {
    LoxFunction function(
        method, environment, method.getName().getLexeme() == "init");
    methods.emplace(method.getName().getLexeme(), std::move(function));
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

  environment.assign(
      _stmt.getName(), std::static_pointer_cast<LoxCallable>(klass));

  return;
}


// expression stmt


void lox::Interpreter::visitExpressionStmt(const lox::stmt::Expression& _stmt) {
  lox::Interpreter::evaluate(_stmt.getExpression());
  return;
}


// function stmt


void lox::Interpreter::visitFunctionStmt(const lox::stmt::Function& _stmt) {
  auto function = std::make_shared<LoxFunction>(_stmt, environment, false);
  environment.define(
      _stmt.getName().getLexeme(),
      std::static_pointer_cast<LoxCallable>(function));
  return;
}

// if stmt


void lox::Interpreter::visitIfStmt(const lox::stmt::If& _stmt) {
  Object value = lox::Interpreter::evaluate(_stmt.getCondition());
  std::string result;

  if (std::holds_alternative<std::nullptr_t>(value)) {
    result = "nullptr";

  } else if (std::holds_alternative<std::string>(value)) {
    result = std::get<std::string>(value);

  } else if (std::holds_alternative<double>(value)) {
    result = std::to_string(std::get<double>(value));

  } else if (std::holds_alternative<bool>(value)) {
    result = std::get<bool>(value) ? "true" : "false";
  }

  if (lox::Interpreter::isTruthy(result)) {
    lox::Interpreter::execute(_stmt.getThenBranch());

  } else if (_stmt.getElseBranch() != nullptr) {
    lox::Interpreter::execute(_stmt.getElseBranch());
  }

  return;
}


// print stmt


void lox::Interpreter::visitPrintStmt(const lox::stmt::Print& _stmt) {
  Object value = evaluate(_stmt.getExpression());
  std::string result;

  if (std::holds_alternative<std::nullptr_t>(value)) {
    result = "nil";

  } else if (std::holds_alternative<std::string>(value)) {
    result = std::get<std::string>(value);

  } else if (std::holds_alternative<double>(value)) {
    result = std::to_string(std::get<double>(value));

  } else if (std::holds_alternative<bool>(value)) {
    result = std::get<bool>(value) ? "true" : "false";

  } else if (std::holds_alternative<std::shared_ptr<LoxCallable>>(value)) {
    auto& callable = std::get<std::shared_ptr<LoxCallable>>(value);
    if (auto* fn = dynamic_cast<LoxFunction*>(callable.get())) {
      result = fn->to_string();
    } else if (auto* cls = dynamic_cast<LoxClass*>(callable.get())) {
      result = cls->to_string();
    } else {
      result = "<callable>";
    }

  } else if (std::holds_alternative<std::shared_ptr<LoxInstance>>(value)) {
    result = std::get<std::shared_ptr<LoxInstance>>(value)->to_string();
  }

  std::cout << stringify(result) << "\n";
  return;
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

  environment.define(_stmt.getName().getLexeme(), value);
  return;
}


// while stmt


void lox::Interpreter::visitWhileStmt(const lox::stmt::While& _stmt) {
  Object value = lox::Interpreter::evaluate(_stmt.getCondition());
  std::string result;

  if (std::holds_alternative<std::nullptr_t>(value)) {
    result = "nullptr";

  } else if (std::holds_alternative<std::string>(value)) {
    result = std::get<std::string>(value);

  } else if (std::holds_alternative<double>(value)) {
    result = std::to_string(std::get<double>(value));

  } else if (std::holds_alternative<bool>(value)) {
    result = std::get<bool>(value) ? "true" : "false";
  }

  while (lox::Interpreter::isTruthy(result)) {
    lox::Interpreter::execute(_stmt.getBody());
  }

  return;
}


// interpret


void lox::Interpreter::interpret(
    const std::vector<lox::stmt::Stmt>& statements) {
  try {
    for (const auto& statement : statements) {
      lox::Interpreter::execute(statement);
    }
  } catch (const RuntimeError& error) {
    Lox _lox;
    _lox.runtimeError(error);
  }
}


// evaluate


// void lox::Interpreter::evaluate(const lox::stmt::Stmt& _stmt) {
//   return _stmt.accept(*this);
// }


// execute


void lox::Interpreter::execute(const lox::stmt::Stmt& _stmt) {
  _stmt.accept(*this);
}


// execute block

void lox::Interpreter::executeBlock(
    const std::vector<lox::stmt::Stmt>& statements,
    const Environment& newEnvironment) {
  Environment previous = this->environment;

  try {
    this->environment = newEnvironment;

    for (const lox::stmt::Stmt& statement : statements) {
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
    environment.assignAt(it->second, _expr.getName(), value);
  } else {
    globals.assign(_expr.getName(), value);
  }

  return value;
}

// binary expr

Object lox::Interpreter::visitBinaryExpr(const lox::expr::Binary& _expr) {
  std::string left =
      std::get<std::string>(lox::Interpreter::evaluate(_expr.getLeft()));
  std::string right =
      std::get<std::string>(lox::Interpreter::evaluate(_expr.getRight()));

  switch (_expr.getOp().tokentype()) {
    case TokenType::MINUS:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) - std::stod(right);

    case TokenType::PLUS:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      if ((std::is_same<decltype(left), double>::value) &&
          (std::is_same<decltype(right), double>::value)) {
        return std::stod(left) + std::stod(right);
      }
      if ((std::is_same<decltype(left), std::string>::value) &&
          (std::is_same<decltype(right), std::string>::value)) {
        return (std::string)left + (std::string)right;
      }
      break;

    case TokenType::GREATER:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) > std::stod(right);

    case TokenType::GREATER_EQUAL:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) >= std::stod(right);

    case TokenType::LESS:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) < std::stod(right);

    case TokenType::LESS_EQUAL:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) <= std::stod(right);

    case TokenType::BANG_EQUAL:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return !lox::Interpreter::isEqual(left, right);

    case TokenType::EQUAL_EQUAL:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return !lox::Interpreter::isEqual(left, right);

    case TokenType::SLASH:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) / std::stod(right);

    case TokenType::STAR:
      lox::Interpreter::checkNumberOperands(_expr.getOp(), left, right);
      return std::stod(left) * std::stod(right);

    default:
      throw RuntimeError(_expr.getOp(), "Unknown binary operator.");
  }

  return nullptr;
}


// call expr


Object lox::Interpreter::visitCallExpr(const lox::expr::Call& _expr) {
  Object callee = lox::Interpreter::evaluate(_expr.getCallee());

  std::vector<Object> arguments;
  for (const lox::expr::Expr& argument : _expr.getArguments()) {
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
    if (lox::Interpreter::isTruthy(std::get<std::string>(left))) {
      return left;
    }
  } else {
    if (!lox::Interpreter::isTruthy(std::get<std::string>(left))) {
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

  Object superObj = environment.getAt(distance, "super");
  Object thisObj = environment.getAt(distance - 1, "this");

  auto* superclass = dynamic_cast<LoxClass*>(
      std::get<std::shared_ptr<LoxCallable>>(superObj).get());
  LoxInstance& object = *std::get<std::shared_ptr<LoxInstance>>(thisObj);

  try {
    LoxFunction method = superclass->findMethod(_expr.getMethod().getLexeme());
    auto bound = std::make_shared<LoxFunction>(method.bind(object));
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
  std::string right =
      std::get<std::string>(lox::Interpreter::evaluate(_expr.getRight()));

  switch (_expr.getOp().tokentype()) {
    case TokenType::BANG:
      lox::Interpreter::checkNumberOperand(_expr.getOp(), right);
      return !lox::Interpreter::isTruthy(right);

    case TokenType::MINUS:
      lox::Interpreter::checkNumberOperand(_expr.getOp(), right);
      return -std::stod(right);

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
    return environment.getAt(distance, name.getLexeme());
  } else {
    return globals.get(name);
  }
}

// resolve - bind variable to scope depth

void lox::Interpreter::resolve(const lox::expr::Expr& _expr, const int& depth) {
  locals[&_expr] = depth;
}

// helper function


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}


// evaluate


Object lox::Interpreter::evaluate(const lox::expr::Expr& _expr) {
  return _expr.accept(*this);
}


// check number of operand


void lox::Interpreter::checkNumberOperand(
    const Token& op,
    const std::string& operand) {
  if (std::is_same<decltype(operand), double>::value) {
    return;
  }

  throw RuntimeError(op, "Operand must be a number.");
}


// check truth value


bool lox::Interpreter::isTruthy(const std::string& object) {
  if (object.empty()) {
    return false;
  }

  if (std::is_same<decltype(object), bool>::value) {
    // https://stackoverflow.com/questions/2165921
    bool b;
    std::istringstream(object) >> std::boolalpha >> b;
    return b;
  }

  return true;
}


// check number of operands


void lox::Interpreter::checkNumberOperands(
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


bool lox::Interpreter::isEqual(const std::string& a, const std::string& b) {
  if (a.empty() && b.empty()) {
    return true;
  }
  if (a.empty()) {
    return false;
  }
  return a == b;
}


// interpret


void lox::Interpreter::interpret(const lox::expr::Expr& expression) {
  try {
    Object result = lox::Interpreter::evaluate(expression);
    std::string value;

    if (std::holds_alternative<std::nullptr_t>(result)) {
      value = "nil";

    } else if (std::holds_alternative<std::string>(result)) {
      value = std::get<std::string>(result);

    } else if (std::holds_alternative<double>(result)) {
      value = std::to_string(std::get<double>(result));

    } else if (std::holds_alternative<bool>(result)) {
      value = std::get<bool>(result) ? "true" : "false";

    } else if (std::holds_alternative<std::shared_ptr<LoxCallable>>(result)) {
      auto& callable = std::get<std::shared_ptr<LoxCallable>>(result);
      if (auto* fn = dynamic_cast<LoxFunction*>(callable.get())) {
        value = fn->to_string();
      } else if (auto* cls = dynamic_cast<LoxClass*>(callable.get())) {
        value = cls->to_string();
      } else {
        value = "<callable>";
      }

    } else if (std::holds_alternative<std::shared_ptr<LoxInstance>>(result)) {
      value = std::get<std::shared_ptr<LoxInstance>>(result)->to_string();
    }

    std::cout << lox::Interpreter::stringify(value) << "\n";

  } catch (const RuntimeError& error) {
    Lox _lox;
    _lox.runtimeError(error);
  }
}


// convert to string


std::string lox::Interpreter::stringify(const std::string& object) {
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


// template class lox::Interpreter<double>;

#include <cstdarg>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "ASTPrinter.h"
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

// convert tree to the string

namespace lox {


std::string ASTPrinter::print(const lox::expr::Expr& _expr) {
  return _expr.accept(*this);
}


std::string ASTPrinter::print(const lox::stmt::Stmt& _stmt) {
  return _stmt.accept(*this);
}


// assign expr

std::string ASTPrinter::visitAssignExpr(const lox::expr::Assign& _expr) {
  return ASTPrinter::parenthesize2(
      "=", 2, _expr.getName().getLexeme(), _expr.getValue());
}


// binary expr

std::string ASTPrinter::visitBinaryExpr(const lox::expr::Binary& _expr) {
  return ASTPrinter::parenthesize(
      _expr.getOp().getLexeme(), 2, _expr.getLeft(), _expr.getRight());
}

/*
// call expr

std::string ASTPrinter::visitCallExpr(const lox::expr::Call& _expr) {
  return ASTPrinter::parenthesize2(
      "call", _expr.getCallee(), _expr.getArguments());
}


// get expr

std::string ASTPrinter::visitGetExpr(const lox::expr::Get& _expr) {
  return ASTPrinter::parenthesize2(
      ".", _expr.getObject(), _expr.getName().getLexeme());
}
*/

// grouping expr

std::string ASTPrinter::visitGroupingExpr(const lox::expr::Grouping& _expr) {
  return ASTPrinter::parenthesize("group", 1, _expr.getExpression());
}


// literal expr

std::string ASTPrinter::visitLiteralExpr(const lox::expr::Literal& _expr) {
  if (_expr.getValue().empty()) {
    return "nil";
  }
  return std::string(_expr.getValue());
}


// logical expr

std::string ASTPrinter::visitLogicalExpr(const lox::expr::Logical& _expr) {
  return ASTPrinter::parenthesize(
      _expr.getOp().getLexeme(), 2, _expr.getLeft(), _expr.getRight());
}

/*
// set expr

std::string ASTPrinter::visitSetExpr(const lox::expr::Set& _expr) {
  return ASTPrinter::parenthesize2(
      "=", _expr.getObject(), _expr.getName().getLexeme(), _expr.getValue());
}


// super expr

std::string ASTPrinter::visitSuperExpr(const lox::expr::Super& _expr) {
  return ASTPrinter::parenthesize2("super", 1, _expr.getMethod());
}


// this expr

std::string ASTPrinter::visitThisExpr(const lox::expr::This& _expr) {
  return "this";
}
*/

// unary expr

std::string ASTPrinter::visitUnaryExpr(const lox::expr::Unary& _expr) {
  return ASTPrinter::parenthesize(
      _expr.getOp().getLexeme(), 1, _expr.getRight());
}


// variable expr

std::string ASTPrinter::visitVariableExpr(const lox::expr::Variable& _expr) {
  return _expr.getName().getLexeme();
}


// block stmt

std::string ASTPrinter::visitBlockStmt(const lox::stmt::Block& _stmt) {
  std::string builder = "(block ";

  for (lox::stmt::Stmt statement : _stmt.getStatements()) {
    builder += statement.accept(*this);
  }

  builder += ")";
  return builder;
}

/*
// class stmt

std::string ASTPrinter::visitClassStmt(const lox::stmt::Class& _stmt) {
  std::vector<std::string> builder;
  builder.push_back("class " + _stmt.getName().getLexeme());

  if (_stmt.getSuperclass() != NULL) {
    builder.push_back(" < " + print(_stmt.getSuperclass()));
  }

  for (typename lox::stmt::Function method : _stmt.getMethods()) {
    builder.push_back(" " + print(method));
  }

  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}
*/

// expression stmt

std::string ASTPrinter::visitExpressionStmt(
    const lox::stmt::Expression& _stmt) {
  return ASTPrinter::parenthesize(";", 1, _stmt.getExpression());
}


// function stmt
/*
std::string ASTPrinter::visitFunctionStmt(const lox::stmt::Function& _stmt) {
  std::vector<std::string> builder;
  builder.push_back("fun( " + _stmt.getName().getLexeme() + "(");

  for (Token param : _stmt.getParams()) {
    if (param != _stmt.params[0]) {  // TODO
      builder.push_back(" ");
    }
    builder.push_back(param.getLexeme());
  }

  builder.push_back(")");

  for (lox::stmt::Stmt body : _stmt.getBody()) {
    builder.push_back(body.accept(*this));
  }

  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}
*/

// if stmt

std::string ASTPrinter::visitIfStmt(const lox::stmt::If& _stmt) {
  if (_stmt.getElseBranch() == NULL) {
    return ASTPrinter::parenthesize2(
        "if", 2, _stmt.getCondition(), _stmt.getThenBranch());
  }

  return ASTPrinter::parenthesize2(
      "if-else",
      3,
      _stmt.getCondition(),
      _stmt.getThenBranch(),
      _stmt.getElseBranch());
}


// print stmt

std::string ASTPrinter::visitPrintStmt(const lox::stmt::Print& _stmt) {
  return ASTPrinter::parenthesize("print", 1, _stmt.getExpression());
}


// return stmt

std::string ASTPrinter::visitReturnStmt(const lox::stmt::Return& _stmt) {
  if (_stmt.getValue() == NULL) {
    return "(return)";
  }

  return ASTPrinter::parenthesize("return", 1, _stmt.getValue());
}


// var stmt

std::string ASTPrinter::visitVarStmt(const lox::stmt::Var& _stmt) {
  if (_stmt.getInitializer() == NULL) {
    return ASTPrinter::parenthesize2("var", 1, _stmt.getName());
  }

  return ASTPrinter::parenthesize2(
      "var", 3, _stmt.getName(), "=", _stmt.getInitializer());
}


// while stmt

std::string ASTPrinter::visitWhileStmt(const lox::stmt::While& _stmt) {
  return ASTPrinter::parenthesize2(
      "while", 2, _stmt.getCondition(), _stmt.getBody());
}


std::string ASTPrinter::parenthesize(
    const std::string& name,
    int numExprs,
    ...) {
  std::string builder = "(" + name;

  va_list args;
  va_start(args, numExprs);

  for (int i = 0; i < numExprs; i++) {
    lox::expr::Expr* _expr = va_arg(args, lox::expr::Expr*);
    builder += " " + _expr->accept(*this);
  }

  va_end(args);

  builder += ")";
  return builder;
}


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}

/*
void transform(std::string& builder, int numObj, ...) {
  va_list args;
  va_start(args, numObj);

  for (int i = 0; i < numObj; i++) {
    Object* _obj = va_arg(args, Object*);

    builder += " ";

    if (std::holds_alternative<std::string>(_obj)) {
      if (lox::expr::Expr* _expr =
dynamic_cast<lox::expr::Expr*>(std::get<std::string>(_obj))) { builder +=
std::get<std::string>(_obj); // _expr->accept(*this); } else if
(lox::stmt::Stmt* _stmt =
dynamic_cast<lox::stmt::Stmt*>(std::get<std::string>(_obj))) { builder +=
std::get<std::string>(_obj); // _stmt->accept(*this);
//      } else if (Token* token = dynamic_cast<Token*>(_obj)) {
//        builder += token->getLexeme();
      }
    } (std::holds_alternative<double>(_obj)) {
      if (lox::expr::Expr* _expr =
dynamic_cast<lox::expr::Expr*>(std::get<double>(_obj))) { builder +=
std::string(std::get<double>(_obj)); //_expr->accept(*this); } else if
(lox::stmt::Stmt* _stmt =
dynamic_cast<lox::stmt::Stmt*>(std::get<double>(_obj))) { builder +=
std::string(std::get<double>(_obj)); //_stmt->accept(*this);
//      } else if (Token* token = dynamic_cast<Token*>(_obj)) {
//        builder += token->getLexeme();
      }
    } (std::holds_alternative<bool>(_obj)) {
      if (lox::expr::Expr* _expr =
dynamic_cast<lox::expr::Expr*>(std::get<bool>(_obj))) { builder +=
std::string(std::get<bool>(_obj)); //_expr->accept(*this); } else if
(lox::stmt::Stmt* _stmt = dynamic_cast<lox::stmt::Stmt*>(std::get<bool>(_obj)))
{ builder += std::string(std::get<bool>(_obj)); // _stmt->accept(*this);
//      } else if (Token* token = dynamic_cast<Token*>(_obj)) {
//        builder += token->getLexeme();
//      }
//    } else if (std::vector<Object*>* vec =
dynamic_cast<std::vector<Object*>*>(_obj)) {
//        for (Object* item : *vec) {
//          transform(builder, 1, item);
        }
    } else {
      builder += "Unknown type";
    }
  }

  va_end(args);
}
*/


void transform(std::string& builder, int numObj, ...) {
  va_list args;
  va_start(args, numObj);

  for (int i = 0; i < numObj; i++) {
    Object obj = va_arg(args, Object);

    builder += " ";

    if (std::holds_alternative<std::string>(obj)) {
      builder += std::get<std::string>(obj);

    } else if (std::holds_alternative<double>(obj)) {
      builder += std::to_string(std::get<double>(obj));

    } else if (std::holds_alternative<bool>(obj)) {
      builder += std::get<bool>(obj) ? "true" : "false";

    } else if (std::holds_alternative<std::nullptr_t>(obj)) {
      builder += "null";

    } else {
      builder += "Unknown type";
    }
  }

  va_end(args);
}


std::string ASTPrinter::parenthesize2(
    const std::string& name,
    int numObj,
    ...) {
  std::string builder = "(" + name;

  va_list args;
  va_start(args, numObj);

  for (int i = 0; i < numObj; i++) {
    Object* _obj = va_arg(args, Object*);
    transform(builder, 1, _obj);
  }

  va_end(args);

  builder += ")";
  return builder;
}


static void main(std::vector<std::string> args) {
  lox::expr::Unary unaryExpr(
      Token(TokenType::MINUS, "-", nullptr, 1),
      lox::expr::Literal(Object(123.0)));

  lox::expr::Grouping groupingExpr(lox::expr::Literal(Object(45.67)));

  lox::expr::Binary binaryExpr(
      unaryExpr, Token(TokenType::STAR, "*", nullptr, 1), groupingExpr);

  // ASTPrinter printer;
  // std::cout << printer.print(binaryExpr) << "\n";
}


}  // namespace lox

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
      "=", _expr.getName().getLexeme(), _expr.getValue());
}


// binary expr

std::string ASTPrinter::visitBinaryExpr(const lox::expr::Binary& _expr) {
  return ASTPrinter::parenthesize(
      _expr.getOp().getLexeme(), _expr.getLeft(), _expr.getRight());
}


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


// grouping expr

std::string ASTPrinter::visitGroupingExpr(const lox::expr::Grouping& _expr) {
  return ASTPrinter::parenthesize("group", _expr.getExpression());
}


// literal expr

std::string ASTPrinter::visitLiteralExpr(const lox::expr::Literal& _expr) {
  if (_expr.getValue() == NULL) {
    return "nil";
  }
  return _expr.getValue().to_string();
}


// logical expr

std::string ASTPrinter::visitLogicalExpr(const lox::expr::Logical& _expr) {
  return ASTPrinter::parenthesize(
      _expr.getOp().getLexeme(), _expr.getLeft(), _expr.getRight());
}


// set expr

std::string ASTPrinter::visitSetExpr(const lox::expr::Set& _expr) {
  return ASTPrinter::parenthesize2(
      "=", _expr.getObject(), _expr.getName().getLexeme(), _expr.getValue());
}


// super expr

std::string ASTPrinter::visitSuperExpr(const lox::expr::Super& _expr) {
  return ASTPrinter::parenthesize2("super", _expr.getMethod());
}


// this expr

std::string ASTPrinter::visitThisExpr(const lox::expr::This& _expr) {
  return "this";
}


// unary expr

std::string ASTPrinter::visitUnaryExpr(const lox::expr::Unary& _expr) {
  return ASTPrinter::parenthesize(_expr.getOp().getLexeme(), _expr.getRight());
}


// variable expr

std::string ASTPrinter::visitVariableExpr(const lox::expr::Variable& _expr) {
  return _expr.getName().getLexeme();
}


// block stmt

std::string ASTPrinter::visitBlockStmt(const lox::stmt::Block& _stmt) {
  std::vector<std::string> builder;
  builder.push_back("(block ");

  for (lox::stmt::Stmt statement : _stmt.getStatements()) {
    builder.push_back(statement.accept(*this));
  }

  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}


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


// expression stmt

std::string ASTPrinter::visitExpressionStmt(
    const lox::stmt::Expression& _stmt) {
  return ASTPrinter::parenthesize(";", _stmt.getExpression());
}


// function stmt

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


// if stmt

std::string ASTPrinter::visitIfStmt(const lox::stmt::If& _stmt) {
  if (_stmt.getElseBranch() == NULL) {
    return ASTPrinter::parenthesize2(
        "if", _stmt.getCondition(), _stmt.getThenBranch());
  }

  return ASTPrinter::parenthesize2(
      "if-else",
      _stmt.getCondition(),
      _stmt.getThenBranch(),
      _stmt.getElseBranch());
}


// print stmt

std::string ASTPrinter::visitPrintStmt(const lox::stmt::Print& _stmt) {
  return ASTPrinter::parenthesize("print", _stmt.getExpression());
}


// return stmt

std::string ASTPrinter::visitReturnStmt(const lox::stmt::Return& _stmt) {
  if (_stmt.getValue() == NULL) {
    return "(return)";
  }

  return ASTPrinter::parenthesize("return", _stmt.getValue());
}


// var stmt

std::string ASTPrinter::visitVarStmt(const lox::stmt::Var& _stmt) {
  if (_stmt.getInitializer() == NULL) {
    return ASTPrinter::parenthesize2("var", _stmt.getName());
  }

  return ASTPrinter::parenthesize2(
      "var", _stmt.getName(), "=", _stmt.getInitializer());
}


// while stmt

std::string ASTPrinter::visitWhileStmt(const lox::stmt::While& _stmt) {
  return ASTPrinter::parenthesize2(
      "while", _stmt.getCondition(), _stmt.getBody());
}


std::string ASTPrinter::parenthesize(
    const std::string& name,
    const lox::expr::Expr& exprs) {
  std::vector<std::string> builder;
  builder.push_back("(");
  builder.push_back(name);
  for (lox::expr::Expr _expr : exprs) {
    builder.push_back(" ");
    builder.push_back(_expr.accept(*this));
  }
  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}


template <typename base, typename T>
bool instanceof (const T* ptr) {
  return dynamic_cast<const base*>(ptr) != nullptr;
}


void transform(std::vector<std::string>& builder, const Object& parts) {
  for (const Object& part : parts) {
    builder.push_back(" ");
    // https://stackoverflow.com/questions/500493
    if (instanceof <lox::expr::Expr>(part)) {
      lox::expr::Expr _expr = static_cast<lox::expr::Expr>(part);
      // TODO: re-check the reason
      // error: invalid use of ‘this’ in non-member function
      builder.push_back(_expr.accept());

    } else if (instanceof <lox::stmt::Stmt>(part)) {
      lox::stmt::Stmt _stmt = static_cast<lox::stmt::Stmt>(part);
      builder.push_back(_stmt.accept());

    } else if (instanceof <Token>(part)) {
      builder.push_back(static_cast<Token>(part).getLexeme());

    } else if (std::holds_alternative<std::vector<std::string>>(part)) {
      const auto& _part = std::get<std::vector<std::string>>(part);
      transform(builder, _part);

    } else {
      // https://stackoverflow.com/questions/23799174
      std::string _str = std::get<std::string>(part);
      builder.push_back(_str);
    }
  }
}


std::string ASTPrinter::parenthesize2(
    const std::string& name,
    const Object& parts) {
  std::vector<std::string> builder;

  builder.push_back("(");
  builder.push_back(name);

  transform(builder, parts);
  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());

  return _builder;
}


static void main(std::vector<std::string> args) {
  lox::expr::Expr expression = lox::expr::Binary(
      lox::expr::Unary(
          Token(TokenType::MINUS, "-", NULL, 1), lox::expr::Literal(123)),
      Token(TokenType::STAR, "*", NULL, 1),
      lox::expr::Grouping(lox::expr::Literal(45.67)));
  std::cout << ASTPrinter::print(expression);
}


}  // namespace lox

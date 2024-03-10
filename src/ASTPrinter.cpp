#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "ASTPrinter.h"
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"


// convert tree to the string

namespace lox {


template <class T>
std::string ASTPrinter<T>::print(const lox::expr::Expr<T>& _expr) {
  return _expr.accept(*this);
}


template <class T>
std::string ASTPrinter<T>::print(const lox::stmt::Stmt<T>& _stmt) {
  return _stmt.accept(*this);
}


// all exprs....

template <class T>
std::string ASTPrinter<T>::visitBinaryExpr(
    const lox::expr::Expr<T>::Binary& _expr) {
  return ASTPrinter<T>::parenthesize(_expr.op.lexeme, _expr.left, _expr.right);
}


template <class T>
std::string ASTPrinter<T>::visitGroupingExpr(
    const lox::expr::Expr<T>::Grouping& _expr) {
  return ASTPrinter<T>::parenthesize("group", _expr.expression);
}


template <class T>
std::string ASTPrinter<T>::visitLiteralExpr(
    const lox::expr::Expr<T>::Literal& _expr) {
  if (_expr.value == NULL) {
    return "nil";
  }
  return _expr.value.to_string();
}


template <class T>
std::string ASTPrinter<T>::visitUnaryExpr(
    const lox::expr::Expr<T>::Unary& _expr) {
  return ASTPrinter<T>::parenthesize(_expr.op.lexeme, _expr.right);
}


// all Stmt....

template <class T>
std::string ASTPrinter<T>::visitBlockStmt(
    const lox::stmt::Stmt<T>::Block& _stmt) {
  std::vector<std::string> builder;
  builder.push_back("(block ");

  for (lox::stmt::Stmt<T> statement : _stmt.statements) {
    builder.push_back(statement.accept(*this));
  }

  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}


/*
template <class T>
std::string ASTPrinter<T>::visitClassStmt(const lox::stmt::Stmt<T>::Class&
_stmt) { std::vector<std::string> builder; builder.push_back("class " +
_stmt.name.lexeme);

  if (_stmt.superclass != NULL) {
      builder.push_back(" < " + print(_stmt.superclass));
  }

  for (lox::stmt::Stmt<T>::Function<T> method : _stmt.methods) {
      builder.push_back(" " + print(method));
  }

  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}
*/


template <class T>
std::string ASTPrinter<T>::visitExpressionStmt(
    const lox::stmt::Stmt<T>::Expression& _stmt) {
  return ASTPrinter<T>::parenthesize(";", _stmt.expression);
}


template <class T>
std::string ASTPrinter<T>::visitFunctionStmt(
    const lox::stmt::Stmt<T>::Function& _stmt) {
  std::vector<std::string> builder;
  builder.push_back("fun( " + _stmt.name.lexeme + "(");

  for (Token param : _stmt.params) {
    if (param != _stmt.params[0]) {
      builder.push_back(" ");
    }
    builder.push_back(param.lexeme);
  }

  builder.push_back(")");

  for (lox::stmt::Stmt<T> body : _stmt.body) {
    builder.push_back(body.accept(*this));
  }

  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}


template <class T>
std::string ASTPrinter<T>::visitIfStmt(const lox::stmt::Stmt<T>::If& _stmt) {
  if (_stmt.elseBranch == NULL) {
    return ASTPrinter<T>::parenthesize2(
        "if", _stmt.condition, _stmt.thenBranch);
  }

  return parenthesize2(
      "if-else", _stmt.condition, _stmt.thenBranch, _stmt.elseBranch);
}


template <class T>
std::string ASTPrinter<T>::visitPrintStmt(
    const lox::stmt::Stmt<T>::Print& _stmt) {
  return parenthesize("print", _stmt.expression);
}


template <class T>
std::string ASTPrinter<T>::visitReturnStmt(
    const lox::stmt::Stmt<T>::Return& _stmt) {
  if (_stmt.value == NULL) {
    return "(return)";
  }

  return parenthesize("return", _stmt.value);
}


template <class T>
std::string ASTPrinter<T>::visitVarStmt(const lox::stmt::Stmt<T>::Var& _stmt) {
  if (_stmt.initializer == NULL) {
    return parenthesize2("var", _stmt.name);
  }

  return parenthesize2("var", _stmt.name, "=", _stmt.initializer);
}


template <class T>
std::string ASTPrinter<T>::visitWhileStmt(
    const lox::stmt::Stmt<T>::While& _stmt) {
  return parenthesize2("while", _stmt.condition, _stmt.body);
}


template <class T>
std::string ASTPrinter<T>::parenthesize(
    const std::string& name,
    const lox::expr::Expr<T>& exprs) {
  std::vector<std::string> builder;
  builder.push_back("(");
  builder.push_back(name);
  for (lox::expr::Expr<T> _expr : exprs) {
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


template <class T>
void transform(
    std::vector<std::string> builder,
    const std::variant<std::nullptr_t, std::string, double, bool>& parts) {
  for (std::variant<std::nullptr_t, std::string, double, bool> part : parts) {
    builder.push_back(" ");
    // https://stackoverflow.com/questions/500493
    if (instanceof <lox::expr::Expr<T>>(part)) {
      lox::expr::Expr<T> _expr = static_cast<lox::expr::Expr<T>>(part);
      // TODO: re-check the reason
      // error: invalid use of ‘this’ in non-member function
      builder.push_back(_expr.accept());
    } else if (instanceof <lox::stmt::Stmt<T>>(part)) {
      lox::stmt::Stmt<T> _stmt = static_cast<lox::stmt::Stmt<T>>(part);
      builder.push_back(_stmt.accept());
      //} else if (instanceof<Token>(part)) {
      //    builder.push_back(static_cast<Token>(part).lexeme);
      //} else if (instanceof<std::vector<std::string>>(part)) {
      //    std::vector<std::string> _part =
      //    static_cast<std::vector<std::string>>(part); transform(builder,
      //    _part);
    } else {
      // https://stackoverflow.com/questions/23799174
      std::string _str = std::get<std::string>(part);
      builder.push_back(_str);
    }
  }
}


template <class T>
std::string ASTPrinter<T>::parenthesize2(
    const std::string& name,
    const std::variant<std::nullptr_t, std::string, double, bool>& parts) {
  std::vector<std::string> builder;

  builder.push_back("(");
  builder.push_back(name);
  // using <T>: because type deduction failed earlier
  transform<T>(builder, parts);
  builder.push_back(")");
  std::string _builder(builder.begin(), builder.end());
  return _builder;
}


template <class T>
static void main(std::vector<std::string> args) {
  lox::expr::Expr<T> expression = lox::expr::Expr<T>::Binary(
      lox::expr::Expr<T>::Unary(
          Token(TokenType::MINUS, "-", NULL, 1),
          lox::expr::Expr<T>::Literal(123)),
      Token(TokenType::STAR, "*", NULL, 1),
      lox::expr::Expr<T>::Grouping(lox::expr::Expr<T>::Literal(45.67)));
  std::cout << ASTPrinter<T>::print(expression);
}


}  // namespace lox

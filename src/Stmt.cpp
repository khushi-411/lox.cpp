#include <vector>

#include "Stmt.h"


namespace lox {


// block

lox::stmt::Block::Block(const std::vector<lox::stmt::Stmt>& statements)
    : statements(statements) {}


template <class T>
const T lox::stmt::Block::accept(const Visitor<T>& visitor) const {
  return visitor.visitBlockStmt(*this);
}


const std::vector<lox::stmt::Stmt>& lox::stmt::Block::getStatements() const {
  return statements;
}


// expression

lox::stmt::Expression::Expression(const lox::expr::Expr& expression)
    : expression(expression) {}


template <class T>
const T lox::stmt::Expression::accept(const Visitor<T>& visitor) const {
  return visitor.visitExpressionStmt(*this);
}


const lox::expr::Expr& lox::stmt::Expression::getExpression() const {
  return expression;
}


// function

lox::stmt::Function::Function(
    const Token& name,
    const std::vector<Token>& params,
    const std::vector<lox::stmt::Stmt>& body)
    : name(name), params(params), body(body) {}


template <class T>
const T lox::stmt::Function::accept(const Visitor<T>& visitor) const {
  return visitor.visitFunctionStmt(*this);
}


const Token& lox::stmt::Function::getName() const {
  return name;
}


const std::vector<Token>& lox::stmt::Function::getParams() const {
  return params;
}


const std::vector<lox::stmt::Stmt>& lox::stmt::Function::getBody() const {
  return body;
}


// class

lox::stmt::Class::Class(
    const Token& name,
    const lox::expr::Variable& superclass,
    const std::vector<lox::stmt::Function>& methods)
    : name(name), superclass(superclass), methods(methods) {}


template <class T>
const T lox::stmt::Class::accept(const Visitor<T>& visitor) const {
  return visitor.visitClassStmt(*this);
}


const Token& lox::stmt::Class::getName() const {
  return name;
}


const lox::expr::Variable& lox::stmt::Class::getSuperclass() const {
  return superclass;
}


const std::vector<lox::stmt::Function>& lox::stmt::Class::getMethods() const {
  return methods;
}


// if

lox::stmt::If::If(
    const lox::expr::Expr& condition,
    const lox::stmt::Stmt* thenBranch,
    const lox::stmt::Stmt* elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}


template <class T>
const T lox::stmt::If::accept(const Visitor<T>& visitor) const {
  return visitor.visitIfStmt(*this);
}


const lox::expr::Expr& lox::stmt::If::getCondition() const {
  return condition;
}


const lox::stmt::Stmt& lox::stmt::If::getThenBranch() const {
  return *thenBranch;
}


const lox::stmt::Stmt& lox::stmt::If::getElseBranch() const {
  return *elseBranch;
}


// print

lox::stmt::Print::Print(const lox::expr::Expr& expression)
    : expression(expression) {}


template <class T>
const T lox::stmt::Print::accept(const Visitor<T>& visitor) const {
  return visitor.visitPrintStmt(*this);
}


const lox::expr::Expr& lox::stmt::Print::getExpression() const {
  return expression;
}


// return

lox::stmt::Return::Return(const Token& keyword, const lox::expr::Expr& value)
    : keyword(keyword), value(value) {}


template <class T>
const T lox::stmt::Return::accept(const Visitor<T>& visitor) const {
  return visitor.visitReturnStmt(*this);
}


const Token& lox::stmt::Return::getKeyword() const {
  return keyword;
}


const lox::expr::Expr& lox::stmt::Return::getValue() const {
  return value;
}


// var

lox::stmt::Var::Var(const Token& name, const lox::expr::Expr& initializer)
    : name(name), initializer(initializer) {}


template <class T>
const T lox::stmt::Var::accept(const Visitor<T>& visitor) const {
  return visitor.visitVarStmt(*this);
}


const Token& lox::stmt::Var::getName() const {
  return name;
}


const lox::expr::Expr& lox::stmt::Var::getInitializer() const {
  return initializer;
}


// while

lox::stmt::While::While(
    const lox::expr::Expr& condition,
    const lox::stmt::Stmt& body)
    : condition(condition), body(body) {}


template <class T>
const T lox::stmt::While::accept(const Visitor<T>& visitor) const {
  return visitor.visitWhileStmt(*this);
}


const lox::expr::Expr& lox::stmt::While::getCondition() const {
  return condition;
}


const lox::stmt::Stmt& lox::stmt::While::getBody() const {
  return body;
}


// accept

/*
template <class T>
T accept(const Visitor<T>& visitor) {
    return visitor(*this);
}
*/

}  // namespace lox

#include <vector>

#include "Stmt.h"


namespace lox {


// block

template <class T>
lox::stmt::Block<T>::Block(const std::vector<lox::stmt::Stmt<T>>& statements)
    : statements(statements) {}


template <class T>
const T lox::stmt::Block<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitBlockStmt(*this);
}


const std::vector<Stmt<T>> getStatements() const {
  return statements;
}


// expression

template <class T>
lox::stmt::Expression<T>::Expression(const lox::expr::Expr<T>& expression)
    : expression(expression) {}


template <class T>
const T lox::stmt::Expression<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitExpressionStmt(*this);
}


template <class T>
const lox::expr::Expr<T> getExpression() const {
    return expression;
}


// function

template <class T>
lox::stmt::Function<T>::Function(
    const Token& name,
    const std::vector<Token>& params,
    const std::vector<lox::stmt::Stmt<T>>& body)
    : name(name), params(params), body(body) {}


template <class T>
const T lox::stmt::Function<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitFunctionStmt(*this);
}


// class

template <class T>
lox::stmt::Class<T>::Class(
    const Token& name,
    const lox::expr::Variable<T>& superclass,
    const std::vector<lox::stmt::Function<T>>& methods)
    : name(name), superclass(superclass), methods(methods) {}


template <class T>
const T lox::stmt::Class<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitClassStmt(*this);
}


const Token& getName() const {
  return name;
}


const lox::expr::Variable<T>& getSuperclass() const {
  return superclass;
}


const std::vector<lox::stmt::Function<T>>& getMethods() const {
  return methods;
}


// if

template <class T>
lox::stmt::If<T>::If(
    const lox::expr::Expr<T>& condition,
    const lox::stmt::Stmt<T>* thenBranch,
    const lox::stmt::Stmt<T>* elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}


template <class T>
const T lox::stmt::If<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitIfStmt(*this);
}


const lox::expr::Expr<T>& lox::stmt::If<T>::getCondition() const {
  return condition;
}


const lox::stmt::Stmt<T>& lox::stmt::If<T>::getThenBranch() const {
  return *thenBranch;
}


const lox::stmt::Stmt<T>& lox::stmt::If<T>::getElseBranch() const {
  return *elseBranch;
}


// print

template <class T>
lox::stmt::Print<T>::Print(const lox::expr::Expr<T>& expression)
    : expression(expression) {}


template <class T>
const T lox::stmt::Print<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitPrintStmt(*this);
}


// return

template <class T>
lox::stmt::Return<T>::Return(
    const Token& keyword,
    const lox::expr::Expr<T>& value)
    : keyword(keyword), value(value) {}


template <class T>
const T lox::stmt::Return<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitReturnStmt(*this);
}


// var

template <class T>
lox::stmt::Var<T>::Var(const Token& name, const lox::expr::Expr<T>& initializer)
    : name(name), initializer(initializer) {}


template <class T>
const T lox::stmt::Var<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitVarStmt(*this);
}


// while

template <class T>
lox::stmt::While<T>::While(
    const lox::expr::Expr<T>& condition,
    const lox::stmt::Stmt<T>& body)
    : condition(condition), body(body) {}


template <class T>
const T lox::stmt::While<T>::accept(const Visitor<T>& visitor) const {
  return visitor.visitWhileStmt(*this);
}


// accept

/*
template <class T>
T accept(const Visitor<T>& visitor) {
    return visitor(*this);
}
*/
}  // namespace lox


template class lox::stmt::Block<double>;
template class lox::stmt::Expression<double>;
template class lox::stmt::Function<double>;
template class lox::stmt::Class<double>;
template class lox::stmt::If<double>;
template class lox::stmt::Print<double>;
template class lox::stmt::Return<double>;
template class lox::stmt::Var<double>;
template class lox::stmt::While<double>;

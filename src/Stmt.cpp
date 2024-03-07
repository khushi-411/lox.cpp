#include <vector>

#include "Stmt.h"


namespace lox {


// block

template <class T>
lox::stmt::Block<T>::Block(const std::vector<lox::stmt::Stmt<T>>& statements)
    : statements(statements) {}


template <class T>
T lox::stmt::Block<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitBlockStmt(*this);
}


// class
/*
template <class T>
lox::stmt::Class<T>::Class(
    const Token& name,
    const lox::expr::Variable<T>& superclass,
    const std::vector<lox::stmt::Function<T>>& methods)
    : name(name), superclass(superclass), methods(methods) {}


template <class T>
T lox::stmt::Class<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitorClassStmt(*this);
}
*/

// expression

template <class T>
lox::stmt::Expression<T>::Expression(const lox::expr::Expr<T>& expression)
    : expression(expression) {}


template <class T>
T lox::stmt::Expression<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitExpressionStmt(*this);
}


// function

template <class T>
lox::stmt::Function<T>::Function(
    const Token& name,
    const std::vector<Token>& params,
    const std::vector<lox::stmt::Stmt<T>>& body)
    : name(name), params(params), body(body) {}


template <class T>
T lox::stmt::Function<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitFunctionStmt(*this);
}


// if

template <class T>
lox::stmt::If<T>::If(
    const lox::expr::Expr<T>& condition,
    const lox::stmt::Stmt<T>& thenBranch,
    const lox::stmt::Stmt<T>& elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}


template <class T>
T lox::stmt::If<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitIfStmt(*this);
}


// print

template <class T>
lox::stmt::Print<T>::Print(const lox::expr::Expr<T>& expression)
    : expression(expression) {}


template <class T>
T lox::stmt::Print<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitPrintStmt(*this);
}


// return

template <class T>
lox::stmt::Return<T>::Return(
    const Token& keyword,
    const lox::expr::Expr<T>& value)
    : keyword(keyword), value(value) {}


template <class T>
T lox::stmt::Return<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitReturnStmt(*this);
}


// var

template <class T>
lox::stmt::Var<T>::Var(const Token& name, const lox::expr::Expr<T>& initializer)
    : name(name), initializer(initializer) {}


template <class T>
T lox::stmt::Var<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitVarStmt(*this);
}


// while

template <class T>
lox::stmt::While<T>::While(
    const lox::expr::Expr<T>& condition,
    const lox::stmt::Stmt<T>& body)
    : condition(condition), body(body) {}


template <class T>
T lox::stmt::While<T>::accept(const Visitor<T>& visitor) {
  return visitor.visitWhileStmt(*this);
}

}  // namespace lox

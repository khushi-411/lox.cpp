#include "Stmt.h"


namespace lox {

template <class T>
lox::stmt::Block<T>::Block(
        std::vector<lox::stmt::Stmt> statements)
    : statements(statements) {}

template <class T>
T lox::stmt::Block<T>::accept(Visitor<T> statements) {
    return visitor.visitBlockStmt(*this);
}


template <class T>
lox::stmt::Class<T>::Class(
        Token name,
        lox::expr::Expr::Variable superclass,
        std::vector<lox::stmt::Stmt::Function methods)
    : name(name), superclass(superclass), methods(methods) {}

template <class T>
T lox::stmt::Class<T>::accept(Visitor<T> visitor) {
    return visitor.visitorClassStmt(*this);
}


template <class T>
lox::stmt::Expression<T>::Expression(
        lox::expr::Expr expression)
    : expression(expression) {}

template <class T>
T lox::stmt::Expression<T>::accept(Visitor<T> visitor) {
    return visitor.visitExpressionStmt(*this);
}


template <class T>
lox::stmt::Function<T>::Function(
        Token name,
        std::vector<Token> params,
        std::vector<lox::stmt::Stmt> body)
    : name(name), params(params), body(body) {}

template <class T>
T lox::stmt::Function<T>::accept(Visitor<T> visitor) {
    return visitor.visitFunctionStmt(*this);
}


template <class T>
lox::stmt::If<T>::If(
        lox::expr::Expr condition,
        lox::stmt::Stmt thenBranch,
        lox::stmt::Stmt elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

template <class T>
T lox::stmt::If<T>::accept(Visitor<T> visitor) {
    return visitor.visitIfStmt(*this);
}


template <class T>
lox::stmt::Print<T>::Print(
        lox::expr::Expr expression)
    : expression(expression) {}

template <class T>
T lox::stmt::Print<T>::accept(Visitor<T> visitor) {
    return visitor.visitPrintStmt(*this);
}


template <class T>
lox::stmt::Return<T>::Return(
        Token keyword,
        lox::expr::Expr value)
    : keyword(keyword), value(value) {}

template <class T>
T lox::stmt::Return<T>::accept(Visitor<T> visitor) {
    return visitor.visitReturnStmt(*this);
}


template <class T>
lox::stmt::Var<T>::Var(Token name, lox::expr::Expr initializer)
    : name(name), initializer(initializer) {}

template <class T>
T lox::stmt::Var<T>::accept(Visitor<T> visitor) {
    return visitor.visitVarStmt(*this);
}


template <class T>
lox::stmt::While<T>::While(lox::expr::Expr condition, lox::stmt::Stmt body)
    : condition(condition), body(body) {}

template <class T>
T lox::stmt::While<T>::accept(Visitor<T> visitor) {
    return visitor.visitWhileStmt(*this);
}

}  // namespace lox

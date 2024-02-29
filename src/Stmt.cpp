#include "Stmt.h"


namespace lox {

template <typename R>
Block::Block(std::vector<Stmt::Stmt> statements)
    : statements(statements) {
    R accept(Visitor<R> statements) {
        return visitor.visitBlockStmt(this);
    }
}


template <typename R>
Class::Class(Token::Token name, Expr::Expr::Variable superclass, std::vector<Stmt::Stmt::Function methods)
    : name(name), superclass(superclass), methods(methods) {
    R accept(Visitor<R> visitor) {
        return visitor.visitorClassStmt(this);
    }
}


template <typename R>
Expression::Expression(Expr::Expr expression)
    : expression(expression) {
    R accept(Visitor<R> visitor) {
        return visitor.visitExpressionStmt(this);
    }
}


template <typename R>
Function::Function(Token::Token name, std::vector<Token::Token> params, std::vector<Stmt::Stmt> body)
    : name(name), params(params), body(body) {
    R accept(Visitor<R> visitor) {
        return visitor.visitFunctionStmt(this);
    }
}


template <typename R>
If::If(Expr::Expr condition, Stmt::Stmt thenBranch, Stmt::Stmt elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {
    R accept(Visitor<R> visitor) {
        return visitor.visitIfStmt(this);
    }
}


template <typename R>
Print::Print(Expr::Expr expression)
    : expression(expression) {
    R accept(Visitor<R> visitor) {
        return visitor.visitPrintStmt(this);
    }
}


template <typename R>
Return::Return(Token::Token keyword, Expr::Expr value)
    : keyword(keyword), value(value) {
    R accept(Visitor<R> visitor) {
        return visitor.visitReturnStmt(this);
    }
}


template <typename R>
Var::Var(Token::Token name, Expr::Expr initializer)
    : name(name), initializer(initializer) {
    R accept(Visitor<R> visitor) {
        return visitor.visitVarStmt(this);
    }
}


template <typename R>
While::While(Expr::Expr condition, Stmt::Stmt body)
    : condition(condition), body(body) {
    R accept(Visitor<R> visitor) {
        return visitor.visitWhileStmt(this);
    }
}

}  // namespace lox

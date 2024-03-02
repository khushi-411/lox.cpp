#ifndef STMT_H
#define STMT_H

#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

namespace stmt {

template <class T>
class Stmt {};


template <class T>
class Visitor;

/*template <typename T>
class Visitor<T> {

public:
    virtual T visitBlockStmt(Block stmt);
    virtual T visitClassStmt(Class stmt);
    virtual T visitExpressionStmt(Expression stmt);
    virtual T visitFunctionStmt(Function stmt);
    virtual T visitIfStmt(If stmt);
    virtual T visitPrintStmt(Print stmt);
    virtual T visitReturnStmt(Return stmt);
    virtual T visitVarStmt(Var stmt);
    virtual T visitWhileStmt(While stmt);

};*/


template <typename T>
class Block : public Stmt<T> {

public:
    Block(std::vector<Stmt<T>> statements);

    T accept(Visitor<T> visitor);

    const std::vector<Stmt<T>> statements;
};


template <typename T>
class Class : public Stmt<T> {

public:
    Class(Token, lox::expr::Expr::Variable superclass, std::vector<Stmt::Function> methods);

    T accept(Visitor<T> visitor);

    const Token name;
    const lox::expr::Expr::Variable superclass;
    const std::vector<Stmt::Function> methods;
};


template <class T>
class Expression : public Stmt {
    
public:
    Expression(lox::expr::Expr expression);

    T accept(Visitor<T> visitor);

    const lox::expr::Expr expression;
};


template <class T>
class Function : public Stmt {

public:
    Function(Token name, std::vector<Token> params, std::vector<Stmt> body);

    T accept(Visitor<T> visitor);

    const Token name;
    const std::vector<Token> params;
    const std::vector<Stmt> body;
};


template <class T>
class If : public Stmt {

public:
    If(lox::expr::Expr condition, Stmt thenBranch, Stmt elseBranch);

    T accept(Visitor<T> visitor);

    const lox::expr::Expr condition;
    const Stmt thenBranch;
    const Stmt elseBranch;
};


template <class T>
class Print : public Stmt {

public:
    Print(lox::expr::Expr expression);

    T accept(Visitor<T> visitor);

    const lox::expr::Expr expression;
};


template <class T>
class Return : public Stmt {

public:
    Return(Token keyword, lox::expr::Expr value);

    T accept(Visitor<T> visitor);

    const Token keyword;
    const lox::expr::Expr value;
};


template <class T>
class Var : public Stmt {

public:
    Var(Token name, lox::expr::Expr initializer);

    T accept(Visitor<T> visitor);

    const Token name;
    const lox::expr::Expr initializer;
};


template <class T>
class While : public Stmt {

public:
    While(lox::expr::Expr condition, Stmt body);

    T accept(Visitor<T> visitor);

    const lox::expr::Expr condition;
    const Stmt body;
};


template <class T>
T accept(Visitor<T> visitor);


template <class T>
class Visitor : public Stmt<T> {

public:
    virtual T visitBlockStmt(const Block<T>& stmt) = 0;
    virtual T visitClassStmt(const Class<T>& stmt) = 0;
    virtual T visitExpressionStmt(const Expression<T>& stmt) = 0;
    virtual T visitFunctionStmt(const Function<T>& stmt) = 0;
    virtual T visitIfStmt(const If<T>& stmt) = 0;
    virtual T visitPrintStmt(const Print<T>& stmt) = 0;
    virtual T visitReturnStmt(const Return<T>& stmt) = 0;
    virtual T visitVarStmt(const Var<T>& stmt) = 0;
    virtual T visitWhileStmt(const While<T>& stmt) = 0;

};


}  // namespace stmt

}  // namespace lox

#endif

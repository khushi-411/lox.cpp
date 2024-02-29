#ifndef STMT_H
#define STMT_H

#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

class Stmt {};

template <typename R>
class Visitor<R> {

public:
    virtual R visitBlockStmt(Block stmt);
    virtual R visitClassStmt(Class stmt);
    virtual R visitExpressionStmt(Expression stmt);
    virtual R visitFunctionStmt(Function stmt);
    virtual R visitIfStmt(If stmt);
    virtual R visitPrintStmt(Print stmt);
    virtual R visitReturnStmt(Return stmt);
    virtual R visitVarStmt(Var stmt);
    virtual R visitWhileStmt(While stmt);

};

template <typename R>
class Block : public Stmt {

public:
    Block(std::vector<Stmt> statements);

    R accept(Visitor<R> visitor);

    const std::vector<Stmt> statements;
};


template <typename R>
class Class : public Stmt {

public:
    Class(Token::Token, Expr::Expr::Variable superclass, std::vector<Stmt::Stmt::Function> methods);

    R accept(Visitor<R> visitor);

    const Token::Token name;
    const Expr::Expr::Variable superclass;
    const std::vector<Stmt::Stmt::Function> methods;
};


template <typename R>
class Expression : public Stmt {
    
public:
    Expression(Expr::Expr expression);

    R accept(Visitor<R> visitor);

    const Expr::Expr expression;
};


template <typename R>
class Function : public Stmt {

public:
    Function(Token::Token name, std::vector<Token::Token params, std::vector<Stmt> body);

    R accept(Visitor<R> visitor);

    const Token::Token name;
    const std::vector<Token::Token> params;
    const std::vector<Stmt::Stmt> body;
};


template <typename R>
class If : public Stmt {

public:
    If(Expr::Expr condition, Stmt::Stmt thenBranch, Stmt::Stmt elseBranch);

    R accept(Visitor<R> visitor);

    const Expr::Expr condition;
    const Stmt::Stmt thenBranch;
    const Stmt::Stmt elseBranch;
};


template <typename R>
class Print : public Stmt {

public:
    Print(Expr::Expr expression);

    R accept(Visitor<R> visitor);

    const Expr::Expr expression;
};


template <typename R>
class Return : public Stmt {

public:
    Return(Token::Token keyword, Expr::Expr value);

    R accept(Visitor<R> visitor);

    const Token::Token keyword;
    const Expr::Expr value;
};


template <typename R>
class Var : public Stmt {

public:
    Var(Token::Token name, Expr::Expr initializer);

    R accept(Visitor<R> visitor);

    const Token::Token name;
    const Expr::Expr initializer;
};


template <typename R>
class While : public Stmt {

public:
    While(Expr::Expr condition, Stmt::Stmt body);

    R accept(Visitor<R> visitor);

    const Expr::Expr condition;
    const Stmt::Stmt body;
};


R accept(Visitor<R> visitor);

}  // namespace lox

#endif

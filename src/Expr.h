#ifndef EXPR_H
#define EXPR_H

#include <vector>

#include "Token.h"

namespace lox {

class Expr {};

template <typename R>
class Visitor<R> {
public:
    virtual R visitAssignExpr(Assign expr);
    virtual R visitBinaryExpr(Binary expr);
    virtual R visitCallExpr(Call expr);
    virtual R visitGetExpr(Get expr);
    virtual R visitGroupingExpr(Grouping expr);
    virtual R visitLiteralExpr(Literal expr);
    virtual R visitLogicalExpr(Logical expr);
    virtual R visitSetExpr(Set expr);
    virtual R visitSuperExpr(Super expr);
    virtual R visitThisExpr(This expr);
    virtual R visitUnaryExpr(Unary expr);
    virtual R visitVariableExpr(Variable expr);
}

template <typename R>
class Visitor::Assign : public Expr {
public:
    Visitor::Assign(Token name, Expr value);

    R accept(Visitor<R> visitor);

    const Token name;
    const Expr value;
};


template <typename R>
class Visitor::Binary : public Expr {
public:
    Visitor::Binary(Expr left, Token op, Expr right);

    R accept(Visitor<R> visitor);

    const Expr left;
    const Token op;
    const Expr right;
};


template <typename R>
class Visitor::Call : public Expr {
public:
    Visitor::Call(Expr callee, Token paren, std::vector<Expr> arguments);

    R accept(Visitor<R> visitor);

    const Expr callee;
    const Token paren;
    const std::vector<Expr> arguments;
};


template <typename R>
class Visitor::Get : public Expr {
public:
    Visitor::Get(Expr object, Token name);

    R accept(Visitor<R> visitor);

    const Expr object;
    const Token name;
};


template <typename R>
class Visitor::Grouping : public Expr {
public:
    Visitor::Grouping(Expr expression);

    R accept(Visitor<R> visitor);

    const Expr expression;
};


template <typename R>
class Visitor::Literal : public Expr {
public:
    Visitor::Literal(std::string value);  // TODO: java object

    R accept(Visitor<R> visitor);

    const std::string value;
};


template <typename R>
class Visitor::Logical : public Expr {
public:
    Visitor::Logical(Expr left, Token op, Expr right);

    R accept(Visitor<R> visitor);

    const Expr left;
    const Token op;
    const Expr right;
};


template <typename R>
class Visitor::Set : public Expr {
public:
    Visitor::Set(Expr object, Token name, Expr value);

    R accept(Visitor<R> visitor);

    const Expr object;
    const Token name;
    const Expr value;
};


template <typename R>
class Visitor::Super : public Expr {
public:
    Visitor::Super(Token keyword, Token method);

    R accept(Visitor<R> visitor);

    const Token keyword;
    const Token method;
};


template <typename R>
class Visitor::This : public Expr {
public:
    Visitor::This(Token keyword);

    R accept(Visitor<R> visitor);

    const Token keyword;
};


template <typename R>
class Visitor::Unary : public Expr {
public:
    Visitor::Unary(Token op, Expr right);

    R accept(Visitor<R> visitor);

    const Token op;
    const Expr right;
};


template <typename R>
class Visitor::Variable : public Expr {
public:
    Visitor::Variable(Token name);

    R accept(Visitor<R> visitor);

    const Token name;
};

R accept(Visitor<R> visitor);

}  // namespace lox

#endif

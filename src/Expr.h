#ifndef EXPR_H
#define EXPR_H

#include <vector>

#include "Token.h"

namespace lox {

namespace expr {

// https://en.cppreference.com/w/cpp/language/template_specialization
template <class T>
class Expr {};

template <class T>
class Visitor;

/*template <class T>
class Visitor : public Expr<T> {
public:
    virtual T visitAssignExpr(const Assign<T>& expr) = 0;  // TODO
    virtual T visitBinaryExpr(const Binary<T>& expr) = 0;
    virtual T visitCallExpr(const Call<T>& expr) = 0;
    virtual T visitGetExpr(const Get<T>& expr) = 0;
    virtual T visitGroupingExpr(const Grouping<T>& expr) = 0;
    virtual T visitLiteralExpr(const Literal<T>& expr) = 0;
    virtual T visitLogicalExpr(const Logical<T>& expr) = 0;
    virtual T visitSetExpr(const Set<T>& expr) = 0;
    virtual T visitSuperExpr(const Super<T>& expr) = 0;
    virtual T visitThisExpr(const This<T>& expr) = 0;
    virtual T visitUnaryExpr(const Unary<T>& expr) = 0;
    virtual T visitVariableExpr(const Variable<T>& expr) = 0;
};*/


// TODO: template specialization
template <class T>
class Assign : public Expr<T> {  // TODO: why not Assign<T>
public:
    Assign(Token name, Expr<T> value);

    T accept(Visitor<T>& visitor);

    const Token name;
    const Expr<T> value;
};


template <class T>
class Binary : public Expr<T> {
public:
    Binary(Expr<T> left, Token op, Expr<T> right);

    T accept(Visitor<T>& visitor);

    const Expr<T> left;
    const Token op;
    const Expr<T> right;
};


template <class T>
class Call : public Expr<T> {
public:
    Call(Expr<T> callee, Token paren, std::vector<Expr<T>> arguments);

    T accept(Visitor<T>& visitor);

    const Expr<T> callee;
    const Token paren;
    const std::vector<Expr<T>> arguments;
};


template <class T>
class Get : public Expr<T> {
public:
    Get(Expr<T> object, Token name);

    T accept(Visitor<T>& visitor);

    const Expr<T> object;
    const Token name;
};


template <class T>
class Grouping : public Expr<T> {
public:
    Grouping(Expr<T> expression);

    T accept(Visitor<T>& visitor);

    const Expr<T> expression;
};


template <class T>
class Literal : public Expr<T> {
public:
    Literal(std::string value);  // TODO: java object

    T accept(Visitor<T>& visitor);

    const std::string value;
};


template <class T>
class Logical : public Expr<T> {
public:
    Logical(Expr<T> left, Token op, Expr<T> right);

    T accept(Visitor<T>& visitor);

    const Expr<T> left;
    const Token op;
    const Expr<T> right;
};


template <class T>
class Set : public Expr<T> {
public:
    Set(Expr<T> object, Token name, Expr<T> value);

    T accept(Visitor<T>& visitor);

    const Expr<T> object;
    const Token name;
    const Expr<T> value;
};


template <class T>
class Super : public Expr<T> {
public:
    Super(Token keyword, Token method);

    T accept(Visitor<T>& visitor);

    const Token keyword;
    const Token method;
};


template <class T>
class This : public Expr<T> {
public:
    This(Token keyword);

    T accept(Visitor<T>& visitor);

    const Token keyword;
};


template <class T>
class Unary : public Expr<T> {
public:
    Unary(Token op, Expr<T> right);

    T accept(Visitor<T>& visitor);

    const Token op;
    const Expr<T> right;
};


template <class T>
class Variable : public Expr<T> {
public:
    Variable(Token name);

    T accept(Visitor<T>& visitor);

    const Token name;
};

template <class T>
class Visitor : public Expr<T> {
public:
    virtual T visitAssignExpr(const Assign<T>& expr) = 0;  // TODO
    virtual T visitBinaryExpr(const Binary<T>& expr) = 0;
    virtual T visitCallExpr(const Call<T>& expr) = 0;
    virtual T visitGetExpr(const Get<T>& expr) = 0;
    virtual T visitGroupingExpr(const Grouping<T>& expr) = 0;
    virtual T visitLiteralExpr(const Literal<T>& expr) = 0;
    virtual T visitLogicalExpr(const Logical<T>& expr) = 0;
    virtual T visitSetExpr(const Set<T>& expr) = 0;
    virtual T visitSuperExpr(const Super<T>& expr) = 0;
    virtual T visitThisExpr(const This<T>& expr) = 0;
    virtual T visitUnaryExpr(const Unary<T>& expr) = 0;
    virtual T visitVariableExpr(const Variable<T>& expr) = 0;
};


template <typename T>
T accept(Visitor<T>& visitor);


}  // namespace expr

}  // namespace lox

#endif

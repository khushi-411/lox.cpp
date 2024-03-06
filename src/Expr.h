#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include <string>

#include "Token.h"

namespace lox {

namespace expr {

// https://en.cppreference.com/w/cpp/language/template_specialization
template <class T>
class Expr {};

template <class T>
class Visitor;


// TODO: template specialization
template <class T>
class Assign : public Expr<T> {
public:
    Assign(const Token &name, const Expr<T> &value);

    T accept(const Visitor<T> &visitor);

    const Token name;
    const Expr<T> value;
};


template <class T>
class Binary : public Expr<T> {
public:
    Binary(const Expr<T> &left, const Token &op, const Expr<T> &right);

    T accept(const Visitor<T> &visitor);

    const Expr<T> left;
    const Token op;
    const Expr<T> right;
};


template <class T>
class Call : public Expr<T> {
public:
    Call(const Expr<T> &callee, const Token &paren,
            const std::vector<Expr<T>> &arguments);

    T accept(const Visitor<T> &visitor);

    const Expr<T> callee;
    const Token paren;
    const std::vector<Expr<T>> arguments;
};


template <class T>
class Get : public Expr<T> {
public:
    Get(const Expr<T> &object, const Token &name);

    T accept(const Visitor<T> &visitor);

    const Expr<T> object;
    const Token name;
};


template <class T>
class Grouping : public Expr<T> {
public:
    Grouping(const Expr<T> &expression);

    T accept(const Visitor<T> &visitor);

    const Expr<T> expression;
};


template <class T>
class Literal : public Expr<T> {
public:
    Literal(const std::variant<std::nullptr_t, std::string, double, bool> &value);

    T accept(const Visitor<T> &visitor);

    const std::string value;
};


template <class T>
class Logical : public Expr<T> {
public:
    Logical(const Expr<T> &left, const Token &op, const Expr<T> &right);

    T accept(const Visitor<T>& visitor);

    const Expr<T> left;
    const Token op;
    const Expr<T> right;
};


template <class T>
class Set : public Expr<T> {
public:
    Set(const Expr<T> &object, const Token &name, const Expr<T> &value);

    T accept(const Visitor<T> &visitor);

    const Expr<T> object;
    const Token name;
    const Expr<T> value;
};


template <class T>
class Super : public Expr<T> {
public:
    Super(const Token &keyword, const Token &method);

    T accept(const Visitor<T>& visitor);

    const Token keyword;
    const Token method;
};


template <class T>
class This : public Expr<T> {
public:
    This(const Token &keyword);

    T accept(const Visitor<T>& visitor);

    const Token keyword;
};


template <class T>
class Unary : public Expr<T> {
public:
    Unary(const Token &op, const Expr<T> &right);

    T accept(const Visitor<T>& visitor);

    const Token op;
    const Expr<T> right;
};


template <class T>
class Variable : public Expr<T> {
public:
    Variable(const Token &name);

    T accept(const Visitor<T>& visitor);

    const Token name;
};


template <class T>
class Visitor : public Expr<T> {
public:
    virtual T visitAssignExpr(const Assign<T> &expr) = 0;  // TODO
    virtual T visitBinaryExpr(const Binary<T> &expr) = 0;
    virtual T visitCallExpr(const Call<T> &expr) = 0;
    virtual T visitGetExpr(const Get<T> &expr) = 0;
    virtual T visitGroupingExpr(const Grouping<T> &expr) = 0;
    virtual T visitLiteralExpr(const Literal<T> &expr) = 0;
    virtual T visitLogicalExpr(const Logical<T> &expr) = 0;
    virtual T visitSetExpr(const Set<T> &expr) = 0;
    virtual T visitSuperExpr(const Super<T> &expr) = 0;
    virtual T visitThisExpr(const This<T> &expr) = 0;
    virtual T visitUnaryExpr(const Unary<T> &expr) = 0;
    virtual T visitVariableExpr(const Variable<T> &expr) = 0;
};


template <typename T>
T accept(const Visitor<T>& visitor);


}  // namespace expr

}  // namespace lox

#endif

#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

template <class T>
lox::expr::Assign<T>::Assign(Token name, lox::expr::Expr<T> value)
    : name(name), value(value) {}

template <class T>
T lox::expr::Assign<T>::accept(lox::expr::Visitor<T>& visitor) {
    return visitor.visitAssignExpr(*this);
}


template <class T>
lox::expr::Binary<T>::Binary(lox::expr::Expr<T> left, Token op, lox::expr::Expr<T> right)
    : left(left), op(op), right(right) {}

template <class T>
T lox::expr::Binary<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitBinaryExpr(*this);
}


template <class T>
lox::expr::Call<T>::Call(lox::expr::Expr<T> callee, Token paren, std::vector<lox::expr::Expr<T>> arguments)
    : callee(callee), paren(paren), arguments(arguments) {}

template <class T>
T lox::expr::Call<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitCallExpr(*this);
}


template <class T>
T lox::expr::Get<T>::Get(lox::expr::Expr<T> object, Token name)
    : object(object), name(name) {}

template <class T>
T lox::expr::Get<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitGetExpr(*this);
}


template <class T>
lox::expr::Grouping<T>::Grouping(lox::expr::Expr<T> expression)
    : expression(expression) {}

template <class T>
T lox::expr::Grouping<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitGroupingExpr(*this);
}


template <class T>
lox::expr::Literal<T>::Literal(std::string value)
    : value(value) {}

template <class T>
T lox::expr::Literal<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitLiteralExpr(*this);
}


template <class T>
lox::expr::Logical<T>::Logical(lox::expr::Expr<T> left, Token op, lox::expr::Expr<T> right)
    : left(left), op(op), right(right) {}

template <class T>
T lox::expr::Logical<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitLogicalExpr(*this);
}
}


template <class T>
lox::expr::Set<T>::Set(lox::expr::Expr<T> object, Token name, lox::expr::Expr<T> value)
    : object(object), name(name), value(value) {}

template <class T>
T lox::expr::Set<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitSetExpr(*this);
}


template <class T>
lox::expr::Super<T>::Super(Token keyword, Token method)
    : keyword(keyword), method(method) {}

template <class T>
T lox::expr::Super<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitSuperExpr(*this);
}


template <class T>
lox::expr::This<T>::This(Token keyword)
    : keyword(keyword) {}

template <class T>
T lox::expr::This<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitThisExpr(*this);
}


template <class T>
lox::expr::Unary<T>::Unary(Token op, lox::expr::Expr<T> right)
    : op(op), right(right) {}

template <class T>
T lox::expr::Unary<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitUnaryExpr(*this);
}


template <class T>
lox::expr::Variable<T>::Variable(Token name)
    : name(name) {}

template <class T>
T lox::expr::Variable<T>::accept(lox::expr::Visitor<T> visitor) {
    return visitor.visitVariableExpr(*this);
}

}  // namespace lox

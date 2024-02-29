#include <vector>

#include "Expr.h"

namespace lox {

template <typename R>
Assign::Assign(Token name, Expr value)
    : name(name), value(value) {
    R accept(Visitor<R> visitor) {
        return visitor.visitAssignExpr(this);
    }
}


template <typename R>
Binary::Binary(Expr left, Token op, Expr right)
    : left(left), op(op), right(right) {
    R accept(Visitor<R> visitor) {
        return visitor.visitBinaryExpr(this);
    }
}


template <typename R>
Call::Call(Expr callee, Token paren, std::vector<Expr> arguments)
    : callee(callee), paren(paren), arguments(arguments) {
    R accept(Visitor<R> visitor) {
        return visitor.visitCallExpr(this);
    }
}


template <typename R>
Get::Get(Expr object, Token name)
    : object(object), name(name) {
    R accept(Visitor<R> visitor) {
        return visitor.visitGetExpr(this);
    }
}


template <typename R>
Grouping::Grouping(Expr expression)
    : expression(expression) {
    R accept(Visitor<R> visitor) {
        return visitor.visitGroupingExpr(this);
    }
}


template <typename R>
Literal::Literal(std::string value)
    : value(value) {
    R accept(Visitor<R> visitor) {
        return visitor.visitLiteralExpr(this);
    }
}


template <typename R>
Logical::Logical(Expr left, Token op, Expr right)
    : left(left), op(op), right(right) {
    R accept(Visitor<R> visitor) {
        return visitor.visitLogicalExpr(this);
    }
}


template <typename R>
Set::Set(Expr object, Token name, Expr value)
    : object(object), name(name), value(value) {
    R accept(Visitor<R> visitor) {
        return visitor.visitSetExpr(this);
    }
}


template <typename R>
Super::Super(Token keyword, Token method)
    : keyword(keyword), method(method) {
    R accept(Visitor<R> visitor) {
        return visitor.visitSuperExpr(this);
    }
}


template <typename R>
This::This(Token keyword)
    : keyword(keyword) {
    R accept(Visitor<R> visitor) {
        return visitor.visitThisExpr(this);
    }
}


template <typename R>
Unary::Unary(Token op, Expr right)
    : op(op), right(right) {
    R accept(Visitor<R> visitor) {
        return visitor.visitUnaryExpr(this);
    }
}


template <typename R>
Variable::Variable(Token name)
    : name(name) {
    R accept(Visitor<R> visitor) {
        return visitor.visitVariableExpr(this);
    }
}

}  // namespace lox

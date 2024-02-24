#include "Expr.h"

namespace lox {

Assign::Assign(Token name, Expr value)
    : name(name), value(value) {
    R accept(Visitor<R> visitor) {
        return visitor.visitAssignExpr(this);
    }
}


Binary::Binary(Expr left, Token op, Expr right)
    : left(left), op(op), right(right) {
    R accept(Visitor<R> visitor) {
        return visitor.visitBinaryExpr(this);
    }
}


}  // namespace lox

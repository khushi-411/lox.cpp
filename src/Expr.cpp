#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Expr.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

namespace expr {


// assign

Assign::Assign(const Token& name, std::shared_ptr<Expr> value)
    : name(name), value(std::move(value)) {}


Object Assign::accept(ExprVisitor& visitor) const {
  return visitor.visitAssignExpr(*this);
}


// binary

Binary::Binary(
    std::shared_ptr<Expr> left,
    const Token& op,
    std::shared_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}


Object Binary::accept(ExprVisitor& visitor) const {
  return visitor.visitBinaryExpr(*this);
}


// call

Call::Call(
    std::shared_ptr<Expr> callee,
    const Token& paren,
    std::vector<std::shared_ptr<Expr>> arguments)
    : callee(std::move(callee)),
      paren(paren),
      arguments(std::move(arguments)) {}


Object Call::accept(ExprVisitor& visitor) const {
  return visitor.visitCallExpr(*this);
}


// get

Get::Get(std::shared_ptr<Expr> object, const Token& name)
    : object(std::move(object)), name(name) {}


Object Get::accept(ExprVisitor& visitor) const {
  return visitor.visitGetExpr(*this);
}


// grouping

Grouping::Grouping(std::shared_ptr<Expr> expression)
    : expression(std::move(expression)) {}


Object Grouping::accept(ExprVisitor& visitor) const {
  return visitor.visitGroupingExpr(*this);
}


// literal

Literal::Literal(const Object& value) : value(value) {}


Object Literal::accept(ExprVisitor& visitor) const {
  return visitor.visitLiteralExpr(*this);
}


// logical

Logical::Logical(
    std::shared_ptr<Expr> left,
    const Token& op,
    std::shared_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}


Object Logical::accept(ExprVisitor& visitor) const {
  return visitor.visitLogicalExpr(*this);
}


// set

Set::Set(
    std::shared_ptr<Expr> object,
    const Token& name,
    std::shared_ptr<Expr> value)
    : object(std::move(object)), name(name), value(std::move(value)) {}


Object Set::accept(ExprVisitor& visitor) const {
  return visitor.visitSetExpr(*this);
}


// super

Super::Super(const Token& keyword, const Token& method)
    : keyword(keyword), method(method) {}


Object Super::accept(ExprVisitor& visitor) const {
  return visitor.visitSuperExpr(*this);
}


// this

This::This(const Token& keyword) : keyword(keyword) {}


Object This::accept(ExprVisitor& visitor) const {
  return visitor.visitThisExpr(*this);
}


// unary

Unary::Unary(const Token& op, std::shared_ptr<Expr> right)
    : op(op), right(std::move(right)) {}


Object Unary::accept(ExprVisitor& visitor) const {
  return visitor.visitUnaryExpr(*this);
}


// variable

Variable::Variable(const Token& name) : name(name) {}


Object Variable::accept(ExprVisitor& visitor) const {
  return visitor.visitVariableExpr(*this);
}


}  // namespace expr

}  // namespace lox

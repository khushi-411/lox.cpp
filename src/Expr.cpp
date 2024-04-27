#include <string>
#include <variant>
#include <vector>

#include "Expr.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {


// assign

lox::expr::Assign::Assign(const Token& name, const lox::expr::Expr& value)
    : name(name), value(value) {}


template <class T>
const T lox::expr::Assign::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitAssignExpr(*this);
}


// binary

lox::expr::Binary::Binary(
    const lox::expr::Expr& left,
    const Token& op,
    const lox::expr::Expr& right)
    : left(left), op(op), right(right) {}


template <class T>
const T lox::expr::Binary::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitBinaryExpr(*this);
}


// call
/*
lox::expr::Call::Call(
    const lox::expr::Expr& callee,
    const Token& paren,
    const std::vector<lox::expr::Expr>& arguments)
    : callee(callee), paren(paren), arguments(arguments) {}


template <class T>
const T lox::expr::Call::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitCallExpr(*this);
}


// get

lox::expr::Get::Get(const lox::expr::Expr& object, const Token& name)
    : object(object), name(name) {}


template <class T>
const T lox::expr::Get::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitGetExpr(*this);
}
*/

// grouping

lox::expr::Grouping::Grouping(const lox::expr::Expr& expression)
    : expression(expression) {}


template <class T>
const T lox::expr::Grouping::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitGroupingExpr(*this);
}


// literal

lox::expr::Literal::Literal(const Object& value)
    : value([&value]() {
        if (const auto* str = std::get_if<std::string>(&value)) {
          return *str;
        } else if (const auto* dbl = std::get_if<double>(&value)) {
          return std::to_string(*dbl);
        } else if (const auto* b = std::get_if<bool>(&value)) {
          return std::string(*b ? "true" : "false");
        } else {
          return std::string("null");
        }
      }()) {}


template <class T>
const T lox::expr::Literal::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitLiteralExpr(*this);
}


// logical

lox::expr::Logical::Logical(
    const lox::expr::Expr& left,
    const Token& op,
    const lox::expr::Expr& right)
    : left(left), op(op), right(right) {}


template <class T>
const T lox::expr::Logical::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitLogicalExpr(*this);
}

/*
// set

lox::expr::Set::Set(
    const lox::expr::Expr& object,
    const Token& name,
    const lox::expr::Expr& value)
    : object(object), name(name), value(value) {}


template <class T>
const T lox::expr::Set::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitSetExpr(*this);
}


// super

lox::expr::Super::Super(const Token& keyword, const Token& method)
    : keyword(keyword), method(method) {}


template <class T>
const T lox::expr::Super::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitSuperExpr(*this);
}


// this

lox::expr::This::This(const Token& keyword) : keyword(keyword) {}


template <class T>
const T lox::expr::This::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitThisExpr(*this);
}
*/

// unary

lox::expr::Unary::Unary(const Token& op, const lox::expr::Expr& right)
    : op(op), right(right) {}


template <class T>
const T lox::expr::Unary::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitUnaryExpr(*this);
}


// variable

lox::expr::Variable::Variable(const Token& name) : name(name) {}


template <class T>
const T lox::expr::Variable::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitVariableExpr(*this);
}

}  // namespace lox

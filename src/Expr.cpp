#include <string>
#include <variant>
#include <vector>

#include "Expr.h"
#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {


// assign

template <class T>
lox::expr::Assign<T>::Assign(const Token& name, const lox::expr::Expr<T>& value)
    : name(name), value(value) {}


template <class T>
const T lox::expr::Assign<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitAssignExpr(*this);
}


// binary

template <class T>
lox::expr::Binary<T>::Binary(
    const lox::expr::Expr<T>& left,
    const Token& op,
    const lox::expr::Expr<T>& right)
    : left(left), op(op), right(right) {}


template <class T>
const T lox::expr::Binary<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitBinaryExpr(*this);
}


// call

template <class T>
lox::expr::Call<T>::Call(
    const lox::expr::Expr<T>& callee,
    const Token& paren,
    const std::vector<lox::expr::Expr<T>>& arguments)
    : callee(callee), paren(paren), arguments(arguments) {}


template <class T>
const T lox::expr::Call<T>::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitCallExpr(*this);
}


// get

template <class T>
lox::expr::Get<T>::Get(const lox::expr::Expr<T>& object, const Token& name)
    : object(object), name(name) {}


template <class T>
const T lox::expr::Get<T>::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitGetExpr(*this);
}


// grouping

template <class T>
lox::expr::Grouping<T>::Grouping(const lox::expr::Expr<T>& expression)
    : expression(expression) {}


template <class T>
const T lox::expr::Grouping<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitGroupingExpr(*this);
}


// literal

template <class T>
lox::expr::Literal<T>::Literal(const Object& value)
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
const T lox::expr::Literal<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitLiteralExpr(*this);
}


// logical

template <class T>
lox::expr::Logical<T>::Logical(
    const lox::expr::Expr<T>& left,
    const Token& op,
    const lox::expr::Expr<T>& right)
    : left(left), op(op), right(right) {}


template <class T>
const T lox::expr::Logical<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitLogicalExpr(*this);
}


// set

template <class T>
lox::expr::Set<T>::Set(
    const lox::expr::Expr<T>& object,
    const Token& name,
    const lox::expr::Expr<T>& value)
    : object(object), name(name), value(value) {}


template <class T>
const T lox::expr::Set<T>::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitSetExpr(*this);
}


// super

template <class T>
lox::expr::Super<T>::Super(const Token& keyword, const Token& method)
    : keyword(keyword), method(method) {}


template <class T>
const T lox::expr::Super<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitSuperExpr(*this);
}


// this

template <class T>
lox::expr::This<T>::This(const Token& keyword) : keyword(keyword) {}


template <class T>
const T lox::expr::This<T>::accept(const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitThisExpr(*this);
}


// unary

template <class T>
lox::expr::Unary<T>::Unary(const Token& op, const lox::expr::Expr<T>& right)
    : op(op), right(right) {}


template <class T>
const T lox::expr::Unary<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitUnaryExpr(*this);
}


// variable

template <class T>
lox::expr::Variable<T>::Variable(const Token& name) : name(name) {}


template <class T>
const T lox::expr::Variable<T>::accept(
    const lox::expr::Visitor<T>& visitor) const {
  return visitor.visitVariableExpr(*this);
}

}  // namespace lox


template class lox::expr::Assign<double>;
template class lox::expr::Binary<double>;
template class lox::expr::Call<double>;
template class lox::expr::Get<double>;
template class lox::expr::Grouping<double>;
template class lox::expr::Literal<double>;
template class lox::expr::Logical<double>;
template class lox::expr::Set<double>;
template class lox::expr::Super<double>;
template class lox::expr::This<double>;
template class lox::expr::Unary<double>;
template class lox::expr::Variable<double>;

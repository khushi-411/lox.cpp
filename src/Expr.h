#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <variant>
#include <vector>

#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

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
 private:
  const Token name;
  const Expr<T> value;

 public:
  Assign(const Token& name, const Expr<T>& value);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Binary : public Expr<T> {
 private:
  const Expr<T> left;
  const Token op;
  const Expr<T> right;

 public:
  Binary(const Expr<T>& left, const Token& op, const Expr<T>& right);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Call : public Expr<T> {
 private:
  const Expr<T> callee;
  const Token paren;
  const std::vector<Expr<T>> arguments;

 public:
  Call(
      const Expr<T>& callee,
      const Token& paren,
      const std::vector<Expr<T>>& arguments);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Get : public Expr<T> {
 private:
  const Expr<T> object;
  const Token name;

 public:
  Get(const Expr<T>& object, const Token& name);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Grouping : public Expr<T> {
 private:
  const Expr<T> expression;

 public:
  Grouping(const Expr<T>& expression);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Literal : public Expr<T> {
 private:
  const std::string value;

 public:
  Literal(const Object& value);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Logical : public Expr<T> {
 private:
  const Expr<T> left;
  const Token op;
  const Expr<T> right;

 public:
  Logical(const Expr<T>& left, const Token& op, const Expr<T>& right);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Set : public Expr<T> {
 private:
  const Expr<T> object;
  const Token name;
  const Expr<T> value;

 public:
  Set(const Expr<T>& object, const Token& name, const Expr<T>& value);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Super : public Expr<T> {
 private:
  const Token keyword;
  const Token method;

 public:
  Super(const Token& keyword, const Token& method);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class This : public Expr<T> {
 private:
  const Token keyword;

 public:
  This(const Token& keyword);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Unary : public Expr<T> {
 private:
  const Token op;
  const Expr<T> right;

 public:
  Unary(const Token& op, const Expr<T>& right);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Variable : public Expr<T> {
 private:
  const Token name;

 public:
  Variable(const Token& name);

  T accept(const Visitor<T>& visitor);
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
T accept(const Visitor<T>& visitor);


}  // namespace expr

}  // namespace lox

#endif

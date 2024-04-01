#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <variant>
#include <vector>

#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

namespace expr {


template <class T>
class Visitor;


// https://en.cppreference.com/w/cpp/language/template_specialization
template <class T>
class Expr {
 public:
  friend bool operator==(const Expr<T>& _x, const Expr<T>& _y) {
    return _x == _y;
  }

  //  virtual Object accept(const Visitor<Object>& visitor) const = 0;
};

// template <class T>
// class Visitor;


// TODO: template specialization
template <class T>
class Assign : public Expr<T> {
 private:
  const Token& name;
  const Expr<T>& value;

 public:
  Assign(const Token& name, const Expr<T>& value);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Binary : public Expr<T> {
 private:
  const Expr<T>& left;
  const Token& op;
  const Expr<T>& right;

 public:
  Binary(const Expr<T>& left, const Token& op, const Expr<T>& right);

  const T accept(const Visitor<T>& visitor) const;

  const Expr<T>& getLeft() const {
    return left;
  }

  const Token& getOp() const {
    return op;
  }

  const Expr<T>& getRight() const {
    return right;
  }
};


template <class T>
class Call : public Expr<T> {
 private:
  const Expr<T>& callee;
  const Token& paren;
  const std::vector<Expr<T>>& arguments;

 public:
  Call(
      const Expr<T>& callee,
      const Token& paren,
      const std::vector<Expr<T>>& arguments);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Get : public Expr<T> {
 private:
  const Expr<T>& object;
  const Token& name;

 public:
  Get(const Expr<T>& object, const Token& name);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Grouping : public Expr<T> {
 private:
  const Expr<T>& expression;

 public:
  Grouping(const Expr<T>& expression);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Literal : public Expr<T> {
 private:
  const std::string& value;

 public:
  Literal(const Object& value);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Logical : public Expr<T> {
 private:
  const Expr<T>& left;
  const Token& op;
  const Expr<T>& right;

 public:
  Logical(const Expr<T>& left, const Token& op, const Expr<T>& right);

  const T accept(const Visitor<T>& visitor) const;

  const Token& getOp() const {
    return op;
  }
};


template <class T>
class Set : public Expr<T> {
 private:
  const Expr<T>& object;
  const Token& name;
  const Expr<T>& value;

 public:
  Set(const Expr<T>& object, const Token& name, const Expr<T>& value);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Super : public Expr<T> {
 private:
  const Token& keyword;
  const Token& method;

 public:
  Super(const Token& keyword, const Token& method);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class This : public Expr<T> {
 private:
  const Token& keyword;

 public:
  This(const Token& keyword);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class Unary : public Expr<T> {
 private:
  const Token& op;
  const Expr<T>& right;

 public:
  Unary(const Token& op, const Expr<T>& right);

  const T accept(const Visitor<T>& visitor) const;

  const Token& getOp() const {
    return op;
  }

  const Expr<T>& getRight() const {
    return right;
  }
};


template <class T>
class Variable : public Expr<T> {
 private:
  const Token& name;

 public:
  Variable(const Token& name);

  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const {
    return name;
  }
};


template <class T>
class Visitor : public Expr<T> {
 public:
  virtual T visitAssignExpr(const Assign<T>& expr) const = 0;
  virtual T visitBinaryExpr(const Binary<T>& expr) const = 0;
  virtual T visitCallExpr(const Call<T>& expr) const = 0;
  virtual T visitGetExpr(const Get<T>& expr) const = 0;
  virtual T visitGroupingExpr(const Grouping<T>& expr) const = 0;
  virtual T visitLiteralExpr(const Literal<T>& expr) const = 0;
  virtual T visitLogicalExpr(const Logical<T>& expr) const = 0;
  virtual T visitSetExpr(const Set<T>& expr) const = 0;
  virtual T visitSuperExpr(const Super<T>& expr) const = 0;
  virtual T visitThisExpr(const This<T>& expr) const = 0;
  virtual T visitUnaryExpr(const Unary<T>& expr) const = 0;
  virtual T visitVariableExpr(const Variable<T>& expr) const = 0;
};


// template <typename T>
// T accept(const Visitor<T>& visitor);


}  // namespace expr

}  // namespace lox

#endif

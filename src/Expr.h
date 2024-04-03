#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <variant>
#include <vector>

#include "Token.h"


using Object = std::variant<std::nullptr_t, std::string, double, bool>;

namespace lox {

namespace expr {


// forward declaration

template <class T>
class Visitor;


// expr class

class Expr {
 public:
  friend bool operator==(const Expr& _x, const Expr& _y) {
    return _x == _y;
  }

  bool operator==(const std::nullptr_t& _y) {
    return *this == _y;
  }

  friend bool operator!=(const Expr& _x, const Expr& _y) {
    return _x != _y;
  }

  bool operator!=(const std::nullptr_t& _y) {
    return !(*this == _y);
  }

  Expr& operator=(const std::nullptr_t&) {
    return *this;
  }

  Expr& operator=(const Expr&) {
    return *this;
  }

  template <class T>
  T accept(const Visitor<T>& visitor) const;
};


// TODO: template specialization
// https://en.cppreference.com/w/cpp/language/template_specialization

// assign expr

class Assign : public Expr {
 private:
  const Token& name;
  const Expr& value;

 public:
  Assign(const Token& name, const Expr& value);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const {
    return name;
  }

  const Expr& getValue() const {
    return value;
  }
};


// binary expr

class Binary : public Expr {
 private:
  const Expr& left;
  const Token& op;
  const Expr& right;

 public:
  Binary(const Expr& left, const Token& op, const Expr& right);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Expr& getLeft() const {
    return left;
  }

  const Token& getOp() const {
    return op;
  }

  const Expr& getRight() const {
    return right;
  }
};


// call expr

class Call : public Expr {
 private:
  const Expr& callee;
  const Token& paren;
  const std::vector<Expr>& arguments;

 public:
  Call(
      const Expr& callee,
      const Token& paren,
      const std::vector<Expr>& arguments);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Expr& getCallee() const {
    return callee;
  }

  const Token& getParen() const {
    return paren;
  }

  const std::vector<Expr>& getArguments() const {
    return arguments;
  }
};


// get expr

class Get : public Expr {
 private:
  const Expr& object;
  const Token& name;

 public:
  Get(const Expr& object, const Token& name);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Expr& getObject() const {
    return object;
  }

  const Token& getName() const {
    return name;
  }
};


// grouping expr

class Grouping : public Expr {
 private:
  const Expr& expression;

 public:
  Grouping(const Expr& expression);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Expr& getExpression() const {
    return expression;
  }
};


// literal expr

class Literal : public Expr {
 private:
  const std::string& value;

 public:
  Literal(const Object& value);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const std::string& getValue() const {
    return value;
  }
};


// logical expr

class Logical : public Expr {
 private:
  const Expr& left;
  const Token& op;
  const Expr& right;

 public:
  Logical(const Expr& left, const Token& op, const Expr& right);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Expr& getLeft() const {
    return left;
  }

  const Token& getOp() const {
    return op;
  }

  const Expr& getRight() const {
    return right;
  }
};


// set expr

class Set : public Expr {
 private:
  const Expr& object;
  const Token& name;
  const Expr& value;

 public:
  Set(const Expr& object, const Token& name, const Expr& value);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Expr& getObject() const {
    return object;
  }

  const Token& getName() const {
    return name;
  }

  const Expr& getValue() const {
    return value;
  }
};


// super expr

class Super : public Expr {
 private:
  const Token& keyword;
  const Token& method;

 public:
  Super(const Token& keyword, const Token& method);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getKeyword() const {
    return keyword;
  }

  const Token& getMethod() const {
    return method;
  }
};


// this expr

class This : public Expr {
 private:
  const Token& keyword;

 public:
  This(const Token& keyword);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getKeyword() const {
    return keyword;
  }
};


// unary expr

class Unary : public Expr {
 private:
  const Token& op;
  const Expr& right;

 public:
  Unary(const Token& op, const Expr& right);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getOp() const {
    return op;
  }

  const Expr& getRight() const {
    return right;
  }
};


// variable expr

class Variable : public Expr {
 private:
  const Token& name;

 public:
  Variable(const Token& name);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const {
    return name;
  }
};


// visitor class

template <class T>
class Visitor : public Expr {
 public:
  virtual T visitAssignExpr(const Assign& expr) const = 0;
  virtual T visitBinaryExpr(const Binary& expr) const = 0;
  virtual T visitCallExpr(const Call& expr) const = 0;
  virtual T visitGetExpr(const Get& expr) const = 0;
  virtual T visitGroupingExpr(const Grouping& expr) const = 0;
  virtual T visitLiteralExpr(const Literal& expr) const = 0;
  virtual T visitLogicalExpr(const Logical& expr) const = 0;
  virtual T visitSetExpr(const Set& expr) const = 0;
  virtual T visitSuperExpr(const Super& expr) const = 0;
  virtual T visitThisExpr(const This& expr) const = 0;
  virtual T visitUnaryExpr(const Unary& expr) const = 0;
  virtual T visitVariableExpr(const Variable& expr) const = 0;
};


}  // namespace expr

}  // namespace lox

#endif

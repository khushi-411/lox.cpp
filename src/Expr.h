#pragma once

#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "Token.h"


// Forward declarations for Lox runtime objects
namespace lox {
class LoxCallable;
class LoxInstance;
}  // namespace lox

using Object = std::variant<std::nullptr_t, std::string, double, bool,
    std::shared_ptr<lox::LoxCallable>, std::shared_ptr<lox::LoxInstance>>;

namespace lox {

namespace expr {


// forward declarations

class Expr;
class Assign;
class Binary;
class Call;
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class Super;
class This;
class Unary;
class Variable;


// visitor interface

class ExprVisitor {
 public:
  virtual ~ExprVisitor() = default;

  virtual Object visitAssignExpr(const Assign& expr) = 0;
  virtual Object visitBinaryExpr(const Binary& expr) = 0;
  virtual Object visitCallExpr(const Call& expr) = 0;
  virtual Object visitGetExpr(const Get& expr) = 0;
  virtual Object visitGroupingExpr(const Grouping& expr) = 0;
  virtual Object visitLiteralExpr(const Literal& expr) = 0;
  virtual Object visitLogicalExpr(const Logical& expr) = 0;
  virtual Object visitSetExpr(const Set& expr) = 0;
  virtual Object visitSuperExpr(const Super& expr) = 0;
  virtual Object visitThisExpr(const This& expr) = 0;
  virtual Object visitUnaryExpr(const Unary& expr) = 0;
  virtual Object visitVariableExpr(const Variable& expr) = 0;
};


// expr base class

class Expr {
 public:
  virtual ~Expr() = default;
  virtual Object accept(ExprVisitor& visitor) const = 0;
};


// assign expr

class Assign : public Expr {
 private:
  Token name;
  std::shared_ptr<Expr> value;

 public:
  Assign(const Token& name, std::shared_ptr<Expr> value);

  Object accept(ExprVisitor& visitor) const override;

  const Token& getName() const {
    return name;
  }

  const std::shared_ptr<Expr>& getValue() const {
    return value;
  }
};


// binary expr

class Binary : public Expr {
 private:
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

 public:
  Binary(std::shared_ptr<Expr> left, const Token& op, std::shared_ptr<Expr> right);

  Object accept(ExprVisitor& visitor) const override;

  const std::shared_ptr<Expr>& getLeft() const {
    return left;
  }

  const Token& getOp() const {
    return op;
  }

  const std::shared_ptr<Expr>& getRight() const {
    return right;
  }
};


// call expr

class Call : public Expr {
 private:
  std::shared_ptr<Expr> callee;
  Token paren;
  std::vector<std::shared_ptr<Expr>> arguments;

 public:
  Call(
      std::shared_ptr<Expr> callee,
      const Token& paren,
      std::vector<std::shared_ptr<Expr>> arguments);

  Object accept(ExprVisitor& visitor) const override;

  const std::shared_ptr<Expr>& getCallee() const {
    return callee;
  }

  const Token& getParen() const {
    return paren;
  }

  const std::vector<std::shared_ptr<Expr>>& getArguments() const {
    return arguments;
  }
};


// get expr

class Get : public Expr {
 private:
  std::shared_ptr<Expr> object;
  Token name;

 public:
  Get(std::shared_ptr<Expr> object, const Token& name);

  Object accept(ExprVisitor& visitor) const override;

  const std::shared_ptr<Expr>& getObject() const {
    return object;
  }

  const Token& getName() const {
    return name;
  }
};


// grouping expr

class Grouping : public Expr {
 private:
  std::shared_ptr<Expr> expression;

 public:
  Grouping(std::shared_ptr<Expr> expression);

  Object accept(ExprVisitor& visitor) const override;

  const std::shared_ptr<Expr>& getExpression() const {
    return expression;
  }
};


// literal expr

class Literal : public Expr {
 private:
  Object value;

 public:
  Literal(const Object& value);

  Object accept(ExprVisitor& visitor) const override;

  const Object& getValue() const {
    return value;
  }
};


// logical expr

class Logical : public Expr {
 private:
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

 public:
  Logical(std::shared_ptr<Expr> left, const Token& op, std::shared_ptr<Expr> right);

  Object accept(ExprVisitor& visitor) const override;

  const std::shared_ptr<Expr>& getLeft() const {
    return left;
  }

  const Token& getOp() const {
    return op;
  }

  const std::shared_ptr<Expr>& getRight() const {
    return right;
  }
};


// set expr

class Set : public Expr {
 private:
  std::shared_ptr<Expr> object;
  Token name;
  std::shared_ptr<Expr> value;

 public:
  Set(std::shared_ptr<Expr> object, const Token& name, std::shared_ptr<Expr> value);

  Object accept(ExprVisitor& visitor) const override;

  const std::shared_ptr<Expr>& getObject() const {
    return object;
  }

  const Token& getName() const {
    return name;
  }

  const std::shared_ptr<Expr>& getValue() const {
    return value;
  }
};


// super expr

class Super : public Expr {
 private:
  Token keyword;
  Token method;

 public:
  Super(const Token& keyword, const Token& method);

  Object accept(ExprVisitor& visitor) const override;

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
  Token keyword;

 public:
  This(const Token& keyword);

  Object accept(ExprVisitor& visitor) const override;

  const Token& getKeyword() const {
    return keyword;
  }
};


// unary expr

class Unary : public Expr {
 private:
  Token op;
  std::shared_ptr<Expr> right;

 public:
  Unary(const Token& op, std::shared_ptr<Expr> right);

  Object accept(ExprVisitor& visitor) const override;

  const Token& getOp() const {
    return op;
  }

  const std::shared_ptr<Expr>& getRight() const {
    return right;
  }
};


// variable expr

class Variable : public Expr {
 private:
  Token name;

 public:
  Variable(const Token& name);

  Object accept(ExprVisitor& visitor) const override;

  const Token& getName() const {
    return name;
  }
};


}  // namespace expr

}  // namespace lox

#endif

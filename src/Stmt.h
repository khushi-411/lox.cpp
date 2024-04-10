#pragma once

#ifndef STMT_H
#define STMT_H

#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

namespace stmt {


// forward declaration

template <class T>
class Visitor;


// stmt class

class Stmt {
 public:
  friend bool operator==(const Stmt& _x, const Stmt& _y) {
    return _x == _y;
  }

  bool operator==(const std::nullptr_t& _y) const {
    return *this == _y;
  }

  friend bool operator!=(const Stmt& _x, const Stmt& _y) {
    return _x != _y;
  }

  bool operator!=(const std::nullptr_t& _y) const {
    return !(*this == _y);
  }

  Stmt& operator=(const std::nullptr_t&) {
    return *this;
  }

  Stmt& operator=(const Stmt&) {
    return *this;
  }

  template <class T>
  T accept(const Visitor<T>& visitor) const;
};


// block stmt

class Block : public Stmt {
 private:
  const std::vector<Stmt>& statements;

 public:
  Block(const std::vector<Stmt>& statements);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const std::vector<Stmt>& getStatements() const;
};


// expression stmt

class Expression : public Stmt {
 private:
  const lox::expr::Expr& expression;

 public:
  Expression(const lox::expr::Expr& expression);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr& getExpression() const;
};


// function stmt

class Function : public Stmt {
 private:
  const Token& name;
  const std::vector<Token>& params;
  const std::vector<Stmt>& body;

 public:
  Function(
      const Token& name,
      const std::vector<Token>& params,
      const std::vector<Stmt>& body);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const;
  const std::vector<Token>& getParams() const;
  const std::vector<Stmt>& getBody() const;
};


// class stmt

class Class : public Stmt {
 private:
  const Token& name;
  const lox::expr::Variable& superclass;
  const std::vector<lox::stmt::Function>& methods;

 public:
  Class(
      const Token& name,
      const lox::expr::Variable& superclass,
      const std::vector<lox::stmt::Function>& methods);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const;
  const lox::expr::Variable& getSuperclass() const;
  const std::vector<lox::stmt::Function>& getMethods() const;
};


// if stmt

class If : public Stmt {
 private:
  const lox::expr::Expr& condition;
  const Stmt* thenBranch;
  const Stmt* elseBranch;

 public:
  If(const lox::expr::Expr& condition,
     const Stmt* thenBranch,
     const Stmt* elseBranch);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr& getCondition() const;
  const Stmt& getThenBranch() const;
  const Stmt& getElseBranch() const;
};


// print stmt

class Print : public Stmt {
 private:
  const lox::expr::Expr& expression;

 public:
  Print(const lox::expr::Expr& expression);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr& getExpression() const;
};


// return stmt

class Return : public Stmt {
 private:
  const Token& keyword;
  const lox::expr::Expr& value;

 public:
  Return(const Token& keyword, const lox::expr::Expr& value);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getKeyword() const;
  const lox::expr::Expr& getValue() const;
};


// var stmt

class Var : public Stmt {
 private:
  const Token& name;
  const lox::expr::Expr& initializer;

 public:
  Var(const Token& name, const lox::expr::Expr& initializer);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const;
  const lox::expr::Expr& getInitializer() const;
};


// while stmt

class While : public Stmt {
 private:
  const lox::expr::Expr& condition;
  const Stmt& body;

 public:
  While(const lox::expr::Expr& condition, const Stmt& body);

  template <class T>
  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr& getCondition() const;
  const Stmt& getBody() const;
};


// visitor class

template <class T>
class Visitor : public Stmt {
 public:
  virtual T visitBlockStmt(const Block& stmt) const = 0;
  virtual T visitClassStmt(const Class& stmt) const = 0;
  virtual T visitExpressionStmt(const Expression& stmt) const = 0;
  virtual T visitFunctionStmt(const Function& stmt) const = 0;
  virtual T visitIfStmt(const If& stmt) const = 0;
  virtual T visitPrintStmt(const Print& stmt) const = 0;
  virtual T visitReturnStmt(const Return& stmt) const = 0;
  virtual T visitVarStmt(const Var& stmt) const = 0;
  virtual T visitWhileStmt(const While& stmt) const = 0;
};


}  // namespace stmt

}  // namespace lox


#endif

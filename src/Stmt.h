#ifndef STMT_H
#define STMT_H

#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

namespace stmt {

template <class T>
class Visitor;


template <class T>
class Stmt {
 public:
  virtual void accept(const Visitor<void>& visitor) const = 0;
};


// template <class T>
// class Visitor;


template <typename T>
class Block : public Stmt<T> {
 private:
  const std::vector<Stmt<T>>& statements;

 public:
  Block(const std::vector<Stmt<T>>& statements);

  const T accept(const Visitor<T>& visitor) const;
  const std::vector<Stmt<T>>& getStatements() const;
};


template <class T>
class Expression : public Stmt<T> {
 private:
  const lox::expr::Expr<T>& expression;

 public:
  Expression(const lox::expr::Expr<T>& expression);

  const T accept(const Visitor<T>& visitor) const;
  const lox::expr::Expr<T>& getExpression() const;
};


template <class T>
class Function : public Stmt<T> {
 private:
  const Token& name;
  const std::vector<Token>& params;
  const std::vector<Stmt<T>>& body;

 public:
  Function(
      const Token& name,
      const std::vector<Token>& params,
      const std::vector<Stmt<T>>& body);

  const T accept(const Visitor<T>& visitor) const;
};


template <typename T>
class Class : public Stmt<T> {
 private:
  const Token& name;
  const lox::expr::Variable<T>& superclass;
  const std::vector<lox::stmt::Function<T>>& methods;

 public:
  Class(
      const Token& name,
      const lox::expr::Variable<T>& superclass,
      const std::vector<lox::stmt::Function<T>>& methods);

  const T accept(const Visitor<T>& visitor) const;

  const Token& getName() const;
  const lox::expr::Variable<T>& getSuperclass() const;
  const std::vector<lox::stmt::Function<T>>& getMethods() const;
};


template <class T>
class If : public Stmt<T> {
 private:
  const lox::expr::Expr<T>& condition;
  const Stmt<T>* thenBranch;
  const Stmt<T>* elseBranch;

 public:
  If(const lox::expr::Expr<T>& condition,
     const Stmt<T>* thenBranch,
     const Stmt<T>* elseBranch);

  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr<T>& getCondition() const;
  const Stmt<T>& getThenBranch() const;
  const Stmt<T>& getElseBranch() const;
};


template <class T>
class Print : public Stmt<T> {
 private:
  const lox::expr::Expr<T>& expression;

 public:
  Print(const lox::expr::Expr<T>& expression);

  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr<T>& getExpression() const;
};


template <class T>
class Return : public Stmt<T> {
 private:
  const Token& keyword;
  const lox::expr::Expr<T>& value;

 public:
  Return(const Token& keyword, const lox::expr::Expr<T>& value);

  const T accept(const Visitor<T>& visitor) const;

  const Token& getKeyword() const;
  const lox::expr::Expr<T>& getValue() const;
};


template <class T>
class Var : public Stmt<T> {
 private:
  const Token& name;
  const lox::expr::Expr<T>& initializer;

 public:
  Var(const Token& name, const lox::expr::Expr<T>& initializer);

  const T accept(const Visitor<T>& visitor) const;
};


template <class T>
class While : public Stmt<T> {
 private:
  const lox::expr::Expr<T>& condition;
  const Stmt<T>& body;

 public:
  While(const lox::expr::Expr<T>& condition, const Stmt<T>& body);

  const T accept(const Visitor<T>& visitor) const;

  const lox::expr::Expr<T>& getCondition() const;
  const Stmt<T>& getBody() const;
};


// template <class T>
// T accept(const Visitor<T>& visitor);


template <class T>
class Visitor : public Stmt<T> {
 public:
  virtual T visitBlockStmt(const Block<T>& stmt) const = 0;
  virtual T visitClassStmt(const Class<T>& stmt) const = 0;
  virtual T visitExpressionStmt(const Expression<T>& stmt) const = 0;
  virtual T visitFunctionStmt(const Function<T>& stmt) const = 0;
  virtual T visitIfStmt(const If<T>& stmt) const = 0;
  virtual T visitPrintStmt(const Print<T>& stmt) const = 0;
  virtual T visitReturnStmt(const Return<T>& stmt) const = 0;
  virtual T visitVarStmt(const Var<T>& stmt) const = 0;
  virtual T visitWhileStmt(const While<T>& stmt) const = 0;
};


}  // namespace stmt

}  // namespace lox


#endif

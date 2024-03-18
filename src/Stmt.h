#ifndef STMT_H
#define STMT_H

#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

namespace stmt {

template <class T>
class Stmt {};


template <class T>
class Visitor;


template <typename T>
class Block : public Stmt<T> {
 private:
  const std::vector<Stmt<T>> statements;

 public:
  Block(const std::vector<Stmt<T>>& statements);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Expression : public Stmt<T> {
 private:
  const lox::expr::Expr<T> expression;

 public:
  Expression(const lox::expr::Expr<T>& expression);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Function : public Stmt<T> {
 private:
  const Token name;
  const std::vector<Token> params;
  const std::vector<Stmt<T>> body;

 public:
  Function(
      const Token& name,
      const std::vector<Token>& params,
      const std::vector<Stmt<T>>& body);

  T accept(const Visitor<T>& visitor);
};


template <typename T>
class Class : public Stmt<T> {
 private:
  const Token name;
  const lox::expr::Variable<T> superclass;
  const std::vector<lox::stmt::Function<T>> methods;

 public:
  Class(
      const Token& name,
      const lox::expr::Variable<T>& superclass,
      const std::vector<lox::stmt::Function<T>>& methods);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class If : public Stmt<T> {
 private:
  const lox::expr::Expr<T> condition;
  const Stmt<T> thenBranch;
  const Stmt<T> elseBranch;

 public:
  If(const lox::expr::Expr<T>& condition,
     const Stmt<T>& thenBranch,
     const Stmt<T>& elseBranch);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Print : public Stmt<T> {
 private:
  const lox::expr::Expr<T> expression;

 public:
  Print(const lox::expr::Expr<T>& expression);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Return : public Stmt<T> {
 private:
  const Token keyword;
  const lox::expr::Expr<T> value;

 public:
  Return(const Token& keyword, const lox::expr::Expr<T>& value);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class Var : public Stmt<T> {
 private:
  const Token name;
  const lox::expr::Expr<T> initializer;

 public:
  Var(const Token& name, const lox::expr::Expr<T>& initializer);

  T accept(const Visitor<T>& visitor);
};


template <class T>
class While : public Stmt<T> {
 private:
  const lox::expr::Expr<T> condition;
  const Stmt<T> body;

 public:
  While(const lox::expr::Expr<T>& condition, const Stmt<T>& body);

  T accept(const Visitor<T>& visitor);
};


template <class T>
T accept(const Visitor<T>& visitor);


template <class T>
class Visitor : public Stmt<T> {
 public:
  virtual T visitBlockStmt(const Block<T>& stmt) = 0;
  virtual T visitClassStmt(const Class<T>& stmt) = 0;
  virtual T visitExpressionStmt(const Expression<T>& stmt) = 0;
  virtual T visitFunctionStmt(const Function<T>& stmt) = 0;
  virtual T visitIfStmt(const If<T>& stmt) = 0;
  virtual T visitPrintStmt(const Print<T>& stmt) = 0;
  virtual T visitReturnStmt(const Return<T>& stmt) = 0;
  virtual T visitVarStmt(const Var<T>& stmt) = 0;
  virtual T visitWhileStmt(const While<T>& stmt) = 0;
};


}  // namespace stmt

}  // namespace lox

#endif

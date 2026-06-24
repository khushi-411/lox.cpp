#pragma once

#ifndef STMT_H
#define STMT_H

#include <stdexcept>
#include <vector>

#include "Expr.h"
#include "Token.h"


namespace lox {

namespace stmt {


// forward declarations

class Stmt;
class Block;
class Class;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class While;


// visitor interface

class StmtVisitor {
 public:
  virtual ~StmtVisitor() = default;

  virtual void visitBlockStmt(const Block& stmt) = 0;
  virtual void visitClassStmt(const Class& stmt) = 0;
  virtual void visitExpressionStmt(const Expression& stmt) = 0;
  virtual void visitFunctionStmt(const Function& stmt) = 0;
  virtual void visitIfStmt(const If& stmt) = 0;
  virtual void visitPrintStmt(const Print& stmt) = 0;
  virtual void visitReturnStmt(const Return& stmt) = 0;
  virtual void visitVarStmt(const Var& stmt) = 0;
  virtual void visitWhileStmt(const While& stmt) = 0;
};


// stmt base class

class Stmt {
 public:
  virtual ~Stmt() = default;
  virtual void accept(StmtVisitor& visitor) const = 0;
};


// block stmt

class Block : public Stmt {
 private:
  std::vector<std::shared_ptr<Stmt>> statements;

 public:
  Block(std::vector<std::shared_ptr<Stmt>> statements);

  void accept(StmtVisitor& visitor) const override;

  const std::vector<std::shared_ptr<Stmt>>& getStatements() const;
};


// expression stmt

class Expression : public Stmt {
 private:
  std::shared_ptr<lox::expr::Expr> expression;

 public:
  Expression(std::shared_ptr<lox::expr::Expr> expression);

  void accept(StmtVisitor& visitor) const override;

  const std::shared_ptr<lox::expr::Expr>& getExpression() const;
};


// function stmt

class Function : public Stmt {
 private:
  Token name;
  std::vector<Token> params;
  std::vector<std::shared_ptr<Stmt>> body;

 public:
  Function(
      const Token& name,
      std::vector<Token> params,
      std::vector<std::shared_ptr<Stmt>> body);

  void accept(StmtVisitor& visitor) const override;

  const Token& getName() const;
  const std::vector<Token>& getParams() const;
  const std::vector<std::shared_ptr<Stmt>>& getBody() const;
};


// class stmt

class Class : public Stmt {
 private:
  Token name;
  std::shared_ptr<lox::expr::Variable> superclass;
  std::vector<std::shared_ptr<lox::stmt::Function>> methods;

 public:
  Class(
      const Token& name,
      std::shared_ptr<lox::expr::Variable> superclass,
      std::vector<std::shared_ptr<lox::stmt::Function>> methods);

  void accept(StmtVisitor& visitor) const override;

  const Token& getName() const;
  const std::shared_ptr<lox::expr::Variable>& getSuperclass() const;
  const std::vector<std::shared_ptr<lox::stmt::Function>>& getMethods() const;
};


// if stmt

class If : public Stmt {
 private:
  std::shared_ptr<lox::expr::Expr> condition;
  std::shared_ptr<Stmt> thenBranch;
  std::shared_ptr<Stmt> elseBranch;

 public:
  If(std::shared_ptr<lox::expr::Expr> condition,
     std::shared_ptr<Stmt> thenBranch,
     std::shared_ptr<Stmt> elseBranch);

  void accept(StmtVisitor& visitor) const override;

  const std::shared_ptr<lox::expr::Expr>& getCondition() const;
  const std::shared_ptr<Stmt>& getThenBranch() const;
  const std::shared_ptr<Stmt>& getElseBranch() const;
};


// print stmt

class Print : public Stmt {
 private:
  std::shared_ptr<lox::expr::Expr> expression;

 public:
  Print(std::shared_ptr<lox::expr::Expr> expression);

  void accept(StmtVisitor& visitor) const override;

  const std::shared_ptr<lox::expr::Expr>& getExpression() const;
};


// return stmt

class Return : public Stmt {
 private:
  Token keyword;
  std::shared_ptr<lox::expr::Expr> value;

 public:
  Return(const Token& keyword, std::shared_ptr<lox::expr::Expr> value);

  void accept(StmtVisitor& visitor) const override;

  const Token& getKeyword() const;
  const std::shared_ptr<lox::expr::Expr>& getValue() const;
};


// var stmt

class Var : public Stmt {
 private:
  Token name;
  std::shared_ptr<lox::expr::Expr> initializer;

 public:
  Var(const Token& name, std::shared_ptr<lox::expr::Expr> initializer);

  void accept(StmtVisitor& visitor) const override;

  const Token& getName() const;
  const std::shared_ptr<lox::expr::Expr>& getInitializer() const;
};


// while stmt

class While : public Stmt {
 private:
  std::shared_ptr<lox::expr::Expr> condition;
  std::shared_ptr<Stmt> body;

 public:
  While(std::shared_ptr<lox::expr::Expr> condition, std::shared_ptr<Stmt> body);

  void accept(StmtVisitor& visitor) const override;

  const std::shared_ptr<lox::expr::Expr>& getCondition() const;
  const std::shared_ptr<Stmt>& getBody() const;
};


}  // namespace stmt

}  // namespace lox


#endif

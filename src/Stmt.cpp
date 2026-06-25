#include <utility>
#include <vector>

#include "Stmt.h"


namespace lox {

namespace stmt {


// block

Block::Block(std::vector<std::shared_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}


void Block::accept(StmtVisitor& visitor) const {
  visitor.visitBlockStmt(*this);
}


const std::vector<std::shared_ptr<Stmt>>& Block::getStatements() const {
  return statements;
}


// expression

Expression::Expression(std::shared_ptr<lox::expr::Expr> expression)
    : expression(std::move(expression)) {}


void Expression::accept(StmtVisitor& visitor) const {
  visitor.visitExpressionStmt(*this);
}


const std::shared_ptr<lox::expr::Expr>& Expression::getExpression() const {
  return expression;
}


// function

Function::Function(
    const Token& name,
    std::vector<Token> params,
    std::vector<std::shared_ptr<Stmt>> body)
    : name(name), params(std::move(params)), body(std::move(body)) {}


void Function::accept(StmtVisitor& visitor) const {
  visitor.visitFunctionStmt(*this);
}


const Token& Function::getName() const {
  return name;
}


const std::vector<Token>& Function::getParams() const {
  return params;
}


const std::vector<std::shared_ptr<Stmt>>& Function::getBody() const {
  return body;
}


// class

Class::Class(
    const Token& name,
    std::shared_ptr<lox::expr::Variable> superclass,
    std::vector<std::shared_ptr<lox::stmt::Function>> methods)
    : name(name),
      superclass(std::move(superclass)),
      methods(std::move(methods)) {}


void Class::accept(StmtVisitor& visitor) const {
  visitor.visitClassStmt(*this);
}


const Token& Class::getName() const {
  return name;
}


const std::shared_ptr<lox::expr::Variable>& Class::getSuperclass() const {
  return superclass;
}


const std::vector<std::shared_ptr<lox::stmt::Function>>& Class::getMethods()
    const {
  return methods;
}


// if

If::If(
    std::shared_ptr<lox::expr::Expr> condition,
    std::shared_ptr<Stmt> thenBranch,
    std::shared_ptr<Stmt> elseBranch)
    : condition(std::move(condition)),
      thenBranch(std::move(thenBranch)),
      elseBranch(std::move(elseBranch)) {}


void If::accept(StmtVisitor& visitor) const {
  visitor.visitIfStmt(*this);
}


const std::shared_ptr<lox::expr::Expr>& If::getCondition() const {
  return condition;
}


const std::shared_ptr<lox::stmt::Stmt>& If::getThenBranch() const {
  return thenBranch;
}


const std::shared_ptr<lox::stmt::Stmt>& If::getElseBranch() const {
  return elseBranch;
}


// print

Print::Print(std::shared_ptr<lox::expr::Expr> expression)
    : expression(std::move(expression)) {}


void Print::accept(StmtVisitor& visitor) const {
  visitor.visitPrintStmt(*this);
}


const std::shared_ptr<lox::expr::Expr>& Print::getExpression() const {
  return expression;
}


// return

Return::Return(const Token& keyword, std::shared_ptr<lox::expr::Expr> value)
    : keyword(keyword), value(std::move(value)) {}


void Return::accept(StmtVisitor& visitor) const {
  visitor.visitReturnStmt(*this);
}


const Token& Return::getKeyword() const {
  return keyword;
}


const std::shared_ptr<lox::expr::Expr>& Return::getValue() const {
  return value;
}


// var

Var::Var(const Token& name, std::shared_ptr<lox::expr::Expr> initializer)
    : name(name), initializer(std::move(initializer)) {}


void Var::accept(StmtVisitor& visitor) const {
  visitor.visitVarStmt(*this);
}


const Token& Var::getName() const {
  return name;
}


const std::shared_ptr<lox::expr::Expr>& Var::getInitializer() const {
  return initializer;
}


// while

While::While(
    std::shared_ptr<lox::expr::Expr> condition,
    std::shared_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}


void While::accept(StmtVisitor& visitor) const {
  visitor.visitWhileStmt(*this);
}


const std::shared_ptr<lox::expr::Expr>& While::getCondition() const {
  return condition;
}


const std::shared_ptr<lox::stmt::Stmt>& While::getBody() const {
  return body;
}


}  // namespace stmt

}  // namespace lox

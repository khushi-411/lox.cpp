#ifndef INTERPRETER_H
#define INTERPRETER_H

namespace lox {

class Interpreter : Expr::Visitor<std::string> {  // TODO: java object to string

 public:
  std::string visitLiteralExpr(Expr::Literal expr);
  std::string visitGroupingExpr(Expr::Grouping expr);
  std::string evaluate(Expr::Expr expr);
  std::string visitUnaryExpr(Expr::Unary expr);
  void checkNumberOperand(Token op, std::string operand);
  bool isTruthy(std::string object);
  std::string visitBinaryExpr(Expr::Binary expr);
  void checkNumberOperands(Token op, std::string left, std::string right);
  bool isEqual(std::string a, std::string b);

  void interpret(Expr expression);
  std::string stringify(std::string object);
};

}  // namespace lox

#endif

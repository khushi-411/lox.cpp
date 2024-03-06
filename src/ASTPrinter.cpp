#include <iostream>
#include <vector>
#include <string.h>

#include "ASTPrinter.h"
#include "Token.h"

namespace lox {

std::string Visitor::print(Expr expr) {
    return expr::accept(this);
}

std::string visitBinaryExpr(Expr::Binary expr) {
    return parenthesize(expr::op::lexeme, expr::left, expr::right);
}

std::string visitGroupingExpr(Expr::Grouping expr) {
    return parenthesize("group", expr::expression);
}

std::string visitLiteralExpr(Expr::Literal expr) {
    if (expr::value == NULL) {
        return "nil";
    }
    return expr::value::to_string();
}

std::string visitUnaryExpr(Expr::Unary expr) {
    return parenthesize(expr::op::lexeme, expr::right);
}

template <typename T, typename Expr... expr>
std::string parenthesize(std::string name, Expr... exprs) {
    std::vector<std::string> builder;
    builder.push_back("(");
    builder.push_back(name);
    for (Expr expr : exprs) {
        builder.push_back(" ");
        builder.push_back(expr::accept(this));
    }
    builder.push_back(")");
    return builder.to_string();
}

static void main(std::vector<std::string> args) {
    Expr expression = Expr::Binary(
        Expr::Unary(
            Token(TokenType::MINUS, "-", NULL, 1),
            Expr::Literal(123)),
        Token(TokenType::STAR, "*", NULL, 1),
        Expr::Grouping(
            Expr::Literal(45.67)));
    std::cout << ASTPrinter().print(expression);
}

}  // namespace lox

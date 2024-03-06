#include "Interpreter.h"
#include "TokenType.h"


namespace lox {

std::string Interpreter::visitLiteralExpr(Expr::Literal expr) {
    return expr::value;
}


std::string Interpreter::visitGroupingExpr(Expr::Grouping expr) {
    return Interpreter::evaluate(Expr::expression);
}


std::string Interpreter::evaluate(Expr expr) {
    return Expr::expr.accept(this);
}


std::string Interpreter::visitUnaryExpr(Expr::Unary expr) {
    std::string right = Interpreter::evaluate(expr.right);

    switch (expr.op.type) {
        case TokenType::BANG:
            checkNumberOperands(expr.op, right);
            return !Interpreter::isTruthy(right);
        case TokenType::MINUS:
            checkNumberOperands(expr.op, right);
            return -(double)right;
    }

    return NULL;
}


void checkNumberOperand(Token op, std::string operand) {
    if (typeid(operand).name() == double) {
        return;
    }
    throw RuntimeError(op, "Operand must be a number.");
}


bool Interpreter::isTruthy(std::string object) {
    if (object == NULL) {
        return false;
    }
    if (typeid(object).name() == bool) {
        return bool(object);
    }
    return true;
}


std::string Interpreter::visitBinaryExpr(Expr::Binary expr) {
    std::string left = Interpreter::evaluate(expr.left);
    std::string right = Interpreter::evaluate(expr.right);

    switch (expr.op.type) {
        case TokenType::MINUS:
            checkNumberOperands(expr.op, left, right);
            return (double)left - (double)right;
        case TokenType::PLUS:
            checkNumberOperands(expr.op, left, right);
            if (typeid(left).name() == double && typeid(right).name() == double) {
                return (double)left + (double)right;
            }
            if (typeid(left).name() == std::string && typeid(right).name() == std::string) {
                return (std::string)left + (std::string)right;
            }
            break;
        case TokenType::GREATER:
            checkNumberOperands(expr.op, left, right);
            return (double)left > (double)right;
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return (double)left >= (double)right;
        case TokenType::LESS:
            checkNumberOperands(expr.op, left, right);
            return (double)left < (double)right;
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return (double)left <= (double)right;
        case TokenType::BANG_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return !Interpreter::isEqual(left, right);
        case TokenType::EQUAL_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return !Interpreter::isEqual(left, right);
        case TokenType::SLASH:
            checkNumberOperands(expr.op, left, right);
            return (double)left / (double)right;
        case TokenType::STAR:
            checkNumberOperands(expr.op, left, right);
            return (double)left * (double)right;
    }

    return NULL;
}


void checkNumberOperands(Token op, std::string left, std::string right) {
    if (typeid(left).name() == double && typeid(right).name() == double) {
        return;
    }
    throw RuntimeError(op, "Operands must be numbers.");
}


bool Interpreter::isEqual(std::string a, std::string b) {
    if (a == NULL && b == NULL) {
        return true;
    }
    if (a == NULL) {
        return false;
    }
    return a.equals(b);
}


void Interpreter::interpret(Expr expression) {
    try {
        std::string value = Interpreter::evaluate(expression);
        std::cout << Interpreter::stringify(value);
    } catch (RuntimeError::RuntimeError error) {
        Lox::runtimeError(error);
    }
}


std::string stringify(std::string object) {
    if (object == NULL) {
        return NULL;
    }

    if (typeid(object).name() == double) {
        std::string text = object.to_string();
        if (text.endswidth(".0")) {
            text = text.substr(0, strlen(text) - 2);
        }
        return text;
    }

    return object.to_string();
}

}  // namespace lox

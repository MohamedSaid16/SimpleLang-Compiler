#include "Interpreter.h"
#include "../runtime/StandardLibrary.h"
#include <iostream>
#include <sstream>
#include <limits>

Interpreter::Interpreter() : hasReturn(false) {
    globalEnv = std::make_shared<Environment>();
    currentEnv = globalEnv;
    defineNativeFunctions();
}

Value Interpreter::evaluate(const ExprPtr& expr) {
    return expr->accept(*this);
}

void Interpreter::execute(const StmtPtr& stmt) {
    stmt->accept(*this);
}

void Interpreter::executeBlock(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> env) {
    auto previousEnv = currentEnv;
    currentEnv = env;
    
    try {
        for (auto& stmt : statements) {
            execute(stmt);
            if (hasReturn) {
                break;
            }
        }
    } catch (...) {
        currentEnv = previousEnv;
        throw;
    }
    
    currentEnv = previousEnv;
}

int Interpreter::toInt(const Value& value) {
    if (std::holds_alternative<int>(value)) return std::get<int>(value);
    if (std::holds_alternative<float>(value)) return static_cast<int>(std::get<float>(value));
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value) ? 1 : 0;
    throw std::runtime_error("Cannot convert to int");
}

float Interpreter::toFloat(const Value& value) {
    if (std::holds_alternative<float>(value)) return std::get<float>(value);
    if (std::holds_alternative<int>(value)) return static_cast<float>(std::get<int>(value));
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value) ? 1.0f : 0.0f;
    throw std::runtime_error("Cannot convert to float");
}

bool Interpreter::toBool(const Value& value) {
    return Environment::isTruthy(value);
}

std::string Interpreter::toString(const Value& value) {
    return Environment::valueToString(value);
}

Value Interpreter::add(const Value& left, const Value& right) {
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
        return std::get<int>(left) + std::get<int>(right);
    }
    if ((std::holds_alternative<int>(left) || std::holds_alternative<float>(left)) &&
        (std::holds_alternative<int>(right) || std::holds_alternative<float>(right))) {
        return toFloat(left) + toFloat(right);
    }
    if (std::holds_alternative<std::string>(left) || std::holds_alternative<std::string>(right)) {
        return toString(left) + toString(right);
    }
    throw std::runtime_error("Invalid operands for addition");
}

Value Interpreter::subtract(const Value& left, const Value& right) {
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
        return std::get<int>(left) - std::get<int>(right);
    }
    if ((std::holds_alternative<int>(left) || std::holds_alternative<float>(left)) &&
        (std::holds_alternative<int>(right) || std::holds_alternative<float>(right))) {
        return toFloat(left) - toFloat(right);
    }
    throw std::runtime_error("Invalid operands for subtraction");
}

Value Interpreter::multiply(const Value& left, const Value& right) {
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
        return std::get<int>(left) * std::get<int>(right);
    }
    if ((std::holds_alternative<int>(left) || std::holds_alternative<float>(left)) &&
        (std::holds_alternative<int>(right) || std::holds_alternative<float>(right))) {
        return toFloat(left) * toFloat(right);
    }
    throw std::runtime_error("Invalid operands for multiplication");
}

Value Interpreter::divide(const Value& left, const Value& right) {
    if ((std::holds_alternative<int>(left) || std::holds_alternative<float>(left)) &&
        (std::holds_alternative<int>(right) || std::holds_alternative<float>(right))) {
        float divisor = toFloat(right);
        if (divisor == 0.0f) {
            throw std::runtime_error("Division by zero");
        }
        return toFloat(left) / divisor;
    }
    throw std::runtime_error("Invalid operands for division");
}

Value Interpreter::modulo(const Value& left, const Value& right) {
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
        int divisor = std::get<int>(right);
        if (divisor == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        return std::get<int>(left) % divisor;
    }
    throw std::runtime_error("Invalid operands for modulo");
}

bool Interpreter::equal(const Value& left, const Value& right) {
    return Environment::isEqual(left, right);
}

bool Interpreter::notEqual(const Value& left, const Value& right) {
    return !equal(left, right);
}

bool Interpreter::less(const Value& left, const Value& right) {
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
        return std::get<int>(left) < std::get<int>(right);
    }
    if ((std::holds_alternative<int>(left) || std::holds_alternative<float>(left)) &&
        (std::holds_alternative<int>(right) || std::holds_alternative<float>(right))) {
        return toFloat(left) < toFloat(right);
    }
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) < std::get<std::string>(right);
    }
    throw std::runtime_error("Invalid operands for comparison");
}

bool Interpreter::greater(const Value& left, const Value& right) {
    return less(right, left);
}

bool Interpreter::lessEqual(const Value& left, const Value& right) {
    return less(left, right) || equal(left, right);
}

bool Interpreter::greaterEqual(const Value& left, const Value& right) {
    return greater(left, right) || equal(left, right);
}

Value Interpreter::logicalAnd(const Value& left, const Value& right) {
    return toBool(left) && toBool(right);
}

Value Interpreter::logicalOr(const Value& left, const Value& right) {
    return toBool(left) || toBool(right);
}

Value Interpreter::logicalNot(const Value& value) {
    return !toBool(value);
}

void Interpreter::runtimeError(const Token& token, const std::string& message) {
    errors.push_back(Error(ErrorType::RUNTIME, message, token.line, token.column, "Interpreter"));
}

// Expression visitors
Value Interpreter::visitLiteralExpr(const LiteralExpr& expr) {
    return expr.value;
}

Value Interpreter::visitVariableExpr(const VariableExpr& expr) {
    try {
        return currentEnv->get(expr.name.lexeme);
    } catch (const std::runtime_error& e) {
        runtimeError(expr.name, e.what());
        return nullptr;
    }
}

Value Interpreter::visitBinaryExpr(const BinaryExpr& expr) {
    Value left = evaluate(expr.left);
    Value right = evaluate(expr.right);
    
    try {
        switch (expr.op.type) {
            case TokenType::PLUS: return add(left, right);
            case TokenType::MINUS: return subtract(left, right);
            case TokenType::MULTIPLY: return multiply(left, right);
            case TokenType::DIVIDE: return divide(left, right);
            case TokenType::MODULO: return modulo(left, right);
            case TokenType::EQUAL: return equal(left, right);
            case TokenType::NOT_EQUAL: return notEqual(left, right);
            case TokenType::LESS: return less(left, right);
            case TokenType::GREATER: return greater(left, right);
            case TokenType::LESS_EQUAL: return lessEqual(left, right);
            case TokenType::GREATER_EQUAL: return greaterEqual(left, right);
            case TokenType::AND: return logicalAnd(left, right);
            case TokenType::OR: return logicalOr(left, right);
            default:
                runtimeError(expr.op, "Unknown binary operator");
                return nullptr;
        }
    } catch (const std::runtime_error& e) {
        runtimeError(expr.op, e.what());
        return nullptr;
    }
}

Value Interpreter::visitUnaryExpr(const UnaryExpr& expr) {
    Value right = evaluate(expr.right);
    
    try {
        switch (expr.op.type) {
            case TokenType::MINUS:
                if (std::holds_alternative<int>(right)) {
                    return -std::get<int>(right);
                }
                if (std::holds_alternative<float>(right)) {
                    return -std::get<float>(right);
                }
                throw std::runtime_error("Invalid operand for negation");
            case TokenType::NOT:
                return logicalNot(right);
            default:
                runtimeError(expr.op, "Unknown unary operator");
                return nullptr;
        }
    } catch (const std::runtime_error& e) {
        runtimeError(expr.op, e.what());
        return nullptr;
    }
}

Value Interpreter::visitCallExpr(const CallExpr& expr) {
    Value callee;
    try {
        callee = currentEnv->get(expr.callee.lexeme);
    } catch (const std::runtime_error& e) {
        runtimeError(expr.callee, e.what());
        return nullptr;
    }
    
    std::vector<Value> arguments;
    for (auto& arg : expr.arguments) {
        arguments.push_back(evaluate(arg));
    }
    
    if (std::holds_alternative<FunctionObject>(callee)) {
        auto& func = std::get<FunctionObject>(callee);
        
        if (arguments.size() != func.parameters.size()) {
            runtimeError(expr.callee, "Expected " + std::to_string(func.parameters.size()) + 
                         " arguments but got " + std::to_string(arguments.size()));
            return nullptr;
        }
        
        auto env = std::make_shared<Environment>(func.closure);
        
        for (size_t i = 0; i < arguments.size(); i++) {
            env->define(func.parameters[i].first, arguments[i]);
        }
        
        auto previousEnv = currentEnv;
        currentEnv = env;
        hasReturn = false;
        
        try {
            auto block = std::dynamic_pointer_cast<BlockStmt>(func.body);
            if (block) {
                executeBlock(block->statements, env);
            }
        } catch (...) {
            currentEnv = previousEnv;
            throw;
        }
        
        currentEnv = previousEnv;
        
        if (hasReturn) {
            return returnValue;
        }
        
        return nullptr;
    }
    
    runtimeError(expr.callee, "Can only call functions");
    return nullptr;
}

Value Interpreter::visitAssignmentExpr(const AssignmentExpr& expr) {
    Value value = evaluate(expr.value);
    try {
        currentEnv->assign(expr.name.lexeme, value);
    } catch (const std::runtime_error& e) {
        runtimeError(expr.name, e.what());
    }
    return value;
}

// Statement visitors
void Interpreter::visitPrintStmt(const PrintStmt& stmt) {
    for (size_t i = 0; i < stmt.expressions.size(); i++) {
        Value value = evaluate(stmt.expressions[i]);
        std::cout << toString(value);
        if (i < stmt.expressions.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void Interpreter::visitVariableDeclStmt(const VariableDeclStmt& stmt) {
    Value value = nullptr;
    if (stmt.initializer) {
        value = evaluate(stmt.initializer);
    }
    currentEnv->define(stmt.name.lexeme, value);
}

void Interpreter::visitExpressionStmt(const ExpressionStmt& stmt) {
    evaluate(stmt.expression);
}

void Interpreter::visitBlockStmt(const BlockStmt& stmt) {
    auto env = std::make_shared<Environment>(currentEnv);
    executeBlock(stmt.statements, env);
}

void Interpreter::visitIfStmt(const IfStmt& stmt) {
    if (toBool(evaluate(stmt.condition))) {
        execute(stmt.thenBranch);
    } else if (stmt.elseBranch) {
        execute(stmt.elseBranch);
    }
}

void Interpreter::visitWhileStmt(const WhileStmt& stmt) {
    while (toBool(evaluate(stmt.condition))) {
        execute(stmt.body);
    }
}

void Interpreter::visitFunctionDeclStmt(const FunctionDeclStmt& stmt) {
    FunctionObject func;
    func.parameters = stmt.parameters;
    func.returnType = stmt.returnType;
    func.body = stmt.body;
    func.closure = currentEnv;
    
    currentEnv->define(stmt.name.lexeme, func);
}

void Interpreter::visitReturnStmt(const ReturnStmt& stmt) {
    if (stmt.value) {
        returnValue = evaluate(stmt.value);
    } else {
        returnValue = nullptr;
    }
    hasReturn = true;
}

void Interpreter::interpret(const ProgramPtr& program) {
    try {
        for (auto& stmt : program->statements) {
            execute(stmt);
        }
    } catch (const std::runtime_error& e) {
        // Errors are already recorded via runtimeError
    }
}

void Interpreter::defineNativeFunctions() {
    // This would register standard library functions
    // Implementation depends on StandardLibrary.cpp
}
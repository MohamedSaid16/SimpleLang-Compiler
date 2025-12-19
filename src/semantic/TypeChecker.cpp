#include "TypeChecker.h"
#include <iostream>

TypeChecker::TypeChecker() 
    : currentScope(std::make_shared<SymbolTable>()), currentReturnType(TokenType::VOID_TYPE), inFunction(false) {}

TokenType TypeChecker::getExpressionType(const ExprPtr& expr) {
    // This would be implemented to return the type of an expression
    // For now, return a placeholder
    return TokenType::INT_TYPE;
}

bool TypeChecker::checkType(TokenType expected, TokenType actual, const Token& token) {
    if (expected != actual) {
        reportError(token, "Type mismatch. Expected " + 
            std::to_string(static_cast<int>(expected)) + 
            ", got " + std::to_string(static_cast<int>(actual)));
        return false;
    }
    return true;
}

bool TypeChecker::isNumericType(TokenType type) {
    return type == TokenType::INT_TYPE || type == TokenType::FLOAT_TYPE;
}

bool TypeChecker::isBooleanType(TokenType type) {
    return type == TokenType::BOOL_TYPE;
}

bool TypeChecker::isStringType(TokenType type) {
    return type == TokenType::STRING_TYPE;
}

bool TypeChecker::typesCompatible(TokenType t1, TokenType t2) {
    if (t1 == t2) return true;
    
    // Allow implicit int to float conversion
    if ((t1 == TokenType::INT_TYPE && t2 == TokenType::FLOAT_TYPE) ||
        (t1 == TokenType::FLOAT_TYPE && t2 == TokenType::INT_TYPE)) {
        return true;
    }
    
    return false;
}

TokenType TypeChecker::getBinaryResultType(TokenType left, TokenType right, TokenType op) {
    // Arithmetic operators
    if (op == TokenType::PLUS || op == TokenType::MINUS || 
        op == TokenType::MULTIPLY || op == TokenType::DIVIDE || 
        op == TokenType::MODULO) {
        
        if (left == TokenType::INT_TYPE && right == TokenType::INT_TYPE) {
            return TokenType::INT_TYPE;
        }
        if ((left == TokenType::INT_TYPE || left == TokenType::FLOAT_TYPE) &&
            (right == TokenType::INT_TYPE || right == TokenType::FLOAT_TYPE)) {
            return TokenType::FLOAT_TYPE;
        }
    }
    
    // Comparison operators
    if (op == TokenType::EQUAL || op == TokenType::NOT_EQUAL ||
        op == TokenType::LESS || op == TokenType::LESS_EQUAL ||
        op == TokenType::GREATER || op == TokenType::GREATER_EQUAL) {
        return TokenType::BOOL_TYPE;
    }
    
    // Logical operators
    if (op == TokenType::AND || op == TokenType::OR) {
        if (left == TokenType::BOOL_TYPE && right == TokenType::BOOL_TYPE) {
            return TokenType::BOOL_TYPE;
        }
    }
    
    return TokenType::ERROR;
}

void TypeChecker::reportError(const Token& token, const std::string& message) {
    errors.push_back(Error(ErrorType::SEMANTIC, message, token.line, token.column, "TypeChecker"));
}

// Expression visitors
Value TypeChecker::visitLiteralExpr(const LiteralExpr& expr) {
    // Literals have inherent types based on their value
    return nullptr;
}

Value TypeChecker::visitVariableExpr(const VariableExpr& expr) {
    auto symbol = currentScope->lookup(expr.name.lexeme);
    if (!symbol) {
        reportError(expr.name, "Undefined variable '" + expr.name.lexeme + "'");
        return nullptr;
    }
    return nullptr;
}

Value TypeChecker::visitBinaryExpr(const BinaryExpr& expr) {
    expr.left->accept(*this);
    expr.right->accept(*this);
    return nullptr;
}

Value TypeChecker::visitUnaryExpr(const UnaryExpr& expr) {
    expr.right->accept(*this);
    return nullptr;
}

Value TypeChecker::visitCallExpr(const CallExpr& expr) {
    for (auto& arg : expr.arguments) {
        arg->accept(*this);
    }
    return nullptr;
}

Value TypeChecker::visitAssignmentExpr(const AssignmentExpr& expr) {
    expr.value->accept(*this);
    return nullptr;
}

// Statement visitors
void TypeChecker::visitPrintStmt(const PrintStmt& stmt) {
    for (auto& expr : stmt.expressions) {
        expr->accept(*this);
    }
}

void TypeChecker::visitVariableDeclStmt(const VariableDeclStmt& stmt) {
    if (stmt.initializer) {
        stmt.initializer->accept(*this);
    }
}

void TypeChecker::visitExpressionStmt(const ExpressionStmt& stmt) {
    if (stmt.expression) {
        stmt.expression->accept(*this);
    }
}

void TypeChecker::visitBlockStmt(const BlockStmt& stmt) {
    auto oldScope = currentScope;
    currentScope = std::make_shared<SymbolTable>(oldScope->getScopeLevel() + 1, oldScope);
    
    for (auto& stmtPtr : stmt.statements) {
        stmtPtr->accept(*this);
    }
    
    currentScope = oldScope;
}

void TypeChecker::visitIfStmt(const IfStmt& stmt) {
    if (stmt.condition) {
        stmt.condition->accept(*this);
    }
    if (stmt.thenBranch) {
        stmt.thenBranch->accept(*this);
    }
    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }
}

void TypeChecker::visitWhileStmt(const WhileStmt& stmt) {
    if (stmt.condition) {
        stmt.condition->accept(*this);
    }
    if (stmt.body) {
        stmt.body->accept(*this);
    }
}

void TypeChecker::visitFunctionDeclStmt(const FunctionDeclStmt& stmt) {
    bool oldInFunction = inFunction;
    TokenType oldReturnType = currentReturnType;
    
    inFunction = true;
    currentReturnType = stmt.returnType;
    
    auto oldScope = currentScope;
    currentScope = std::make_shared<SymbolTable>(oldScope->getScopeLevel() + 1, oldScope);
    
    // Add parameters to scope
    for (auto& [paramName, paramType] : stmt.parameters) {
        auto symbol = std::make_shared<Symbol>(paramName.lexeme, SymbolType::PARAMETER, 
                                               paramType, currentScope->getScopeLevel(), true);
        currentScope->insert(symbol);
    }
    
    if (stmt.body) {
        stmt.body->accept(*this);
    }
    
    currentScope = oldScope;
    inFunction = oldInFunction;
    currentReturnType = oldReturnType;
}

void TypeChecker::visitReturnStmt(const ReturnStmt& stmt) {
    if (stmt.value) {
        stmt.value->accept(*this);
    }
}

void TypeChecker::check(const ProgramPtr& program) {
    for (auto& stmt : program->statements) {
        stmt->accept(*this);
    }
}